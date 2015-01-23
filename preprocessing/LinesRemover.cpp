/*
 * LinesRemover.cpp
 *
 *  Created on: 29/gen/2014
 *      Author: alessio
 */

#include "LinesRemover.h"

#include <leptonica/environ.h>
#include <leptonica/pix.h>
//#include <stddef.h>
#include <cmath>
#include <cstdio>
#include <vector>

#include "../utils/UsClusteringUtils.h"

using namespace std;

LinesRemover::LinesRemover(PIX* image) {

	this->image = image;
	pixGetDimensions(this->image, &this->width, &this->height, &this->depth);
}

vector<int> LinesRemover::findBlackRows(PIX* img) {
	int lineProjection;
	l_uint32 actualValue;
	vector<int> row;
	int soglia;
	soglia = ((this->width) * 0.20 * 255);
	for (int i = 0; i < this->height; i++) {
		/*ottengo la proiezione di ogni linea*/
		lineProjection = 0;
		for (int j = 0; j < this->width; j++) {
			pixGetPixel(img, j, i, &actualValue);
			lineProjection += actualValue;
		}

		/*se la linea trovata è una riga nera allora la inserisco tra le righe*/
		if (lineProjection < soglia
				&& (i > (0.85 * this->height) || (i < 0.2 * this->height))) {
			if (row.size() == 0 && i >= 2) {
				row.push_back(i);
				//printf("\nHo trovato una riga in %d dell'immagine ritagliata con valore %d",i,lineProjection);
			}
			if (row.size() != 0 && i > row[row.size() - 1] + 8) {
				row.push_back(i);
				//printf("\nHo trovato una riga in %d dell'immagine ritagliata con valore %d",i,lineProjection);
			}
		}
	}
	return row;

}

int LinesRemover::findThickness(PIX* img, int initialPosition) {
	l_uint32 lineProjection = 0;
	l_uint32 actualValue = 0;
	l_uint32 soglia = ((this->width) * 0.8 * 255);
	for (int i = initialPosition; i < this->height; i++) {
		/*ottengo la proiezione di ogni linea*/
		lineProjection = 0;
		for (int j = 0; j < this->width; j++) {
			pixGetPixel(img, j, i, &actualValue);
			lineProjection += actualValue;
		}

		if (lineProjection > soglia) {
			//printf("%d == Linea %d di spessore %d\n", img, initialPosition,
			//		i - initialPosition);
			return i - initialPosition;
		}
	}
	return (this->height - initialPosition);
}

void LinesRemover::deleteBlackColumns(PIX* img) {
	l_uint32 actualValue;
	int leftProjection[30];
	int rightProjection[30];
	int minValue;
	int sogliaPerMin, sogliaPerCol;
	int leftStart, rightStart;
	int rightValue, leftValue;
	minValue = 255 * this->height;
	sogliaPerMin = this->height * 0.1 * 255;
	sogliaPerCol = this->height * 0.3 * 255;
	leftStart = 0;
	rightStart = this->width;

	for (int i = 0; i < 30; i++) {
		leftProjection[i] = 0;
		for (int j = 0; j < this->height; j++) {
			pixGetPixel(img, i, j, &actualValue);
			leftProjection[i] += actualValue;
		}
		if (leftProjection[i] < minValue) {

			leftValue = leftProjection[i];
			minValue=leftProjection[i];
		}
	}

	minValue = 255 * this->height;

	for (int i = this->width - 30; i < this->width; i++) {
		rightProjection[i-this->width+30] = 0;
		for (int j = 0; j < this->height; j++) {
			pixGetPixel(img, i, j, &actualValue);
			rightProjection[i-this->width+30] += actualValue;
		}
		if (rightProjection[i-this->width+30] < minValue) {
			rightValue = rightProjection[i-this->width+30];
			minValue = rightProjection[i-this->width+30];
		}
	}

	if (leftValue < sogliaPerMin) {
		/*c'è una colonna da eliminare*/
		for (int i = 29; i >= 0; i--) {
			if (leftProjection[i] < sogliaPerCol) {
				leftStart = i;
				break;
			}
		}
		//printf("\nLeftStart = %d\n", leftStart);
		for (int i = 0; i < leftStart; i++) {
			for (int j = 0; j < this->height; j++) {
				pixSetPixel(this->image, i, j, 255);
			}
		}
	}

	if (rightValue < sogliaPerMin) {
		/*eliminare colonna*/
		for (int i = 0; i <= 29; i++) {
			if (rightProjection[i] < sogliaPerCol) {
				rightStart = (this->width) - 30 + i;
				break;
			}
		}
		//printf("\nRightStart = %d\n", rightStart);
		for (int i = rightStart; i < this->width; i++) {
			for (int j = 0; j < this->height; j++) {
				pixSetPixel(this->image, i, j, 255);
			}

		}
	}
}

void LinesRemover::deleteBlackRows(std::vector<int> row, PIX* img) {
	int k;
	int* window;
	l_uint32 actualValue;
	int thickness;
	int white = 0;
	int black = 0;
	/*per ogni riga da cancellare*/
	for (unsigned int i = 0; i < row.size(); i++) {

		thickness = findThickness(img, row[i]);
		//thickness+=3;
		window = (int*) malloc((thickness + 4) * sizeof(int));
		for (int j = 0; j < this->width; j++) {

			for (k = 0; k < thickness + 4; k++) {
				window[k] = 0;
				if (row[i] + k < this->height) {
					pixGetPixel(img, j, row[i] + k - 2, &actualValue);
					window[k] = actualValue;
				}
			}

			int incrementCount = 0;
			bool cancelled = false;
			while (!cancelled && incrementCount < 30) {
				incrementCount++;

				if ((window[0] == 255 && window[thickness / 2] == 0)
						|| row[i] < (0.2 * this->height)) {
					black = 0;
					cancelled = true;

				} else if (window[0] == 0) {

					pixGetPixel(img, j, row[i] - incrementCount - 3,
							&actualValue);
					if (actualValue == 255) {

						for (k = 0; k < thickness + 4; k++) {
							window[k] = 0;
							if (row[i] + k < this->height) {
								pixGetPixel(img, j, row[i] + k - incrementCount,
										&actualValue);
								window[k] = actualValue;
							}
						}
					} else {
						for (k = 0; k < thickness + 4; k++) {
							if (window[k] == 255) {
								white = k;
								break;
							}
						}

						for (k = white; k < thickness + 4; k++) {
							if (window[k] == 0) {
								black = k;
							}
						}
						cancelled = true;
					}
				}

			}

			//}

			//if (black != 0) {
			for (k = black; k < thickness + 4; k++) {

				if (k + row[i] - 2 < this->height && window[k] == 0) {
					pixSetPixel(this->image, j, k + row[i] - 2, 255);

					//DEBUG: cancella sulla binarizzata
					//pixSetPixel(img,j,k+row[i]-2,255);
				}
			}
			//}
		}
	}

}

PIX* LinesRemover::removeLines() {

	PIX* imgToOut;
	vector<int> row;

	/*binarizzo per cercare meglio le linee*/
	imgToOut = pix8Binarize(this->image, 150);
	row = findBlackRows(imgToOut);
	deleteBlackColumns(imgToOut);
	deleteBlackRows(row, imgToOut);

	if(imgToOut != NULL){
		pixFreeData(imgToOut);
	}

	return this->image;

	//DEBUG: salva la binarizzata
	//return imgToOut;

}

double LinesRemover::degToRad(double value) {
	return (M_PI / 180) * value;
}

LinesRemover::~LinesRemover() {
	pixDestroy(&image);
}
