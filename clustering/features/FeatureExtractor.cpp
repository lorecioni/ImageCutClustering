/*
 * FeatureExtractor.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 *
 *
 *   LOOP: 'LL'
 *   IncrocioAlto : 'XXXX'
 *   Incrocio : 'xxxx'
 *   / bassa : 's'
 *   / alta : 'S'
 *   \ bassa : 'u'
 *   \ alta : 'U'
 *   piccola barretta vertic.(es.i) : 'ii'   verticale in 2nda metà: prefisso V
 *   barretta grossa vertic. | : 'IIii'
 *   piccola barr orizzont : '-'            orizzontale alte: prefisso H
 *   barretta grossa orizzont : '_-'
 *   puntino : '...'
 *   spazio bianco: ' '
 *
 * da considerare anche l'ordine con cui appaiono, da mantenere coerente.
 */

#include "FeatureExtractor.h"

#include <leptonica/imageio.h>
#include <sys/types.h>

#include "DiagonalsAndCrossesFeature.h"
#include "DotFeature.h"
#include "HorizontalStrokeFeature.h"
#include "LoopFeature.h"
#include "TrashFeature.h"
#include "VerticalStrokeFeature.h"
#include "WhiteSpaceFeature.h"

#define BOX_WIDTH 36
#define JUMP 16

FeatureExtractor::FeatureExtractor() {}

int FeatureExtractor::counterForName;

void FeatureExtractor::extractFeatures(StateImage* imageState){
	//Metodo per l'estrazione delle nuove features

	//TODO:toglimi x avere nome test file
	FeatureExtractor::counterForName +=1;

	PIX* testImage =  imageState->getImage();
	if(testImage != NULL){

	string singleResults = "";
	string structure = "";

	//Controlla se il segmento contiene solo "spazzatura"
	if(!TrashFeature::isTrash(testImage)){
		structure = FeatureExtractor::findFeatures(testImage, &singleResults);
	} else {
		structure = TrashFeature::getTrashStructure();
	}

	imageState->setStructure(structure);

	//Crea la cartella per il test delle festure
	//TODO da rimuovere dopo i test, non è necessario salvare i singoli pezzi
	stringstream strs;
	strs << FeatureExtractor::counterForName;
	string mainfolder = "./Test/" + strs.str() +"/";
	mkdir(mainfolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

	string path = "./Test/" + strs.str() +"/";

	std::vector<PIX*> vector;
	/*vector =  FeatureExtractor::cutImage(testImage);
	int parts =  vector.size();

	for(int j=0; j< parts ; j++){

		stringstream ss;
		ss << j;

		//crea nuovo file immagine da PIX
		string name = ss.str() + ".jpg";
		string filepath = path + name;

		pixWrite(filepath.c_str(), vector[j],
				IFF_JFIF_JPEG);

	}

	//Scrive il report della stringa
	ofstream f(path + "comparatore.txt"); //se il file non esiste lo crea, altrimenti appende
	if (!f) {
		cout << "Errore nella creazione/apertura del file!";
	}
	f << "Stringa generata [" << strs.str() << "]: _" << structure << "_" << endl;
	f << singleResults;
	f.close();*/
	}

}

std::string FeatureExtractor::findFeatures(PIX* img, string* singleResults){
	std::string report;
	int w,h;
	pixGetDimensions(img, &w, &h, NULL);
	int count = 0;

	//si salta l'ultima perchè quasi certamente vuota o di poco valore
	for (int i = 1; i < (w - 2*BOX_WIDTH +1); i+=JUMP) {
		std::string l = searchFeatures(img, i, BOX_WIDTH);
		report += l;
		stringstream ss;
		ss << count;
		*singleResults += "Stringa " + ss.str() + ": " + l + "\n";
		count++;
	}
	return report;
}


std::vector<PIX*> FeatureExtractor::cutImage( PIX* pix){
	std::vector<PIX* > vector;
	int w,h;
	pixGetDimensions(pix, &w, &h, NULL);
	int i=0;
	for(int curWid=0; curWid< w - BOX_WIDTH-1; curWid+=JUMP){
		PIX* pixN;
		BOX* cropWindow = boxCreate(i*JUMP, 0, BOX_WIDTH, h);  //questo crea una box relativa alle coordinate pix (?)
		pixN = pixClipRectangle(pix, cropWindow, NULL); //con la box mi prendo parte della PIX e ne faccio un altra PIX
		boxDestroy(&cropWindow);
		vector.push_back(pixN);
		i++;
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
	} else{
		firstHalf += DiagonalsAndCrossesFeature::isCross(cut, offset, width);
		firstHalf += VerticalStrokeFeature::isVertical(cut, offset, width);
	}

	if(WhiteSpaceFeature::isWhiteSpace(cut, offset+width, width)){
		secondHalf = " ";
	} else{
		secondHalf += DiagonalsAndCrossesFeature::isCross(cut, offset+width, width);
		secondHalf += VerticalStrokeFeature::isVertical(cut, offset+width, width);
	}

	featureString = firstHalf + secondHalf;
	featureString += HorizontalStrokeFeature::isHorizontal(cut,offset,double_width);
	featureString += DotFeature::isDot(cut, offset, double_width);
	featureString += LoopFeature::isLoop(cut, offset, double_width);
	return featureString;
}


FeatureExtractor::~FeatureExtractor() {
}




