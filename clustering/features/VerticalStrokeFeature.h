/*
 * VerticalStrokeFeature.h
 *
 *  Created on: 16/gen/2015
 *      Author: francesco
 */

#ifndef CLUSTERING_FEATURES_VERTICALSTROKEFEATURE_H_
#define CLUSTERING_FEATURES_VERTICALSTROKEFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>

class VerticalStrokeFeature{
public:
	VerticalStrokeFeature();
	virtual ~VerticalStrokeFeature();
	static std::string isVertical(PIX* image, int offset, int width);

};

#endif /* CLUSTERING_FEATURES_VERTICALSTROKEFEATURE_H_ */
