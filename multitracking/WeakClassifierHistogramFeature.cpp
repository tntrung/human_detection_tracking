#include "StdAfx.h"
#include "WeakClassifierHistogramFeature.h"

WeakClassifierHistogramFeature::~WeakClassifierHistogramFeature(void)
{
	delete m_classifier;
	delete m_feature;
}
WeakClassifierHistogramFeature::WeakClassifierHistogramFeature(Size patchSize)
{
	//m_feature = new FeatureHaar(patchSize);
	m_feature = new FeatureHistogram(patchSize);
	generateRandomClassifier();
	m_feature->getInitialDistribution((EstimatedGaussDistribution*) m_classifier->getDistribution(-1));
	m_feature->getInitialDistribution((EstimatedGaussDistribution*) m_classifier->getDistribution(1));
}

void WeakClassifierHistogramFeature::resetPosDist()
{
	m_feature->getInitialDistribution((EstimatedGaussDistribution*) m_classifier->getDistribution(1));
	m_feature->getInitialDistribution((EstimatedGaussDistribution*) m_classifier->getDistribution(-1));
}


void WeakClassifierHistogramFeature::generateRandomClassifier()
{
	m_classifier = new ClassifierThreshold();
}
/*
bool WeakClassifierHistogramFeature::update(ImageRepresentation *image, Rect ROI, int target) 
{
	float value;
	
	bool valid = m_feature->eval (image, ROI, &value); 
	if (!valid)
		return true;

	m_classifier->update(value, target);
	return (m_classifier->eval(value) != target);
}
*/
bool WeakClassifierHistogramFeature::update(CvHistogram *image, Rect ROI, int target) 
{
	float value;
	
	bool valid = m_feature->eval (image, ROI, &value); 
	if (!valid)
		return true;

	m_classifier->update(value, target);
	return (m_classifier->eval(value) != target);
}
/*
int WeakClassifierHistogramFeature::eval(ImageRepresentation *image, Rect ROI) 
{
	float value;
	bool valid = m_feature->eval(image, ROI, &value); 
	if (!valid)
		return 0;

	return m_classifier->eval(value);
}
*/
int WeakClassifierHistogramFeature::eval(CvHistogram *image, Rect ROI) 
{
	float value;
	bool valid = m_feature->eval(image, ROI, &value); 
	if (!valid)
		return 0;

	return m_classifier->eval(value);
}
/*
float WeakClassifierHistogramFeature::getValue(ImageRepresentation *image, Rect ROI)
{
	float value;
	bool valid = m_feature->eval (image, ROI, &value);
	if (!valid)
		return 0;

	return value;
}
*/
EstimatedGaussDistribution* WeakClassifierHistogramFeature::getPosDistribution()
{
  return static_cast<EstimatedGaussDistribution*>(m_classifier->getDistribution(1));
}


EstimatedGaussDistribution* WeakClassifierHistogramFeature::getNegDistribution()
{
  return static_cast<EstimatedGaussDistribution*>(m_classifier->getDistribution(-1));
}
