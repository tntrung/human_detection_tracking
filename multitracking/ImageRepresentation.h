#pragma once

//#include "OS_specific.h"
#include <memory.h>
#include <math.h>
#include "Regions.h"
#include <vector>

#include <string>

#include <map>

#include <cstdio>
//#include "stdint.h"
//typedef __int32  __uint32;

//typedef __int64  __uint64;
class ImageRepresentation  
{
public:
	ImageRepresentation(){};
	ImageRepresentation(unsigned char* image,int typeft, Size imagSize);
	ImageRepresentation(unsigned char* image, Size imagSize);
	ImageRepresentation(int typeft);
	//ImageRepresentation(IplImage* image,int typeft, Size imagSize);
	ImageRepresentation(unsigned char* image,int typeft, Size imagSize, Rect imageROI);
    void defaultInit(unsigned char* image, Size imageSize);
	virtual ~ImageRepresentation();

	int getSum(Rect imageROI);
	float getMean(Rect imagROI);
	int getValue(Point2D position);
	Size getImageSize(void){return m_imageSize;};
	Rect getImageROI(void){return m_ROI;};
	void setNewImage(unsigned char* image);
	void setNewROI(Rect ROI);
	void setNewImageSize( Rect ROI );
	void setNewImageAndROI(unsigned char* image, Rect ROI);
	float getVariance(Rect imageROI);
	long getSqSum(Rect imageROI);
	bool getUseVariance(){return m_useVariance;};
	void setUseVariance(bool useVariance){ this->m_useVariance = useVariance; };
	int gettypefeature(){return typefeature;};
	/*
	CvHistogram*getimcolor()
	{
		return imcolor;
	}
	void setimacolor(CvHistogram*_imcolor)
	{
		//if(imcolor)
			//cvReleaseHist(&imcolor);
		CvHistogram*tt=NULL;
		cvCopyHist(_imcolor,&tt);
		imcolor=tt;
	}
	*/
private:
	//them vao
	//loai image integralImage(=0) va image histogram(=1)
	int typefeature; 
	bool m_useVariance;
	Size m_imageSize;
	Rect m_ROI;
	Point2D m_offset;
	void createIntegralsOfROI(unsigned char* image);	
	//typefeature == 0	
    __int32* intImage;
    __int64* intSqImage;	
	//typefeature == 1	
	//khai bao bien cho histogram
	//CvHistogram*imcolor;	
};
