#pragma once

#include "ImageRepresentation.h"

class WeakClassifier  
{

public:

	WeakClassifier(){};
	//virtual ~WeakClassifier();
	virtual ~WeakClassifier(){};

	//virtual bool update(ImageRepresentation* image, Rect ROI, int target);
	virtual bool update(ImageRepresentation* image, Rect ROI, int target) 
	{
		return true;
	}
	virtual bool update(CvHistogram*hist, ImageRepresentation* image, Rect ROI, int target) 
	{
		return true;
	}
	virtual bool update(CvHistogram* image, Rect ROI, int target) 
	{
		return true;
	}
	virtual int eval(CvHistogram*hist,ImageRepresentation* image, Rect  ROI) 
	{
		return 0;
	}
	//virtual int eval(ImageRepresentation* image, Rect ROI);
	virtual int eval(ImageRepresentation* image, Rect  ROI) 
	{
		return 0;
	}
	virtual int eval(CvHistogram* image, Rect  ROI) 
	{
		return 0;
	}
	virtual float getValue (ImageRepresentation* image, Rect  ROI){return 0;};

	virtual int getType(){return 0;};

};