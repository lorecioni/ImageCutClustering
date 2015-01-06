/*
 * FeatureExtractor.h
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#ifndef CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_
#define CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_

#include<leptonica/allheaders.h>
#include <string>
#include <vector>
class FeatureExtractor {
public:
	FeatureExtractor(PIX* pixd);
	virtual ~FeatureExtractor();
	std::string findFeatures();


	PIX* pix;
	static std::string searchFeatures(PIX* cut);


	static std::vector<PIX*> cutImage(PIX* pix);
};


#endif /* CLUSTERING_FEATURES_FEATUREEXTRACTOR_H_ */
