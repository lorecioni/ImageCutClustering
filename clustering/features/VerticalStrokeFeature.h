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

using namespace std;
class VerticalStrokeFeature {
public:
	VerticalStrokeFeature();
	virtual ~VerticalStrokeFeature();
	static string isVertical(PIX* image, int offset, int width);

private: int findStroke(PIX* image, int offset, int width);
};

#endif /* CLUSTERING_FEATURES_VERTICALSTROKEFEATURE_H_ */
