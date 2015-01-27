/*
 * FeaturesMiner.cpp
 *
 *  Created on: 28/gen/2014
 *      Author: alessio
 */

#include "FeaturesMiner.h"
#include <cmath>

FeaturesMiner::FeaturesMiner() {

	this->width = 0;
	this->height = 0;
	this->windowsSize = 10;
	this->projectionHorizontal = NULL;
	this->projectionVertical = NULL;
	this->image = NULL;
}

FeaturesMiner::~FeaturesMiner() {

	pixDestroy(&image);
	delete[] projectionHorizontal;
	delete[] projectionVertical;
}
/**
 *
 */
int FeaturesMiner::findBlackBorder(int orientation) {

	if (image == NULL) {
		return -1;
	}
	int sum;
	unsigned int val;

		if (orientation == HORIZONTAL) {
			for (int i = 0; i < width; i++) {
				val = sum = 0;
				for (int j = 0; j < height; j++) {
					pixGetPixel(image, i, j, &val);
					sum += val;
				}
				if ((sum) >= 200 * height) {
					return i;
				}
			}
		} else if (orientation == VERTICAL) {
			for (int i = 0; i < height; i++) {
				val = sum = 0;
				for (int j = 0; j < width; j++) {
					pixGetPixel(image, j, i, &val);
					sum += val;
				}
				if ((sum) >= 200 * width) {
					return i;
				}
			}
		}
	return -1;
}

int* FeaturesMiner::findColums() {

	if (this->projectionVertical == NULL || this->windowsSize == 0) {

		return NULL;
	}

	unsigned int* window;
	unsigned int oldDifference, difference;
	unsigned int min, max1, max2;
	int firstCol = 0;
	int ala1 = 0;
	int ala2 = 0;
	int column = 0;
	bool minFound;

	/*ONLY FOR DEBUG*/
	int oldColumn = 0;
	/*END OF DEBUG*/

	window = (unsigned int*) malloc(this->windowsSize * sizeof(unsigned int));

	/*inizializzo la finestra che esamina l'immagine*/
	for (int i = 0; i < this->windowsSize; i++) {

		window[i] = this->projectionVertical[i];
	}

	/*inizio il confronto*/
	for (int j = 0; j < this->width; j++) {

		/*cerco massimo pre-minimo, minimo e massimo post-minimo*/
		minFound = false;
		max1 = max2 = 0;
		min = this->height * 255;
		for (int k = 0; k < this->windowsSize; k++) {
			if (max1 == 0 && window[k] > max1 && !minFound) {
				max1 = window[k];
			}
			if (window[k] > max1 && max2 == 0 && !minFound) {
				max1 = window[k];
			}
			if (window[k] < min) {
				min = window[k];
				column = j + k;
				minFound = true;
			}
			if (minFound && window[k] > max2) {
				max2 = window[k];
			}
		}

		difference = (l_uint32) fmax((double) max1, (double) max2) - min;
		if (difference >= /*soglia tra massimo e minimo*/100000
				&& difference > oldDifference
				&& abs(max2 - max1) < 100000 /*soglia tra massimi*/) {

			if (firstCol == 0) {
				firstCol = column;
			}

			/*ONLY FOR DEBUG*/
			if (column != (oldColumn + 1)) {
				//printf("Colonna in: %d Soglia: %d \n", column, difference);
			}
			oldColumn = column;
			/*END OF DEBUG*/

			if ((column - firstCol) >= 1320 && (column - firstCol) <= 1400) {
				ala1 = column;
			}
			if ((column - firstCol) >= 1550 && (column - firstCol) <= 1625) {
				ala2 = column;
			}

		}
		oldDifference = difference;

		/*scorre la finestra*/
		for (int l = 0; l < this->windowsSize - 1; l++) {

			window[l] = window[l + 1];
		}
		window[9] = this->projectionVertical[this->windowsSize - 1 + j];
	}
	/*printf("Prima colonna: %d\n", firstCol);
	 printf("Colonna iniziale: %d \n", ala1);
	 printf("Colonna finale: %d \n", ala2);
	 */
	int* results = (int*) malloc(2 * sizeof(int));
	results[0] = ala1;
	results[1] = ala2;

	return results;
}

/**
 * ampiezza finestra 4
 */
