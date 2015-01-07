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

class WhiteSpaceFeature{
public:
	WhiteSpaceFeature();
	virtual ~WhiteSpaceFeature();
	std::string static isWhiteSpace(PIX* pix);
};
#endif /* CLUSTERING_FEATURES_WHITESPACEFEATURE_H_ */
