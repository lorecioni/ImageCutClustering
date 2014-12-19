/*
 * LinesRemover.h
 *
 *  Created on: 29/gen/2014
 *      Author: alessio
 */

#ifndef LINESREMOVER_H_
#define LINESREMOVER_H_
#define HORIZONTAL 1

#include <leptonica/allheaders.h>
#include <vector>

class LinesRemover {
private:
	PIX* image;
	l_int32 width;
	l_int32 height;
	l_int32 depth;
	std::vector<int> findBlackRows(PIX* img);
	int findThickness(PIX* img, int initialPosition);
	void deleteBlackRows(std::vector<int> row, PIX* img);
	void deleteBlackColumns(PIX* img);

	double degToRad(double value);

public:
	LinesRemover(PIX* image);
	PIX* removeLines();
	virtual ~LinesRemover();
};

#endif /* LINESREMOVER_H_ */
