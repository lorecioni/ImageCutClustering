/*
 * StateImage.h
 *
 *  Created on: 28/mar/2014
 *      Author: alessio
 */

#ifndef STATEIMAGE_H_
#define STATEIMAGE_H_

#include <leptonica/allheaders.h>
#include "../utils/UsClusteringUtils.h"
#include <vector>
#include <string>

#include "../clustering/features/DimensionFeatures.h"

#define WHITE 255
#define BLACK 0

class StateImage {

private:
	string label;
	string sourceFile;
	int index;
	PIX* image;
	int cluster;
	std::vector<DimensionFeatures*> features;
	//Stringa di comparazione
	string structure;


public:
	StateImage(string* sourceFile,int index, PIX*image);
	virtual ~StateImage();
	void calculateFeatures();
	PIX* getImage();
	void setImage(PIX*& image);
	string getLabel();
	int getIndex();
	string getSourceFile();
	void setLabel(string label);
	std::vector<DimensionFeatures*> getFeatures();
	void parseContentsFile(int labelPosition);
	int getCluster() const;
	void setCluster(int cluster);

	void setFeatures(std::vector<DimensionFeatures*>& features);

	void setStructure(string structure);
	string getStructure();
};

#endif /* STATEIMAGE_H_ */
