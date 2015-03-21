#pragma once
#include "WeakClassifier.h"
//#include "FeatureHaar.h"
#include "FeatureHistogram.h"
#include "ClassifierThreshold.h"
#include "EstimatedGaussDistribution.h"

class WeakClassifierHistogramFeature: public WeakClassifier
{
	public:

	WeakClassifierHistogramFeature(Size patchSize);
	virtual ~WeakClassifierHistogramFeature();

	//bool update(ImageRepresentation* image, Rect ROI, int target); 
	bool update(CvHistogram* image, Rect ROI, int target); 

	//int eval(ImageRepresentation* image, Rect ROI); 
	int eval(CvHistogram* image, Rect ROI); 
	
	//float getValue(ImageRepresentation* image, Rect ROI);
	
	int getType(){return 1;};

	EstimatedGaussDistribution* getPosDistribution();
	EstimatedGaussDistribution* getNegDistribution();

	void resetPosDist();
	void initPosDist();

private:

	//FeatureHaar* m_feature;
	FeatureHistogram* m_feature;
	ClassifierThreshold* m_classifier;

	void generateRandomClassifier();
};

