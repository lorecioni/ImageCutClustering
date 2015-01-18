/*
 * HorizontalStrokeFeature.h
 *
 *  Created on: 17/gen/2015
 *      Author: francesco
 */

#ifndef CLUSTERING_FEATURES_HORIZONTALSTROKEFEATURE_H_
#define CLUSTERING_FEATURES_HORIZONTALSTROKEFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>


class HorizontalStrokeFeature{
public:
	HorizontalStrokeFeature();
	virtual ~HorizontalStrokeFeature();
	static std::string isHorizontal(PIX* image, int offset, int width);
};

#endif /* CLUSTERING_FEATURES_HORIZONTALSTROKEFEATURE_H_ */
