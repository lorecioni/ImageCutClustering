/*
 * FeatureExtractor.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 *
 ***
 *** LOOP: L
 *** Incrocio : X
 *   / : s
 *   \ : S
 *   piccola barretta(es.i) : i
 *   barretta grossa | : l
 *   puntino : .
 *   spazio bianco: " "
 *
 * da considerare anche l'ordine con cui appaiono, da mantenere coerente.
 */

#include "FeatureExtractor.h"
#include "WhiteSpaceFeature.h"
#define WIDTH 8  //todo ??? boh si prova


FeatureExtractor::FeatureExtractor(PIX* pixd) {
	int w,h;

	this->pix = pixd;
}

FeatureExtractor::~FeatureExtractor() {
	pixDestroy(&pix);
}

std::string FeatureExtractor::findFeatures(){
	std::string report;
	std::vector<PIX*> cutUps = cutImage(); //sliding window idea: usare il metodo usato da loro per spezzettare
                                           //fig originale nelle parole applicato alle parole stesse
	 for (std::vector<PIX*>::iterator it = cutUps.begin(); it != cutUps.end(); ++it) {
		report+= searchFeatures(*it);
	}

	return report;
}

//TODO attenzione a dove sono w h x y delle box, Come le avevano trovate in UStat.. le box cambiano da pix a pix?)

std::vector<PIX*> FeatureExtractor::cutImage(){
	std::vector<PIX* > vector;

	int w,h;
	pixGetDimensions(pix, &w, &h, NULL);
	int divNumber = w / WIDTH;
	int pixRemaining = w;
	for(int i=0; i< divNumber; i++){

		PIX* pixN;
		BOX* cropWindow = boxCreate(i*WIDTH, 0, WIDTH, h);  //questo crea una box relativa alle coordinate pix (?)
		pixN = pixClipRectangle(pix, cropWindow, NULL); //con la box mi prendo parte della PIX e ne faccio un altra PIX
		//aggiungi a vettore vector.add?                //TODO 2 tipi di box a volta con width diverse ma centrate?
		pixRemaining = pixRemaining - WIDTH ;
	}

	if(pixRemaining != 0){
		PIX* pixN;
	BOX* cropWindow = boxCreate(w-pixRemaining, 0, pixRemaining, h);
	pixN = pixClipRectangle(pix, cropWindow, NULL);
	}





	return vector;
}


//richiama ogni singola ricerca di Feature partendo dal PIX gia tagliato DOPO aver visto se è vuota o no
std::string FeatureExtractor::searchFeatures(PIX* cut){
	if( WhiteSpaceFeature::isWhiteSpace(cut) ){
		return " ";
	}
	std::string featureString;

	//***
	//***
	//***

	return featureString;
}





