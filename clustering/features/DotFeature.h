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


class DotFeature {
	public:
		DotFeature();
		virtual ~DotFeature();
		static std::string isDot(PIX* image, int offset, int width);
};



#endif /* CLUSTERING_FEATURES_DOTFEATURE_H_ */
