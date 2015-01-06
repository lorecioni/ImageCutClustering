/*
 * DiagonalsAndCrossesFeature.h
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 */

#ifndef CLUSTERING_FEATURES_DIAGONALSANDCROSSESFEATURE_H_
#define CLUSTERING_FEATURES_DIAGONALSANDCROSSESFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>
using namespace std;

class DiagonalsAndCrossesFeature{
public:
	DiagonalsAndCrossesFeature();
	virtual ~DiagonalsAndCrossesFeature();
	bool isUpwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2);
	bool isDownwardDiag(PIX* image,  int* x1,int* x2,int* y1,int* y2);
	bool crossing(int x1,int x2,int y1,int y2, int x11,int x22,int y11,int y22);
	string isCross(PIX* image);
};
#endif /* CLUSTERING_FEATURES_DIAGONALSANDCROSSESFEATURE_H_ */
