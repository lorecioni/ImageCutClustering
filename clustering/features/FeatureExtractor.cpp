/*
 * FeatureExtractor.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 *
 ***
 *** LOOP: L
 *** IncrocioAlto : X
 *   Incrocio : x
 *   / bassa : s
 *   / alta : S
 *   \ bassa : u
 *   \ alta : U
 *   piccola barretta vertic.(es.i) : i
 *   barretta grossa vertic. | : I
 *   puntino : .
 *   spazio bianco: " "
 *
 * da considerare anche l'ordine con cui appaiono, da mantenere coerente.
 */

#include "FeatureExtractor.h"

#include <leptonica/pix.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "DiagonalsAndCrossesFeature.h"
#include "WhiteSpaceFeature.h"
#include "LoopFeature.h"
#include "DotFeature.h"


#define BOX_WIDTH 32


FeatureExtractor::FeatureExtractor() {
}

FeatureExtractor::~FeatureExtractor() {
}

std::string FeatureExtractor::findFeatures(PIX* img){
	std::string report;
	int w,h;
	pixGetDimensions(img, &w, &h, NULL);
	ofstream f("./Comparatore.txt", ios::app); //se il file non esiste lo crea, altrimenti appende
	if (!f) {
		cout << "Errore nella creazione/apertura del file!";
	}

	unsigned int count = 0;

	//si salta l'ultima perchè quasi certamente vuota o di poco valore
	for (int i = 0; i < (w - BOX_WIDTH -1); i+=BOX_WIDTH) {
		std::string l = searchFeatures(img, i, BOX_WIDTH);
		report+= l;
		stringstream ss;
		ss << count;
		f << "stringa_" +ss.str() +":_" << l << "_" << endl;
		count++;
	}

	f.close();
	return report;
}


std::vector<PIX*> FeatureExtractor::cutImage( PIX* pix){
	//PIX* pix = pix8Binarize(pixA, 150);
	std::vector<PIX* > vector;

	int w,h;
	pixGetDimensions(pix, &w, &h, NULL);
	int divNumber = w / BOX_WIDTH;
	int pixRemaining = w;
	for(int i=0; i< divNumber; i++){

		PIX* pixN;
		BOX* cropWindow = boxCreate(i*BOX_WIDTH, 0, BOX_WIDTH, h);  //questo crea una box relativa alle coordinate pix (?)
		pixN = pixClipRectangle(pix, cropWindow, NULL); //con la box mi prendo parte della PIX e ne faccio un altra PIX
		vector.push_back(pixN);              //TODO 2 tipi di box a volta con width diverse ma centrate?
		pixRemaining = pixRemaining - BOX_WIDTH ;
	}

	if(pixRemaining != 0){
		PIX* pixN;
	BOX* cropWindow = boxCreate(w-pixRemaining, 0, pixRemaining, h);
	pixN = pixClipRectangle(pix, cropWindow, NULL);
	vector.push_back(pixN);
	}

	return vector;
}


//richiama ogni singola ricerca di Feature partendo dal PIX gia tagliato DOPO aver visto se è vuota o no
std::string FeatureExtractor::searchFeatures(PIX* cut, int offset, int double_width){
	std::string featureString;
	std::string firstHalf;
	std::string secondHalf;
	int width = double_width / 2;

	if(WhiteSpaceFeature::isWhiteSpace(cut, offset, width)){
		firstHalf = " ";
	}else{
		firstHalf += DiagonalsAndCrossesFeature::isCross(cut, offset, width);
		firstHalf += VerticalStrokeFeature::isVertical(cut, offset, width);
		firstHalf += LoopFeature::isLoop(cut, offset, width);
		firstHalf += DotFeature::isDot(cut, offset, width);
	}



	if(WhiteSpaceFeature::isWhiteSpace(cut, offset+width, width)){
		secondHalf = " ";
	}else{
		secondHalf += DiagonalsAndCrossesFeature::isCross(cut, offset+width, width);
		secondHalf += VerticalStrokeFeature::isVertical(cut, offset+width, width);
		secondHalf += LoopFeature::isLoop(cut, offset+width, width);
		secondHalf += DotFeature::isDot(cut, offset+width, width);
	}
	featureString = firstHalf + secondHalf;

	return featureString;
}
//// Spostare le cose che vanno meglio cercate sugli interi 32 pixel dopo




