/*
 * Projection.cpp
 *
 *  Created on: 27/gen/2014
 *      Author: alessio
 */

#include "Projector.h"
/**
 *
 */
Projector::Projector(PIX* myImage, bool save) {

	setImage(myImage);
	this->isSave = save;
}
/**
 *
 */
void Projector::setImage(PIX* myImage) {

	this->imgToProject = myImage;
	pixGetDimensions(imgToProject, &width, &height, &density);
}

/**
 *
 */
void Projector::setSaveProjection(bool save){

	this->isSave=save;
}

/**
 *
 */
unsigned int* Projector::horizontalProjection() {
	unsigned int* values;
	values = (unsigned int*) malloc(height * sizeof(unsigned int));
	for (int i = 0; i < height; i++) {
		unsigned int val = 0;
		values[i] = 0;
		for (int j = 0; j < width; j++) {
			pixGetPixel(imgToProject, j, i, &val);
			values[i] = values[i] + val;
		}
	}
	return (values);

}
/**
 *
 */
unsigned int* Projector::verticalProjection() {

	unsigned int *values;

	values = (unsigned int*) malloc(width * sizeof(unsigned int));
	for (int i = 0; i < width; i++) {
		unsigned int val = 0;
		values[i] = 0;
		for (int j = 0; j < height; j++) {
			pixGetPixel(imgToProject, i, j, &val);
			values[i] = values[i] + val;
		}
	}
	return (values);
}

/**
 *
 */
Projector::~Projector() {

	pixDestroy(&this->imgToProject);
}

