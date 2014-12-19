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
#include "Feature.h"


/**
 *
 */
class Clusterizer {
private:
	vector<float> precisions;
	std::vector<StateImage*> vectorOfStates;
	/*Per ogni immagine ho un vettore di float*/
	std::vector<std::vector<Feature*>> features;
	void preprocessing();
	int getMaxChanges();
	float L1_distance(std::vector<Feature*> a,std::vector<Feature*> b, int maxChanges);
	int factorize(int number);
	float minimum(float a,float b,float c);
	vector<int> getExamplars(vector<int> examplars);
	float distance(Feature a,Feature b, int maxChanges);
	void adaptStateImagesWidth();
	void calculateOccurencies(int exemplar,std::vector<StateImage*> auxStateImage);
	float calculatePrecision();

public:
	Clusterizer(std::vector<StateImage*> vectorOfPixs);
	void clusterize();
	virtual ~Clusterizer();
};


#endif /* CLUSTERIZER_H_ */
