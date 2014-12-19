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
	FILE *fd;
	if (isSave) {

		/* apre il file in scrittura */
		fd = fopen("outputhorizontal.txt", "w");
		if (fd == NULL) {
			perror("Errore in apertura del file");
			exit(1);
		}
	}
	if (isSave) {
		fprintf(fd, "%d\n", width);
		fprintf(fd, "%d\n", height);
	}
	values = (unsigned int*) malloc(height * sizeof(unsigned int));
	for (int i = 0; i < height; i++) {
		unsigned int val = 0;
		values[i] = 0;
		for (int j = 0; j < width; j++) {
			pixGetPixel(imgToProject, j, i, &val);
			values[i] = values[i] + val;
		}
		//printf("%d ==> %d\n", i, values[i]);
		if (isSave) {
			fprintf(fd, "%d\n", values[i]);
		}
	}

	/* scrive il numero */

	/* chiude il file */
	if (isSave) {
		fclose(fd);
	}
	return (values);

}
/**
 *
 */
unsigned int* Projector::verticalProjection() {

	unsigned int *values;
	FILE *fd;

	/* apre il file in scrittura */
	if (isSave) {
		fd = fopen("outputvertical.txt", "w");
		if (fd == NULL) {
			perror("Errore in apertura del file");
			exit(1);
		}

	}
	//printf("%s %d x %d\n", "img dim: ", w, h);
	if (isSave) {
		fprintf(fd, "%d\n", width);
		fprintf(fd, "%d\n", height);
	}
	values = (unsigned int*) malloc(width * sizeof(unsigned int));
	for (int i = 0; i < width; i++) {
		unsigned int val = 0;
		values[i] = 0;
		for (int j = 0; j < height; j++) {
			pixGetPixel(imgToProject, i, j, &val);
			values[i] = values[i] + val;
		}
		if (isSave) {
			fprintf(fd, "%d\n", values[i]);
		}
	}

	/* scrive il numero */
	/* chiude il file */
	if (isSave) {
		fclose(fd);
	}
	return (values);
}

/**
 *
 */
Projector::~Projector() {

	pixDestroy(&this->imgToProject);
}

