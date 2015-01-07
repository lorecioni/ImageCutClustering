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
#include <iterator>


//#include "../../utils/UsClusteringUtils.h"   se si binarizza viene uno schifo atroce //TODO toglimi
#include "DiagonalsAndCrossesFeature.h"
#include "WhiteSpaceFeature.h"


#define WIDTH 32  //todo ??? boh si prova


FeatureExtractor::FeatureExtractor(PIX* pixd) {
	int w,h;

	this->pix = pixd;
}

FeatureExtractor::~FeatureExtractor() {
	pixDestroy(&pix);
}

std::string FeatureExtractor::findFeatures(){
	std::string report;
	std::vector<PIX*> cutUps = cutImage(NULL); //sliding window idea: usare il metodo usato da loro per spezzettare
                                           //fig originale nelle parole applicato alle parole stesse
	 for (std::vector<PIX*>::iterator it = cutUps.begin(); it != cutUps.end(); ++it) {
		report+= searchFeatures(*it);
	}

	return report;
}

//TODO attenzione a dove sono x y w h delle box, Come le avevano trovate in UStat.. le box cambiano da pix a pix?)

std::vector<PIX*> FeatureExtractor::cutImage( PIX* pix){

	//PIX* pix = pix8Binarize(pixA, 150);  //TODO controllare se con binarizzazione è meglio o peggio
	std::vector<PIX* > vector;

//	if(pix == NULL)pix = this->pix;

	int w,h;
	pixGetDimensions(pix, &w, &h, NULL);
	int divNumber = w / WIDTH;
	int pixRemaining = w;
	for(int i=0; i< divNumber; i++){

		PIX* pixN;
		BOX* cropWindow = boxCreate(i*WIDTH, 0, WIDTH, h);  //questo crea una box relativa alle coordinate pix (?)
		pixN = pixClipRectangle(pix, cropWindow, NULL); //con la box mi prendo parte della PIX e ne faccio un altra PIX
		vector.push_back(pixN);              //TODO 2 tipi di box a volta con width diverse ma centrate?
		pixRemaining = pixRemaining - WIDTH ;
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
std::string FeatureExtractor::searchFeatures(PIX* cut){
	if(WhiteSpaceFeature::isWhiteSpace(cut)){
		return " ";
	}
	std::string featureString;
	featureString += DiagonalsAndCrossesFeature::isCross(cut);


	return featureString;
}





