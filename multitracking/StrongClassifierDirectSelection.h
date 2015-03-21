#pragma once

#include "StrongClassifier.h"

class StrongClassifierDirectSelection : public StrongClassifier
{
public:

	StrongClassifierDirectSelection(int numBaseClassifier, int numWeakClassifier, Size patchSize, bool useFeatureExchange = false, int iterationInit = 0); 
	StrongClassifierDirectSelection(int numBaseClassifier, int numWeakClassifier, Size patchSize, const int features, bool useFeatureExchange = false, int iterationInit = 0); 

	virtual ~StrongClassifierDirectSelection();
	
	bool update(CvHistogram*hist,ImageRepresentation *image, Rect ROI, int target, float importance = 1.0); 
	bool update(ImageRepresentation *image, Rect ROI, int target, float importance = 1.0); 
	bool update(CvHistogram*image, Rect ROI, int target, float importance = 1.0f); 
private:

	bool * m_errorMask;
	float* m_errors;
	float* m_sumErrors;
};
