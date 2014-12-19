/*
 * StateImage.cpp
 *
 *  Created on: 28/mar/2014
 *      Author: alessio
 */

#include "StateImage.h"

#include <iostream>
#include <fstream>

StateImage::StateImage(string* sourceFile, int index, PIX* image) {
	this->image = image;
	this->label = *(new string("no_label"));
	this->index = index;
	this->sourceFile = *sourceFile;
}

void StateImage::setFeatures(std::vector<Feature*>& features) {
		this->features = features;
	}

void StateImage::calculateFeatures() {

	int width = 0;
	bool firstBlackPixel = false;
	bool lastBlackPixel = false;
	int height = 0;
	int countChanges = 0;
	l_uint32 backVal = 0;

	PIX* auxPix = pix8Binarize(this->image, 150);
	pixGetDimensions(auxPix, &width, &height, NULL);
	for (int x = 0; x < width; x++) {

		Feature* feature = new Feature();

		l_uint32 val = 0;
		for (int y = 0; y < height; y++) {
			pixGetPixel(auxPix, x, y, &val);
			if (y == 0 && val == BLACK) {
				backVal = BLACK;
			} else if (y == 0 && val == WHITE) {
				backVal = WHITE;
			}

			if (val != WHITE && !firstBlackPixel) {

				feature->setTopBlack((float) y / (float) height);
				firstBlackPixel = true;

			}
			if (val != backVal) {
				countChanges++;
			}

			backVal = val;
		}

		for (int y = height - 1; y > 0; y--) {
			pixGetPixel(auxPix, x, y, &val);

			if (val != WHITE && !lastBlackPixel) {
				feature->setBottomBlack((float) y / (float) height);
				break;
			}

		}

		pixFreeData(auxPix);
		feature->setChanges(countChanges);
		this->features.push_back(feature);
		countChanges = 0;
	}

}

void StateImage::parseContentsFile() {
	ifstream infile;
	string inputFilePath = this->getSourceFile();
	inputFilePath=inputFilePath.substr(0, inputFilePath.size() - 3);
	inputFilePath += "txt";
	int row = -1;
	infile.open(inputFilePath.c_str());
	//bool firstLine = true;
	while (!infile.eof()) {
		string riga;
		getline(infile, riga);
		if (row == this->getIndex()) {
			//cout<<riga;
			this->setLabel(getState(riga));
			riga="";
			return;
		}
		row++;

	}
	infile.close();
}

string StateImage::getLabel() {
	return this->label;
}

void StateImage::setLabel(string label) {
	this->label = label;
}

std::vector<Feature*> StateImage::getFeatures() {
	return this->features;
}

PIX* StateImage::getImage() {
	return this->image;
}

void StateImage::setImage(PIX*& image) {
		this->image = image;
	}

int StateImage::getIndex() {
	return this->index;
}

string StateImage::getSourceFile() {
	return this->sourceFile;
}

StateImage::~StateImage() {

}

int StateImage::getCluster() const {
	return cluster;
}

void StateImage::setCluster(int cluster) {
	this->cluster = cluster;
}
