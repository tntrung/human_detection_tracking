#include "stdafx.h"
#include <mex.h>
#include <hog.cpp>
#include "Regions.cpp"
#include "StrongClassifier.cpp"
#include "BaseClassifier.cpp"
#include "StrongClassifierDirectSelection.cpp"
//haar
#include "WeakClassifierHaarFeature.cpp"
#include "FeatureHaar.cpp"
#include "ImageRepresentation.cpp"
//his
#include "WeakClassifierHistogramFeature.cpp"
#include "FeatureHistogram.cpp"
#include "ClassifierThreshold.cpp"
#include "WeakClassifier.cpp"
//cau truc 1 classifier
struct mf_hisstclassifier
{	
	StrongClassifier* classifier;
	Rect validROI;
	Rect trackedPatch;
	Size trackingpathsize;
	CvHistogram*hist;;
};
//lay tu mexfile 1 bien kieu int
int mf_hisgetmatrixint(const mxArray*intmt)
{	
	return (int)mxGetPr(intmt)[0];
}
//lay tu mexfile 1 bien kieu float
float mf_hisgetmatrixfloat(const mxArray*intmt)
{	
	return (float)mxGetPr(intmt)[0];
}
//lay tu mexfile 1 bien kieu char*
char* mf_hisgetcharformmxarray(const mxArray*arr)
{
	int n=(int)mxGetN(arr);	
	char*link;		
	link=new char[n+1];
	memset(link,'\0',n+1);
	mxGetString(arr,link,n+1);	
	return link;
}
//lay tu mexfile chuoi kieu int*
int* mf_hisgetmatrixintarr(const mxArray*intmt,const int&sopt)
{	
	double *temp;
	temp=mxGetPr(intmt);
	int*kq=new int[sopt];
	for(int i=0;i<sopt;i++)
		kq[i]=(int)temp[i];
	return kq;
}
//lay tu mexfile 1 bien kieu Rect
Rect mf_hisgetRect(const mxArray*rect)
{
	double *temp;
	temp=mxGetPr(rect);	
	return Rect((int)temp[0],(int)temp[1],(int)temp[2],(int)temp[3]);
}
//bat buoc vung rect phai thuoc whole
Rect mf_hisrectroi(const Rect&whole,const Rect& temp)
{
	Rect searchRegion=temp;
	if (searchRegion.left<0)
		searchRegion.left=0;
	if(searchRegion.upper<0)
		searchRegion.upper=0;
	if (searchRegion.upper+searchRegion.height > whole.height)
		searchRegion.height = whole.height-searchRegion.upper;
	if (searchRegion.left+searchRegion.width > whole.width)
		searchRegion.width = whole.width-searchRegion.left;
	return searchRegion;
}
//Vung rect mo rong tu vung rect thuoc classifier - tham so mo rong lon hon 1
Rect mf_hisgetTrackingROI(const mf_hisstclassifier &cl,const float& searchFactor)
{	
	Rect searchRegion;
	Rect temp= cl.trackedPatch;	
	searchRegion = temp*(searchFactor);
	if (searchRegion.left<0)
		searchRegion.left=0;
	if(searchRegion.upper<0)
		searchRegion.upper=0;
	if (searchRegion.upper+searchRegion.height > cl.validROI.height)
		searchRegion.height = cl.validROI.height-searchRegion.upper;
	if (searchRegion.left+searchRegion.width > cl.validROI.width)
		searchRegion.width = cl.validROI.width-searchRegion.left;
	return searchRegion;
}
//khoi tao mot classifier
mf_hisstclassifier mf_hiskhoitaoclassifer_new(const int feature,const Rect&initPatch,const Rect&validROI,const int&numBaseClassifier,const int& iterationInit=50)
{
	mf_hisstclassifier cl;		
	int numWeakClassifier = numBaseClassifier*10;
	bool useFeatureExchange = true;
	Size patchSize;
	patchSize = initPatch;
	cl.trackingpathsize=patchSize;
	cl.validROI = validROI;
	cl.classifier= new StrongClassifierDirectSelection(numBaseClassifier, numWeakClassifier, patchSize,feature, useFeatureExchange, iterationInit);
	cl.trackedPatch = initPatch;	
	return cl;
}
//kiem tra vung rect co nam trong vung rect khoac hay khong
bool mf_histhuocvungsearch(const Rect &a,const Rect &b)
{
	if(b.upper<a.upper || b.left<a.left || b.height+b.upper>a.height+a.upper || b.width+b.left>a.width+a.left)
		return false;//b thuoc a
	return true;//b khong thuoc a
}
//lay tu mexfile kieu Rect*
Rect* mf_hisgetRectmatrix(const mxArray*mximg,const int& height)
{	
	Rect*detectornew=new Rect[height];
	double *datain=mxGetPr(mximg);		
	int giatridata=0;
	for(int i=0;i<height;i++,giatridata+=4)
	{		
		detectornew[i]=Rect((int)datain[giatridata+1],(int)datain[giatridata],(int)datain[giatridata+3],(int)datain[giatridata+2]);		
	}
	return detectornew;
}
//chuen kenh mau tu bgr den rgi
IplImage* mf_hisbgr2rgi(const IplImage* bgr )
{
	IplImage*rgi = cvCreateImage( cvGetSize(bgr), IPL_DEPTH_32F, 3 );
	int height,width,step,channels;	
	int i,j,tong;
	height    = bgr->height;
	width     = bgr->width;
	step      = bgr->widthStep;
	channels  = bgr->nChannels;
	uchar *bgrdata=(uchar*)bgr->imageData;
	for(i=0;i<height;i++) for(j=0;j<width;j++)
	{			
		tong=bgrdata[i*step+j*channels]+bgrdata[i*step+j*channels+1]+bgrdata[i*step+j*channels+2];
		if(tong==0) continue;
		((float *)(rgi->imageData + i*rgi->widthStep))[j*rgi->nChannels + 0]=(float)bgrdata[i*step+j*channels+2]/tong;
		((float *)(rgi->imageData + i*rgi->widthStep))[j*rgi->nChannels + 1]=(float)bgrdata[i*step+j*channels+1]/tong;
		((float *)(rgi->imageData + i*rgi->widthStep))[j*rgi->nChannels + 2]=(float)tong/255;
	}
	return rgi;
}
// tao histogram cho 3 kenh mau
CvHistogram* mf_hisimage2histogram(const IplImage* im,const int&bins1=3,const int&bins2=3,const int&bins3=3,const float&scalemax1=1.0,const float&scalemax2=1.0,const float&scalemax3=1.0)
{	
	IplImage* channel1 = cvCreateImage( cvGetSize(im), im->depth, 1 );
	IplImage* channel2 = cvCreateImage( cvGetSize(im), im->depth, 1 );
	IplImage* channel3 = cvCreateImage( cvGetSize(im), im->depth, 1 );
	IplImage* planes[] = { channel1, channel2, channel3};
	cvCvtPixToPlane( im, channel1, channel2, channel3, 0 );	
	CvHistogram*hist;
	{
		int hist_size[] = { bins1, bins2, bins3};
		float h_ranges[] = { 0, scalemax1 }; // hue is [0,180]
		float s_ranges[] = { 0, scalemax2 };
		float v_ranges[] = { 0, scalemax3 };
		float* ranges[] = { h_ranges, s_ranges, v_ranges };
		hist = cvCreateHist(2,hist_size,CV_HIST_ARRAY,ranges,1);
	}
	cvCalcHist( planes, hist, 0, 0 ); //Compute histogram
	cvNormalizeHist( hist, 1.0 ); //Normalize it
	cvReleaseImage(&channel1);
	cvReleaseImage(&channel2);
	cvReleaseImage(&channel3);
	return hist;
}
//fix kich thuoc rect ve size qui dinh
Rect mf_hisresizerect(const Rect&wholeimage,const Rect&rect,const Size&size)
{	
	//printf("\n[%d %d %d %d",rect.left,rect.upper,rect.width,rect.height);
	Rect temp=Rect(rect.upper+(rect.height-size.height)/2,rect.left+(rect.width-size.width)/2,size.height,size.width);
	//fix ve vung rect
	if(temp.left<0)temp.left=0;
	if(temp.upper<0)temp.upper=0;
	if(temp.width>wholeimage.width-temp.left)temp.left=wholeimage.width-temp.width;
	if(temp.height>wholeimage.height-temp.upper)temp.upper=wholeimage.height-temp.height;
	//printf("\n[%d %d %d %d",temp.left,temp.upper,temp.width,temp.height);
	return temp;	
}
//fix kich thuoc ve size qui dinh dong thoi doi sang kieu du lieu CvRect
CvRect mf_hisresetsizeregion(const Rect&wholeimage,const Rect&region,const Size&patchsize)
{
	CvRect temp=cvRect(region.left+(region.width-patchsize.width)/2,region.upper+(region.height-patchsize.height)/2,patchsize.width,patchsize.height);
	//fix ve vung rect
	if(temp.x<0)temp.x=0;
	if(temp.y<0)temp.y=0;
	if(temp.width>wholeimage.width-temp.x)temp.x=wholeimage.width-temp.width;
	if(temp.height>wholeimage.height-temp.y)temp.y=wholeimage.height-temp.height;
	return temp;
}
//Tao histogram cho nhieu vung rect - fix kich thuoc ve patch size
CvHistogram** mf_hiscalchistogram(IplImage*im,const Rect&wholeimage,const Rect*region,const Size &patchsize,const int&numregion)
{
	CvHistogram**hist=new CvHistogram*[numregion];
	for(int i=0;i<numregion;i++)
	{
		CvRect regiontmp=mf_hisresetsizeregion(wholeimage,region[i],patchsize);
		cvSetImageROI(im,regiontmp);
		IplImage*tmp = cvCreateImage(cvGetSize(im), im->depth,3 );
		cvCopy(im, tmp, NULL );
		cvResetImageROI( im );		
		hist[i]=mf_hisimage2histogram(tmp,3,3,3,1.0,1.0,255.0);
		cvReleaseImage(&tmp);
	}
	return hist;
}
unsigned char* mf_hisgetdatagrayimage(const IplImage*image)
{
	//converter to gray image
	IplImage* m_grayImage = cvCreateImage(cvGetSize(image), 8, 1);
	cvCvtColor(image, m_grayImage, CV_RGB2GRAY);	

	int rows = m_grayImage->height;
    int cols = m_grayImage->width;
    int iplCols= m_grayImage->widthStep;
    
    unsigned char *dataCh = new unsigned char[rows*cols];
    unsigned char *buffer = reinterpret_cast<unsigned char*>(m_grayImage->imageData);

	for(int i=0; i<rows; i++)
    {
        memcpy(dataCh+i*cols, buffer+i*iplCols, sizeof(unsigned char) * cols);
    }
	cvReleaseImage(&m_grayImage);
    return dataCh;
}
//lat 4 vung xung quanh roi
Rect* mf_hisgetsearchregion(const Rect& ROI,const Rect& subrect)
{
	Size patchSize;
	patchSize=subrect;
	Rect* m_rect=new Rect[4];
	m_rect[0] = m_rect[1]= m_rect[2]= m_rect[3]= patchSize;	
	m_rect[0].upper = ROI.upper;
	m_rect[0].left = ROI.left;
	m_rect[1].upper = ROI.upper;
	m_rect[1].left = ROI.left+ROI.width-patchSize.width;
	m_rect[2].upper = ROI.upper+ROI.height-patchSize.height;
	m_rect[2].left = ROI.left;
	m_rect[3].upper = ROI.upper+ROI.height-patchSize.height;					
	m_rect[3].left = ROI.left+ROI.width-patchSize.width;
	return m_rect;
}
//bien toan cuc
mf_hisstclassifier *cla=NULL;//classifier duoc giu lai
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{  			
	//lat them so tu matlab
#define OPTION			prhs[0]//xac dinh khoi tao hay danh gia
#define FEATURE			prhs[1]//su dung loai feature = 0: haar, 1: histogram
#define LINKIMAGE		prhs[2]//link image
#define NUMTRACK		prhs[3]//so tracker frame hien tai
#define SEARCHREGION	prhs[4]//vung search region de danh gia

	int option						=mf_hisgetmatrixint(OPTION);		
	int feature						=mf_hisgetmatrixint(FEATURE);
	int numt						=mf_hisgetmatrixint(NUMTRACK);
	int searchregion				=mf_hisgetmatrixint(SEARCHREGION);
	IplImage*image					=cvLoadImage(mf_hisgetcharformmxarray(LINKIMAGE));
	IplImage*imcolor				=NULL;//kenh mau cho histogram feature
	unsigned char*image_haar		=NULL;//gray image cho haar feature
	CvHistogram**histotrack			=NULL;//histogram cho cac vung xu ly
	ImageRepresentation*curFrameRe	=NULL;//integral image

	if(feature==0)//haar feature (adaboost)
	{
		image_haar=mf_hisgetdatagrayimage(image);
		curFrameRe=new ImageRepresentation(image_haar,Size(image->height,image->width));
	}
	else if(feature==1)//histogram feature (adaboost)
	{
		imcolor=mf_hisbgr2rgi(image);//chuyen sang kenh mau rgi		
	}
	else if(feature==2)//histogram feature (only histogram)
	{
		imcolor=mf_hisbgr2rgi(image);//chuyen sang kenh mau rgi		
	}
	else if(feature==3)//haar + histogram (adaboost)
	{
		image_haar=mf_hisgetdatagrayimage(image);
		curFrameRe=new ImageRepresentation(image_haar,Size(image->height,image->width));
		imcolor	=mf_hisbgr2rgi(image);//chuyen sang kenh mau rgi		
	}
	
	Rect wholeimage=Rect(0,0,image->height,image->width);
	
	if(option == 0)//khoi tao, huy classfiler cho tap rect
	{
#define ARRTRACT			prhs[5]//mang gia tri vung track moi
#define NUMUPDATE			prhs[6]//mang quan ly update tracker
#define ARRUPDATE			prhs[7]//mang gia tri
#define NUMBASECLASS		prhs[8]//so baseclassifer	
#define NUMTRAININIT		prhs[9]
		
		Rect*arrtract		=mf_hisgetRectmatrix(ARRTRACT,numt);
		int numu			=mf_hisgetmatrixint(NUMUPDATE);
		int*arrupdate		=mf_hisgetmatrixintarr(ARRUPDATE,numu);
		int numb			=mf_hisgetmatrixint(NUMBASECLASS);
		int numi			=mf_hisgetmatrixint(NUMTRAININIT);

		//tao bo classifer moi qua mang update
		mf_hisstclassifier*tempcla=new mf_hisstclassifier[numt];		
		for(int i=0,ud=0,oldtrack=0;i<numt;i++,ud++)
		{			
			while(arrupdate[ud]==-1)//huy tracker (kiem tra neu khong huy thi dung
			{
				ud++;
				oldtrack++;
			}				
			if(arrupdate[ud]!=0)//dung tracker cu
			{	
				tempcla[i]=cla[oldtrack++];//lay gia tri tracker cu				
				//fix lai kich thuoc vung rect (phai thuoc whole image)
				arrtract[i]=mf_hisresizerect(wholeimage,arrtract[i],tempcla[i].trackingpathsize);				
				//neu sau khi fix no thuoc vung rect thi update con nguoc lai thi khong update
				if(mf_histhuocvungsearch(wholeimage,arrtract[i]))
				{
					//update vung tracked patch
					tempcla[i].trackedPatch=arrtract[i];//update tracker nay voi newrect[i]
					if(arrupdate[ud]==-2){continue;}//khong cap nhat	
					//get rect around
					Rect ROI = mf_hisgetTrackingROI(tempcla[i],searchregion);
					Rect*m_rect=mf_hisgetsearchregion(ROI,tempcla[i].trackedPatch);
					//update 
					if(feature==0)
					{
						for(int ra=0;ra<4;ra++)
						{
							tempcla[i].classifier->update (curFrameRe, m_rect[ra], -1);
							tempcla[i].classifier->update (curFrameRe, tempcla[i].trackedPatch, 1);
						}
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(curFrameRe,arrtract[ij],-1);
									tempcla[i].classifier->update(curFrameRe,tempcla[i].trackedPatch,1);
								}
							}
						}
					}
					else if(feature== 1)
					{
						//tinh toan histogram
						delete histotrack;
						histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram			
						tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
						CvHistogram ** histotemp=mf_hiscalchistogram(imcolor,wholeimage,m_rect,tempcla[i].trackingpathsize,4);//compute histogram			
						for(int ij=0;ij<4;ij++)
						{
							tempcla[i].classifier->update(histotemp[ij],m_rect[ij],-1);
							tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
						}
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(histotrack[ij],arrtract[ij],-1);
									tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
								}
							}
						}
						delete histotemp;
					}
					else if(feature== 2)
					{
						delete histotrack;
						histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram								
						tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
						tempcla[i].hist=NULL;
						cvCopyHist(histotrack[i],&tempcla[i].hist);
						float thresh=((rand() % 10000)*0.2f)/10000;	
						cvThreshHist(tempcla[i].hist,thresh);
					}
					else if(feature== 3)
					{
						//tinh toan histogram
						delete histotrack;
						histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram			
						CvHistogram ** histotemp=mf_hiscalchistogram(imcolor,wholeimage,m_rect,tempcla[i].trackingpathsize,4);//compute histogram			
						for(int ra=0;ra<4;ra++)
						{
							tempcla[i].classifier->update (histotemp[ra],curFrameRe, m_rect[ra], -1);
							tempcla[i].classifier->update (histotrack[i],curFrameRe, tempcla[i].trackedPatch, 1);
						}
						delete histotemp;
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(histotrack[ij],arrtract[ij],-1);
									tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
								}
							}
						}
					}
					delete m_rect;
				}				
			}
			else//khoi tao boosting khi update = 0
			{		
				//fix lai vung rect bat buoc phai nam trong whole image
				arrtract[i]=mf_hisrectroi(wholeimage,arrtract[i]);
				
				tempcla[i]=mf_hiskhoitaoclassifer_new(feature,arrtract[i],wholeimage,numb,numi);
				Rect ROI = mf_hisgetTrackingROI(tempcla[i],searchregion);
				Rect*m_rect=mf_hisgetsearchregion(ROI,tempcla[i].trackedPatch);				
				 
				if(feature==0)
				{
					//tinh toan histogram						
					//delete histotrack;
					//histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram			
 					for(int u=0;u<numi;u++)
					{
						for(int ij=0;ij<4;ij++)
						{
							tempcla[i].classifier->update(curFrameRe,m_rect[ij],-1);
							tempcla[i].classifier->update(curFrameRe,tempcla[i].trackedPatch,1);
						}
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(curFrameRe,arrtract[ij],-1);
									tempcla[i].classifier->update(curFrameRe,tempcla[i].trackedPatch,1);
								}
							}
						}
					}
				}
				else if(feature==1)
				{
					//tinh toan histogram
					delete histotrack;
					histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram			
					tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
					CvHistogram ** histotemp=mf_hiscalchistogram(imcolor,wholeimage,m_rect,tempcla[i].trackingpathsize,4);//compute histogram			
 					for(int u=0;u<numi;u++)
					{	for(int ij=0;ij<4;ij++)
						{
							tempcla[i].classifier->update(histotemp[ij],m_rect[ij],-1);
							tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
						}
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(histotrack[ij],arrtract[ij],-1);
									tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
								}
							}
						}
					}
					delete histotemp;
				}
				else if(feature==2)
				{
					//tinh toan histogram
					delete histotrack;
					histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram								
					//tempcla[i].classifier->update(histotrack[i],tempcla[i].trackedPatch,1);
					tempcla[i].hist=NULL;
					cvCopyHist(histotrack[i],&tempcla[i].hist);
					float thresh=((rand() % 10000)*0.2f)/10000;	
					cvThreshHist(tempcla[i].hist,thresh);
				}
				else if(feature==3)
				{
					//tinh toan histogram						
					delete histotrack;
					histotrack=mf_hiscalchistogram(imcolor,wholeimage,arrtract,tempcla[i].trackingpathsize,numt);//compute histogram			
					//tempcla[i].classifier->update(histotrack[i],curFrameRe,tempcla[i].trackedPatch,1);
					CvHistogram ** histotemp=mf_hiscalchistogram(imcolor,wholeimage,m_rect,tempcla[i].trackingpathsize,4);//compute histogram			
 					for(int u=0;u<numi;u++)
					{
						for(int ij=0;ij<4;ij++)
						{
							tempcla[i].classifier->update(histotemp[ij],curFrameRe,m_rect[ij],-1);
							tempcla[i].classifier->update(histotrack[i],curFrameRe,tempcla[i].trackedPatch,1);
						}
						for(int ij=0;ij<numt;ij++)
						{
							if(ij!=i)
							{
								if(mf_histhuocvungsearch(wholeimage,arrtract[ij]))
								{
									tempcla[i].classifier->update(histotrack[ij],curFrameRe,arrtract[ij],-1);
									tempcla[i].classifier->update(histotrack[i],curFrameRe,tempcla[i].trackedPatch,1);
								}
							}
						}
					}
					delete histotemp;
				}
				delete m_rect;
			}								
		}						
		delete cla;
		cla=new mf_hisstclassifier[numt];
		for(int i=0;i<numt;i++)
			cla[i]=tempcla[i];
		delete tempcla;

		delete arrtract;
		delete arrupdate;
	}
	else if(option == 1)//danh gia
	{
#define ARRDETECT		prhs[5]
#define NUMDETECT		prhs[6]
#define ARRPARTICAL		prhs[7]
#define NUMPARTICAL		prhs[8]
#define MINCONFIDENT	prhs[9]//min confident

#define DATAOUT			plhs[0]	


		int numd				=mf_hisgetmatrixint(NUMDETECT);
		Rect*newrectdetect		=mf_hisgetRectmatrix(ARRDETECT,numd);				
		int nump				=mf_hisgetmatrixint(NUMPARTICAL);			
		Rect*newrectpartical	=mf_hisgetRectmatrix(ARRPARTICAL,nump*numt);				
		float minconfident		=mf_hisgetmatrixfloat(MINCONFIDENT);
		//out put score for detect, partical, hog
		DATAOUT			=mxCreateDoubleMatrix(numd+nump+nump,numt,mxREAL);		
		double *dataout=mxGetPr(DATAOUT);				
		
		//lay confident hog
		cv::HOGDescriptor hog;	
		hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());		
		vector<cv::Point>locations;
		for(int i=0;i<numt*nump;i++)
			locations.push_back(cv::Point(newrectpartical[i].left,newrectpartical[i].upper));
		vector<float>confidenthis;		
		if(locations.size())
			hog.detect_track(cv::Mat(image),confidenthis,cv::Size(1,1),cv::Size(2,2),locations);
		hog.~HOGDescriptor();		
		
		for(int i=0,out=0;i<numt;i++)
		{			
			Rect searc=mf_hisgetTrackingROI(cla[i],searchregion);
			if(feature==0)
			{
				curFrameRe->setNewImageAndROI(image_haar, searc);	
			}
			else if(feature==1)
			{
				delete histotrack;	
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectdetect,cla[i].trackingpathsize,numd);//compute histogram			
			}
			else if(feature==2)
			{
				delete histotrack;	
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectdetect,cla[i].trackingpathsize,numd);//compute histogram			
			}
			else if(feature==3)
			{
				curFrameRe->setNewImageAndROI(image_haar, searc);	
				delete histotrack;	
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectdetect,cla[i].trackingpathsize,numd);//compute histogram			
			}
			for(int j=0;j<numd;j++)
			{	
				Rect temp=mf_hisresizerect(wholeimage,newrectdetect[j],cla[i].trackingpathsize);
				if(!mf_histhuocvungsearch(searc,temp))
					dataout[out++]=minconfident;
				else
				{
					if(feature==0)
					{
						float t=cla[i].classifier->eval(curFrameRe,temp);
						dataout[out++]=t;
					}
					else if(feature==1)
					{
						float t=cla[i].classifier->eval(histotrack[j],temp);
						dataout[out++]=t;
					}
					else if(feature==2)
					{
						float result=(cvCompareHist(histotrack[j],cla[i].hist,CV_COMP_CORREL)-0.85);		
						dataout[out++]=result;
					}
					else if(feature==3)
					{
						float t=cla[i].classifier->eval(curFrameRe,temp);
						dataout[out++]=t;
					}
				}
					
			}	
			if(feature==0)
			{
				//khong lam gia ca
			}
			else if(feature==1)
			{
				delete histotrack;
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectpartical,cla[i].trackingpathsize,nump);//compute histogram			
			}
			else if(feature==2)
			{
				delete histotrack;
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectpartical,cla[i].trackingpathsize,nump);//compute histogram			
			}
			else if(feature==3)
			{
				delete histotrack;
				histotrack=mf_hiscalchistogram(imcolor,wholeimage,newrectpartical,cla[i].trackingpathsize,nump);//compute histogram			
			}
			for(int j=0;j<nump;j++)
			{
				Rect temp=mf_hisresizerect(wholeimage,newrectpartical[j+nump*i],cla[i].trackingpathsize);
				if(!mf_histhuocvungsearch(searc,temp))
					dataout[out++]=minconfident;
				else
				{
					if(feature==0)
					{
						float t=cla[i].classifier->eval(curFrameRe,temp);
						dataout[out++]=t;
					}
					else if(feature==1)
					{
						float t=cla[i].classifier->eval(histotrack[j],temp);
						dataout[out++]=t;
					}
					else if(feature==2)
					{
						float result=(cvCompareHist(histotrack[j],cla[i].hist,CV_COMP_CORREL)-0.85);		
						dataout[out++]=result;
					}
					else if(feature==3)
					{
						float t=cla[i].classifier->eval(histotrack[j],curFrameRe,temp);
						dataout[out++]=t;
					}
				}
			}
			for(int j=0;j<nump;j++)
				dataout[out++]=confidenthis[j+nump*i];
		}
		confidenthis.clear();
		delete newrectdetect;
		delete newrectpartical;
	}
	cvReleaseImage(&image);		
	if(feature==0)
	{
		delete curFrameRe;
		delete image_haar;		
	}
	else if(feature==1)
	{
		cvReleaseImage(&imcolor);
		delete histotrack;	
	}
	else if(feature==2)
	{
		cvReleaseImage(&imcolor);
		delete histotrack;	
	}
	else if(feature==3)
	{
		delete curFrameRe;
		delete image_haar;
		cvReleaseImage(&imcolor);
		delete histotrack;	
	}
}
