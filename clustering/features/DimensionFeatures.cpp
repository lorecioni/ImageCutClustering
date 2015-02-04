/*
 * Feature.cpp
 *
 *  Created on: 14/mar/2014
 *      Author: alessio
 */

#include "DimensionFeatures.h"

#define W 100
#define W1 50

/**
 *
 */
DimensionFeatures::DimensionFeatures() {
	setTopBlack(0);
	setBottomBlack(0);
	setChanges(0);

}

float DimensionFeatures::getBottomBlack() const {
	return bottomBlack;
}

void DimensionFeatures::setBottomBlack(float bottomBlack) {
	this->bottomBlack = bottomBlack;
}

int DimensionFeatures::getChanges() const {
	return changes;
}

void DimensionFeatures::setChanges(int changes) {
	this->changes = changes;
}

float DimensionFeatures::getTopBlack() const {
	return topBlack;
}

void DimensionFeatures::setTopBlack(float topBlack) {
	this->topBlack = topBlack;
}

bool DimensionFeatures::isWhite(){
	if(this->bottomBlack==-1 && this->topBlack==-1){
		return false;
	}
	return true;
}


int DimensionFeatures::cost(){
	return ((int)(this->bottomBlack-this->topBlack)*W)+this->changes*W1;
}


DimensionFeatures::~DimensionFeatures() {
	// TODO Auto-generated destructor stub
}

