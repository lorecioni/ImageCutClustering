/*
 * Clusterizer.h
 *
 *  Created on: 24/feb/2014
 *      Author: alessio
 */

#ifndef CLUSTERIZER_H_
#define CLUSTERIZER_H_

//#include <leptonica/allheaders.h>
#include <leptonica/allheaders.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../preprocessing/StateImage.h"
#include "features/DimensionFeatures.h"


/**
 *
 */
class Clusterizer {
private:
	vector<float> precisions;
	vector<float> precisionOld;
	bool LCS;
	bool L1;
	std::vector<StateImage*> vectorOfStates;
	/*Per ogni immagine ho un vettore di float*/
	std::vector<std::vector<DimensionFeatures*> > features;
	void extractDimensionFeatures();
	int getMaxChanges();
	float L1_distance(std::vector<DimensionFeatures*> a,std::vector<DimensionFeatures*> b, int maxChanges);
	int factorize(int number);
	float minimum(float a,float b,float c);
	vector<int> getExamplars(vector<int> examplars);
	float distance(DimensionFeatures a,DimensionFeatures b, int maxChanges);
	void adaptStateImagesWidth();
	void calculateOccurencies(int exemplar,std::vector<StateImage*> auxStateImage);
	float calculatePrecision();
	float calculatePrecisionOld();

public:
	Clusterizer(std::vector<StateImage*> vectorOfPixs, bool LCS, bool L1);
	void clusterize();
	virtual ~Clusterizer();
};


#endif /* CLUSTERIZER_H_ */
