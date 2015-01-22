/*
 * TrashFeature.cpp
 *
 *  Created on: 16/gen/2015
 *      Author: lorenzocioni
 */

#include "TrashFeature.h"

#include <leptonica/pix.h>
#include <stdlib.h>

#include "WhiteSpaceFeature.h"

#define THRESHOLD 75000
#define STROKE_WIDTH 15
#define TRASH_STRUCTURE "133713371337"
using namespace std;

TrashFeature::TrashFeature(){}

bool TrashFeature::isTrash(PIX* image){
	int width, height;
	pixGetDimensions(image, &width, &height, NULL);

	//Controlla se il segmento è interamente bianco
	if(WhiteSpaceFeature::isWhiteSpace(image, 0, width)){
		return true;
	}

	//Creo la proiezione orizzontale del segmento
	unsigned int* values;
	unsigned int count = 0;
	values = (unsigned int*) malloc(height * sizeof(unsigned int));
	for (int j = 0; j < height; j++) {
		unsigned int val = 0;
		values[j] = 0;
		for (int i = 0; i < width; i++) {
			pixGetPixel(image, i, j, &val);
			values[j] = values[j] + val;
		}
		if(values[j] < THRESHOLD){
			count++;
		}
	}

	if(count < STROKE_WIDTH){
		//Il segmento contiene solo una riga (anche tratteggiata)
		return true;
	}

	return false;
}

string TrashFeature::getTrashStructure(){
	return TRASH_STRUCTURE;
}

TrashFeature::~TrashFeature(){}


