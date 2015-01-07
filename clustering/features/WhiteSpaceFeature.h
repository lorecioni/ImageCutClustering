/*
 * WhiteSpaceFeature.h
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 */

#ifndef CLUSTERING_FEATURES_WHITESPACEFEATURE_H_
#define CLUSTERING_FEATURES_WHITESPACEFEATURE_H_
#include <leptonica/allheaders.h>
#include <string>
#include <iostream>
#include <sstream>
#include "../../utils/UsClusteringUtils.h"

class WhiteSpaceFeature{
public:
	WhiteSpaceFeature();
	virtual ~WhiteSpaceFeature();
	bool static isWhiteSpace(PIX* pix);
};
#endif /* CLUSTERING_FEATURES_WHITESPACEFEATURE_H_ */
