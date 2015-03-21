#include "stdafx.h"
#include "FeatureHistogram.h"
#include <stdio.h>
#define SQROOTHALF 0.7071
#define INITSIGMA( numAreas ) ( static_cast<float>( sqrt( 256.0f*256.0f / 12.0f * (numAreas) ) ) );

FeatureHistogram::FeatureHistogram(Size patchSize)
: m_areas(NULL), m_weights(NULL), m_scaleAreas(NULL), m_scaleWeights(NULL)
{
	try {
		generateRandomFeature(patchSize);
	}
	catch (...) {
		delete[] m_scaleWeights;
		delete[] m_scaleAreas;
		delete[] m_areas;
		delete[] m_weights;
		throw;
	}
}


FeatureHistogram::~FeatureHistogram()
{
	delete[] m_scaleWeights;
	delete[] m_scaleAreas;
	delete[] m_areas;
	delete[] m_weights;
}
void FeatureHistogram::generateRandomFeature(Size patchSize)
{
	
	initflag=false;
	//cua haar de scale weight
	Point2D position;
	Size baseDim;
	Size sizeFactor;
	int area;

	Size minSize = Size(3,3);
	int minArea = 9;

	bool valid = false;
	while (!valid)
	{
		//chosse position and scale
		position.row = rand()%(patchSize.height);
		position.col = rand()%(patchSize.width);

		baseDim.width = (int) ((1-sqrt(1-(float)rand()/RAND_MAX))*patchSize.width);
		baseDim.height = (int) ((1-sqrt(1-(float)rand()/RAND_MAX))*patchSize.height);
		
		//select types
		//float probType[11] = {0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0909f, 0.0950f};
		float probType[11] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
		float prob = (float)rand()/RAND_MAX;

		if (prob < probType[0]) 
		{
			//check if feature is valid
			sizeFactor.height = 2;
			sizeFactor.width = 1;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type1");
			m_numAreas = 2;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 

			valid = true;

		}
		else if (prob < probType[0]+probType[1]) 
		{
			//check if feature is valid
			sizeFactor.height = 1;
			sizeFactor.width = 2;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type2");
			m_numAreas = 2;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;

		}
		else if (prob < probType[0]+probType[1]+probType[2]) 
		{
			//check if feature is valid
			sizeFactor.height = 4;
			sizeFactor.width = 1;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type3");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -2;
			m_weights[2] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = 2*baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row+3*baseDim.height;
			m_areas[2].left = position.col;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3])
		{
		//check if feature is valid
			sizeFactor.height = 1;
			sizeFactor.width = 4;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type3");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -2;
			m_weights[2] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = 2*baseDim.width;
			m_areas[2].upper = position.row;
			m_areas[2].left = position.col+3*baseDim.width;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3]+probType[4])
		{
		//check if feature is valid
			sizeFactor.height = 2;
			sizeFactor.width = 2;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type5");
			m_numAreas = 4;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -1;
			m_weights[2] = -1;
			m_weights[3] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row+baseDim.height;
			m_areas[2].left = position.col;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_areas[3].upper = position.row+baseDim.height;
			m_areas[3].left = position.col+baseDim.width;
			m_areas[3].height = baseDim.height;
			m_areas[3].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5])
		{
			//check if feature is valid
			sizeFactor.height = 3;
			sizeFactor.width = 3;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type6");
			m_numAreas = 2;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -9;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = 3*baseDim.height;
			m_areas[0].width = 3*baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_initMean = -8*128;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob< probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5]+probType[6]) 
		{
			//check if feature is valid
			sizeFactor.height = 3;
			sizeFactor.width = 1;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type7");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -2;
			m_weights[2] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row+baseDim.height*2;
			m_areas[2].left = position.col;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5]+probType[6]+probType[7])
		{
		//check if feature is valid
			sizeFactor.height = 1;
			sizeFactor.width = 3;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;

			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;

			if (area < minArea)
				continue;

			strcpy (m_type, "Type8");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -2;
			m_weights[2] = 1;
			m_areas= new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row;
			m_areas[2].left = position.col+2*baseDim.width;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5]+probType[6]+probType[7]+probType[8])
		{
		//check if feature is valid
			sizeFactor.height = 3;
			sizeFactor.width = 3;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type9");
			m_numAreas = 2;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -2;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = 3*baseDim.height;
			m_areas[0].width = 3*baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_initMean = 0;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob< probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5]+probType[6]+probType[7]+probType[8]+probType[9]) 
		{
			//check if feature is valid
			sizeFactor.height = 3;
			sizeFactor.width = 1;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type10");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -1;
			m_weights[2] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col;
			m_areas[1].upper = position.row+baseDim.height;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row+baseDim.height*2;
			m_areas[2].left = position.col;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 128;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else if (prob < probType[0]+probType[1]+probType[2]+probType[3]+probType[4]+probType[5]+probType[6]+probType[7]+probType[8]+probType[9]+probType[10])
		{
		//check if feature is valid
			sizeFactor.height = 1;
			sizeFactor.width = 3;
			if (position.row + baseDim.height*sizeFactor.height >= patchSize.height ||
				position.col + baseDim.width*sizeFactor.width >= patchSize.width)
				continue;
			area = baseDim.height*sizeFactor.height*baseDim.width*sizeFactor.width;
			if (area < minArea)
				continue;

			strcpy (m_type, "Type11");
			m_numAreas = 3;
			m_weights = new int[m_numAreas];
			m_weights[0] = 1;
			m_weights[1] = -1;
			m_weights[2] = 1;
			m_areas = new Rect[m_numAreas];
			m_areas[0].left = position.col;
			m_areas[0].upper = position.row;
			m_areas[0].height = baseDim.height;
			m_areas[0].width = baseDim.width;
			m_areas[1].left = position.col+baseDim.width;
			m_areas[1].upper = position.row;
			m_areas[1].height = baseDim.height;
			m_areas[1].width = baseDim.width;
			m_areas[2].upper = position.row;
			m_areas[2].left = position.col+2*baseDim.width;
			m_areas[2].height = baseDim.height;
			m_areas[2].width = baseDim.width;
			m_initMean = 128;
			m_initSigma = INITSIGMA( m_numAreas ); 
			valid = true;
		}
		else
			assert (false);	
	}

	m_initSize = patchSize;
	m_curSize = m_initSize;
	m_scaleFactorWidth = m_scaleFactorHeight = 1.0f;
	m_scaleAreas = new Rect[m_numAreas];
	m_scaleWeights = new float[m_numAreas];
	for (int curArea = 0; curArea<m_numAreas; curArea++) {
		m_scaleAreas[curArea] = m_areas[curArea];
		m_scaleWeights[curArea] = (float)m_weights[curArea] /
			(float)(m_areas[curArea].width*m_areas[curArea].height);
	}
}
/*
CvHistogram* image2histogram(IplImage* im,int bins1=3,int bins2=3,int bins3=3,float scalemax1=1.0,float scalemax2=1.0,float scalemax3=1.0)
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
*/
CvHistogram* copyhistogram(CvHistogram*t)
{
	CvHistogram*tt=NULL;
	cvCopyHist(t,&tt);
	return tt;
}
/*
bool FeatureHistogram::eval(ImageRepresentation* image, Rect ROI, float* result) 
{		
	*result = -1.0f;
	if(initflag)//cap nhat lan dau
	{		
		if(!image->getimcolor())
		{
			initflag=false;
			return false;
		}		
		*result=cvCompareHist(ref_histos,image->getimcolor(),CV_COMP_CORREL);
	}
	else //lan dau la cap nhat hsv
	{
		if(!image->getimcolor())
		{
			initflag=false;
			return false;
		}
		initflag=true;
		//khoi tao lai
		*result = 1.0f; //ket qua cao nhat
		ref_histos=copyhistogram(image->getimcolor());
	}
	return true;
}
*/
CvHistogram* copyhistogram3(CvHistogram*t)
{
	CvHistogram*tt=NULL;
	cvCopyHist(t,&tt);
	return tt;
}
bool FeatureHistogram::eval(CvHistogram* hist, Rect ROI, float* result) 
{		
	*result = 0.0f;
	Point2D offset;
	offset = ROI;

	// define the minimum size
	Size minSize = Size(3,3);

	// printf("in eval %d = %d\n",curSize.width,ROI.width );

	if ( m_curSize.width != ROI.width || m_curSize.height != ROI.height )
	{
		m_curSize = ROI;
		if (!(m_initSize==m_curSize))
		{
			m_scaleFactorHeight = (float)m_curSize.height/m_initSize.height;
			m_scaleFactorWidth = (float)m_curSize.width/m_initSize.width;

			for (int curArea = 0; curArea < m_numAreas; curArea++)
			{
				m_scaleAreas[curArea].height = (int)floor((float)m_areas[curArea].height*m_scaleFactorHeight+0.5);
				m_scaleAreas[curArea].width = (int)floor((float)m_areas[curArea].width*m_scaleFactorWidth+0.5);

				if (m_scaleAreas[curArea].height < minSize.height || m_scaleAreas[curArea].width < minSize.width) {
					m_scaleFactorWidth = 0.0f;
					return false;
				}

				m_scaleAreas[curArea].left = (int)floor( (float)m_areas[curArea].left*m_scaleFactorWidth+0.5);
				m_scaleAreas[curArea].upper = (int)floor( (float)m_areas[curArea].upper*m_scaleFactorHeight+0.5);
				m_scaleWeights[curArea] = (float)m_weights[curArea] /
					(float)((m_scaleAreas[curArea].width)*(m_scaleAreas[curArea].height));  
			}
		}
		else
		{
			m_scaleFactorWidth = m_scaleFactorHeight = 1.0f;
			for (int curArea = 0; curArea<m_numAreas; curArea++) {
				m_scaleAreas[curArea] = m_areas[curArea];
				m_scaleWeights[curArea] = (float)m_weights[curArea] /
					(float)((m_areas[curArea].width)*(m_areas[curArea].height));
			}
		}
	}

	if ( m_scaleFactorWidth == 0.0f )
		return false;

	for (int curArea = 0; curArea < m_numAreas; curArea++)
	{
		///////////////
		if(initflag)//cap nhat lan dau
		{		
			if(!hist)
			{
				*result  += -1.0f;
				initflag=false;
				return false;
			}		
			float compute=(cvCompareHist(ref_histos,hist,CV_COMP_CORREL)-0.85)*255;		
				
			//if(compute>maxcompute)maxcompute=compute;
			//if(compute<mincompute)mincompute=compute;
			//printf("\nmax compute:%f,min compute:%f",maxcompute,mincompute);
			float k=(compute)*m_scaleWeights[curArea];
			*result +=k;
			//if(k>maxcomputeresult)maxcomputeresult=k;
			//if(k<mincomputeresult)mincomputeresult=k;					
			//printf("\nmax computerresult:%f,min computerresult:%f",maxcomputeresult,mincomputeresult);
		}
		else //lan dau la cap nhat hsv
		{
			if(!hist)
			{
				*result  += -1.0f;
				return false;
			}
			initflag=true;
			//khoi tao lai
			//*result +=m_scaleWeights[curArea]; //ket qua cao nhat
			ref_histos=copyhistogram3(hist);
			float thresh=((rand() % 10000)*0.2f)/10000;	
			cvThreshHist(ref_histos,thresh);
			float compute=(cvCompareHist(ref_histos,hist,CV_COMP_CORREL)-0.85)*255;
			float k=(compute)*m_scaleWeights[curArea];
			*result +=k;
		}
		//////////////			
		//float k=(float)image->getSum( m_scaleAreas[curArea]+offset )*
		//m_scaleWeights[curArea];
		//*result += k;
		//if(k>maxcomputegetsum)maxcomputegetsum=k;
		//if(k<mincomputegetsum)mincomputegetsum=k;
		//printf("\nmax computerget sum:%f,min computeget sum:%f",maxcomputegetsum,mincomputegetsum);
		//////////////
		//old function
		/*
		if(initflag)//cap nhat lan dau
		{		
			if(!image)
			{
				*result = -1.0f;
				initflag=false;
				return false;
			}		
			*result=cvCompareHist(ref_histos,image,CV_COMP_CORREL)*m_scaleWeights[curArea]*1000;
		}
		else //lan dau la cap nhat hsv
		{
			if(!image)
			{
				*result = -1.0f;
				return false;
			}
			initflag=true;
			//khoi tao lai
			*result = m_scaleWeights[curArea]*1000; //ket qua cao nhat
			ref_histos=copyhistogram(image);
			float thresh=((rand() % 10000)*0.2f)/10000;	
			cvThreshHist(ref_histos,thresh);
		}	
		//	*result += (float)image->getSum( m_scaleAreas[curArea]+offset )*
		//		m_scaleWeights[curArea];
				
		*/
	}
	/*
	if (image->getUseVariance())
	{
		float variance = (float) image->getVariance(ROI);
		*result /=  variance;
	}
	*/
	m_response = *result;

	return true;	
}
void FeatureHistogram::getInitialDistribution(EstimatedGaussDistribution* distribution)
{
	distribution->setValues(m_initMean, m_initSigma);
}