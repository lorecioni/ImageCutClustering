/*
 * FeaturesMiner.h
 *
 *  Created on: 28/gen/2014
 *      Author: alessio
 */
#include<leptonica/allheaders.h>
#ifndef FEATURESMINER_H_
#define FEATURESMINER_H_
#define HORIZONTAL 0
#define VERTICAL 1

class FeaturesMiner {
private:
	int windowsSize;
	int width;
	int height;
	PIX* image;
	unsigned int *projectionHorizontal;
	unsigned int *projectionVertical;
public:
	FeaturesMiner();
	int getHeight();
	void setHeight(int height);
	void setImage(PIX*& image);
	unsigned int* getProjectionHorizontal();
	void setProjectionHorizontal(unsigned int* projectionHorizontal);
	unsigned int* getProjectionVertical();
	void setProjectionVertical(unsigned int* projectionVertical);
	int getWidth();
	void setWidth(int width);
	void setWindowsSize(int windowsSize);

	int findBlackBorder(int orientation);
	int* findColums();
	int* findRows(int windowsSizeForRows);


	virtual ~FeaturesMiner();


};

#endif /* FEATURESMINER_H_ */
