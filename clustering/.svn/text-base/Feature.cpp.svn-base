/*
 * Feature.cpp
 *
 *  Created on: 14/mar/2014
 *      Author: alessio
 */

#include "Feature.h"

#define W 100
#define W1 50

/**
 *
 */
Feature::Feature() {
	setTopBlack(0);
	setBottomBlack(0);
	setChanges(0);

}

float Feature::getBottomBlack() const {
	return bottomBlack;
}

void Feature::setBottomBlack(float bottomBlack) {
	this->bottomBlack = bottomBlack;
}

int Feature::getChanges() const {
	return changes;
}

void Feature::setChanges(int changes) {
	this->changes = changes;
}

float Feature::getTopBlack() const {
	return topBlack;
}

void Feature::setTopBlack(float topBlack) {
	this->topBlack = topBlack;
}

bool Feature::isWhite(){
	if(this->bottomBlack==-1 && this->topBlack==-1){
		return false;
	}
	return true;
}


int Feature::cost(){
	return ((int)(this->bottomBlack-this->topBlack)*W)+this->changes*W1;
}


Feature::~Feature() {
	// TODO Auto-generated destructor stub
}

