/*
 * LoopFeature.h
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#ifndef CLUSTERING_FEATURES_LOOPFEATURE_H_
#define CLUSTERING_FEATURES_LOOPFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>
#include<vector>
using namespace std;

class LoopFeature{
public:
	LoopFeature();
	virtual ~LoopFeature();
	static string isLoop(PIX* image, int offset, int width);

private:
	static bool checkLoop(PIX* image, int i, int j, int offset, int width);
};



#endif /* CLUSTERING_FEATURES_LOOPFEATURE_H_ */
