/*
 * DotFeature.cpp
 *
 *  Created on: 16/gen/2015
 *      Author: lorenzocioni
 */

#include "DotFeature.h"
#include "../../utils/UsClusteringUtils.h"

#include <iostream>

#define BLACK_THRES 60
#define RADIUS_MIN 6
#define RADIUS_MAX 10
using namespace std;

DotFeature::DotFeature(){
}

string DotFeature::isDot(PIX* img, int offset, int width){
	int imgWidth, imgHeight;
	pixGetDimensions(img, &imgWidth, &imgHeight, NULL);

	//Binarizza l'immagine
	img = pixConvertTo1(img, 150);

	//Calcola il box relativo alla finestra corrente
	BOX* window = boxCreate(offset, 0, width, imgHeight);

	//Cerca tutte le componenti connesse nell'immagine...
	BOXA* bb = pixConnCompBB(img, 8);
	//...e prende solo quelle che intersecano la finestra
	BOXA* cc = boxaIntersectsBox(bb, window);

	for(int i = 0; i < cc->n; i++){
		if(RADIUS_MIN <= cc->box[i]->w
				&& cc->box[i]->w <= RADIUS_MAX
				&& RADIUS_MIN <= cc->box[i]->h
				&& cc->box[i]->h <= RADIUS_MAX){
			//Se la dimensione della BOX che circonda la CC è compatibile con un punto
			return ".....";
		}
	}
	return "";
}

DotFeature::~DotFeature(){}