int* FeaturesMiner::findRows(int windowsSizeForRows) {

	if (this->projectionHorizontal == NULL || this->windowsSize == 0) {

		return NULL;
	}

	unsigned int *window;
	unsigned int threshold_1 = 1000;
	unsigned int threshold_2 = 1000;
	int firstRowFound = 0;
	int secondRowFound = 0;
	//TODO definire numero righe come costante
	int *rows = (int*) malloc(51 * sizeof(int));

	window = (unsigned int*) malloc(this->windowsSize * sizeof(unsigned int));

	/*inizializzo la finestra che esamina l'immagine*/
	for (int i = 0; i < this->windowsSize; i++) {
		window[i] = this->projectionHorizontal[i];
	}

	/*inizio il confronto*/
	for (int j = 0; j < this->height - this->windowsSize; j++) {
		//printf("projection[%d]=%d\n", j, projection[j]);
		for (int k = 0; k < this->windowsSize; k++) {

			if (window[k] <= threshold_1 && firstRowFound == 0
					&& (j + k - 1) > 150) {
				firstRowFound = j + k - 1;
				//printf("Prima Riga in %d con %d\n", firstColumnFound,	window[k]);
			}
		}

		/*scorre la finestra*/
		for (int l = 0; l < this->windowsSize - 1; l++) {
			window[l] = window[l + 1];
		}
		window[this->windowsSize - 1] =
				this->projectionHorizontal[this->windowsSize - 1 + j];

	}
	/*best effort search*/
	if (firstRowFound != 0) {
		/*inizializzo la finestra che esamina l'immagine*/
		for (int i = 1; i <= this->windowsSize; i++) {
			window[i - 1] = this->projectionHorizontal[firstRowFound + i];
		}

		/*inizio il confronto*/
		for (int j = firstRowFound; j < this->height - this->windowsSize;
				j++) {

			for (int k = 0; k < this->windowsSize; k++) {

				//TODO costanti
				if (window[k] <= threshold_2 && secondRowFound == 0
						&& (j + k - 1) - firstRowFound > 200
						&& (j + k - 1) - firstRowFound < 240) {
					secondRowFound = j + k - 1;
					//printf("Seconda Riga in %d con %d\n", secondColumnFound,window[k]);
				}
			}

			/*scorre la finestra*/
			for (int l = 0; l < this->windowsSize - 1; l++) {
				window[l] = window[l + 1];
			}
			window[this->windowsSize - 1] =
					this->projectionHorizontal[this->windowsSize - 1 + j];
		}
	}

	if (secondRowFound != 0) {
		int *windows;
		int max;
		int row, oldrow;

		windows = (int*) malloc(windowsSizeForRows * sizeof(int));

		for (int ii = 0; ii < 50; ii++) {
			rows[ii] = 0;
		}
		rows[0] = secondRowFound;
		int count = 1;

		oldrow = secondRowFound;
		/*inizializzo la finestra che esamina l'immagine*/
		for (int i = 0; i < windowsSizeForRows; i++) {
			windows[i - 1] = this->projectionHorizontal[secondRowFound + i];
		}

		/*inizio il confronto*/
		for (int j = secondRowFound; j < this->height - windowsSizeForRows;
				j++) {
			max = 0;
			for (int k = 0; k < windowsSizeForRows; k++) {

				if (windows[k] > max) {
					max = windows[k];
					row = (int) (j + k);
				}

			}

			//TODO costanti
			if (row - oldrow > 35 && row - oldrow < 60
					&& max > 255 * (this->width - 50)) {
				rows[count] = row;
				//	printf("Riga %d in %d min %d\n", count, (int) rows[count], max);
				count++;
				if (count == 51) {
					return rows;
				}
				oldrow = row;
			}

			/*scorre la finestra*/
			for (int l = 0; l < windowsSizeForRows - 1; l++) {
				windows[l] = windows[l + 1];
			}
			windows[windowsSizeForRows - 1] =
					this->projectionHorizontal[windowsSizeForRows - 1 + j];

		}
	}

	return rows;

}

int FeaturesMiner::getHeight() {
	return height;
}

void FeaturesMiner::setHeight(int height) {
	this->height = height;
}

void FeaturesMiner::setImage(PIX* &image) {
	this->image = image;
	pixGetDimensions(image, &width, &height, NULL);
}

unsigned int* FeaturesMiner::getProjectionHorizontal() {
	return projectionHorizontal;
}

void FeaturesMiner::setProjectionHorizontal(
		unsigned int* projectionHorizontal) {
	this->projectionHorizontal = projectionHorizontal;
}

unsigned int* FeaturesMiner::getProjectionVertical() {
	return projectionVertical;
}

void FeaturesMiner::setProjectionVertical(unsigned int* projectionVertical) {
	this->projectionVertical = projectionVertical;
}

int FeaturesMiner::getWidth() {
	return width;
}

void FeaturesMiner::setWidth(int width) {
	this->width = width;
}

void FeaturesMiner::setWindowsSize(int windowsSize) {
	this->windowsSize = windowsSize;
}

