/*
 * StateImage.h
 *
 *  Created on: 28/mar/2014
 *      Author: alessio
 */

#ifndef STATEIMAGE_H_
#define STATEIMAGE_H_

#include "../clustering/Feature.h"
#include <leptonica/allheaders.h>
#include "../utils/UsClusteringUtils.h"
#include <vector>
#include <string>

#define WHITE 255
#define BLACK 0

class StateImage {

private:
	string label;
	string sourceFile;
	int index;
	PIX* image;
	int cluster;
	std::vector<Feature*> features;


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
	std::vector<Feature*> getFeatures();
	void parseContentsFile();
	int getCluster() const;
	void setCluster(int cluster);

	void setFeatures(std::vector<Feature*>& features);
};

#endif /* STATEIMAGE_H_ */
