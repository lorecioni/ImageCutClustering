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
	this->structure = "";
}

void StateImage::setFeatures(std::vector<DimensionFeatures*>& features) {
		this->features = features;
	}

void StateImage::calculateFeatures() {

}

void StateImage::parseContentsFile(int labelPosition) {
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
			this->setLabel(getState(riga, labelPosition));
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

std::vector<DimensionFeatures*> StateImage::getFeatures() {
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

string StateImage::getStructure() {
	return this->structure;
}

void StateImage::setStructure(string structure) {
	this->structure = structure;
}
