/*
 * FeatureExtractor.h
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#ifndef CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_
#define CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_

#include<leptonica/allheaders.h>
#include <leptonica/pix.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include "../../preprocessing/StateImage.h"

class FeatureExtractor {
public:
	static int counterForName;
	FeatureExtractor();
	virtual ~FeatureExtractor();
	static void extractFeatures(StateImage* State);
	static std::string searchFeatures(PIX* cut, int offset, int width);
	static std::string findFeatures(PIX* img, string* singleResults);
	static std::vector<PIX*> cutImage(PIX* pix);
};

#endif /* CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_ */
