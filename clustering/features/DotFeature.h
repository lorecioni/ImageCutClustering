/*
 * DotFeature.h
 *
 *  Created on: 16/gen/2015
 *      Author: lorenzocioni
 */

#ifndef CLUSTERING_FEATURES_DOTFEATURE_H_
#define CLUSTERING_FEATURES_DOTFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>
#include<vector>
using namespace std;

class DotFeature {
	public:
		DotFeature();
		virtual ~DotFeature();
		static string isDot(PIX* image, int offset, int width);

	private:
		static bool checkDot(PIX* image, int i, int j, int offset, int width);
};



#endif /* CLUSTERING_FEATURES_DOTFEATURE_H_ */
