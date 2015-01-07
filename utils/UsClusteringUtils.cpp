/*
 * UsClusteringUtils.cpp
 *
 *  Created on: 28/gen/2014
 *      Author: alessio
 */

#include "UsClusteringUtils.h"

#include <leptonica/allheaders.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

/**
 *
 */
char * substring(char *string, int position, int length) {
	char *pointer;
	int c;

	pointer = (char*) malloc((length + 1) * sizeof(char));

	if (pointer == NULL) {
		printf("Unable to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	for (c = 0; c < position - 1; c++)
		string++;

	for (c = 0; c < length; c++) {
		*(pointer + c) = *string;
		string++;
	}

	*(pointer + c) = '\0';

	return pointer;
}


/**
 *
 */
l_int32 countFiles(char* path) {
	l_int32 numFiles = 0;
	DIR*dir;
	if ((dir = opendir(path)) != NULL) {
		char* fileType;
		fileType = (char*) malloc(3 * sizeof(char*));
		/* print all the files and directories within directory */
		dirent* ent;
		while ((ent = readdir(dir)) != NULL) {

			fileType[0] = ent->d_name[strlen(ent->d_name) - 3];
			fileType[1] = ent->d_name[strlen(ent->d_name) - 2];
			fileType[2] = ent->d_name[strlen(ent->d_name) - 1];
			if (ent->d_type == DT_REG && strcmp(fileType, "jpg") == 0) {
				numFiles++;
			}
		}
		return numFiles;
	}
	return -1;
}

/**
 *
 */
PIX * pix8Binarize(PIX* pix, l_uint32 threshold) {
	PIX* pixd = NULL;
	pixd = pixCreate(pix->w, pix->h, pix->d);
	l_int32 w, h, d;

	pixGetDimensions(pixd, &w, &h, &d);

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			l_uint32 val;
			pixGetPixel(pix, i, j, &val);
			if (val > threshold) {
				pixSetPixel(pixd, i, j, 255);
			} else {
				pixSetPixel(pixd, i, j, 0);
			}

		}
	}
	return pixd;

}
/**
 *
 */
void reset(char** myPointer) {
	if (myPointer) {
		free(*myPointer);
		*myPointer = NULL;
	}
}
/**
 *
 */
void reset(int** myPointer) {
	if (myPointer) {
		free(*myPointer);
		*myPointer = NULL;

	}
}

/**
 *
 */


/**
 *
 */
string getState(string line) {

	vector<string> splittedLine = splitStringBy(line, "|");
	string toReturn="";
	if(splittedLine.size()>22){
		toReturn=splittedLine[22];
	}
	else{
		toReturn="no_label";
	}
	if(toReturn==""){
		toReturn="no_label";
	}
	return toReturn;
}

/**
 *
 */
vector<string> splitStringBy(string stringToSplit, string regex) {

	string s = stringToSplit;
	string delimiter = regex;
	vector<std::string> splitted;
	size_t pos = 0;
	string token;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		splitted.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	splitted.push_back(s);
	return splitted;
}

string getGeneratedOutputFileName(string path) {
	string scopy=path;

	unsigned int found = scopy.find_last_of("/");
	//int bound=scopy.find_last_of(".");
	scopy=scopy.substr(found+1);
	scopy=scopy.substr(0,scopy.length()-4);
	return scopy;
}

//Restituisce il valore medio dei pixel nella singola immagine
int getAverageValue(PIX* img, int offset, int width){
	int w, h, d;
	pixGetDimensions(img, &w, &h, &d);
	unsigned int val;
	unsigned int count = 0;
	unsigned int num = 0;
	for(int i = offset; i < (offset + width); i++){
		for(int j = 0; j < h; j++){
			pixGetPixel(img, i, j, &val);
			count += val;
			num++;
		}
	}
	return count/num;
}
