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
#include<vector>
using namespace std;

class DiagonalsAndCrossesFeature{
public:
	DiagonalsAndCrossesFeature();
	virtual ~DiagonalsAndCrossesFeature();
	static string isCross(PIX* image);
private:
	static bool isUpwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2,bool over);
	static bool isDownwardDiag(PIX* image,  int* x1,int* x2,int* y1,int* y2, bool over);
	static bool crossing(int x1,int x2,int y1,int y2, int x11,int x22,int y11,int y22);

};
#endif /* CLUSTERING_FEATURES_DIAGONALSANDCROSSESFEATURE_H_ */
