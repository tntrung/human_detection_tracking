#pragma once
#include "EstimatedGaussDistribution.h"
#include "ImageRepresentation.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define NH 3
#define NS 3
#define NV 3
#define H_MAX 360.0
#define S_MAX 1.0
#define V_MAX 1.0
#define S_THRESH 0.1
#define V_THRESH 0.2
#define LAMBDA 20
typedef struct histogram {
	float histo[NH*NS + NV];   /**< histogram array */
	int n;                     /**< length of histogram array */
} histogram;
class FeatureHistogram
{
	public:

	FeatureHistogram(Size patchSize);
	virtual ~FeatureHistogram();

	void getInitialDistribution(EstimatedGaussDistribution *distribution);

	//bool eval(ImageRepresentation* image, Rect ROI, float* result); 
	bool eval(CvHistogram* image, Rect ROI, float* result); 
	
	float getResponse(){return m_response;};

	int getNumAreas(){return m_numAreas;};
	int* getWeights(){return m_weights;};
	Rect* getAreas(){return m_areas;};
	CvHistogram*getimcolor(){return ref_histos;};
	void setimacolor(CvHistogram*_imcolor){cvReleaseHist(&ref_histos);cvCopyHist(_imcolor,&ref_histos);};	
	
private:

	char m_type[20];
	int m_numAreas;
	int* m_weights;
	float m_initMean;
	float m_initSigma;

	void generateRandomFeature(Size imageSize);
	Rect* m_areas;     // areas within the patch over which to compute the feature
	Size m_initSize;   // size of the patch used during training
	Size m_curSize;    // size of the patches currently under investigation
	float m_scaleFactorHeight;  // scaling factor in vertical direction
	float m_scaleFactorWidth;   // scaling factor in horizontal direction
	Rect* m_scaleAreas;// areas after scaling
	float* m_scaleWeights; // weights after scaling
	float m_response;

	//su dung histogram
	//histogram** ref_histos;
	CvHistogram*ref_histos;
	
	//khai bao bien cho histogram
	//IplImage*hsv;	
	//Luu patchSize lai
	//Rect patsize;
	//flag khoi tao
	bool initflag;	
};

