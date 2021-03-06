/*
 * Clusterizer.cpp
 *
 *  Created on: 24/feb/2014
 *      Author: alessio
 */

#include "Clusterizer.h"

#include <leptonica/environ.h>
#include <leptonica/imageio.h>
#include <leptonica/pix.h>
#include <stddef.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <utility>

#include "../utils/UsClusteringUtils.h"
#include "affinitypropagation/ap.cpp"
#include "affinitypropagation/AffinityPropagationValue.h"
#include "lcs/LCSLength.h"

#define WHITE 255
#define BLACK 0

#define LABEL_POSITION 9

Clusterizer::Clusterizer(std::vector<StateImage*> vectorOfStates, bool LCS, bool L1, timeval begin) {
	this->vectorOfStates = vectorOfStates;
	this->LCS = LCS;
	this->L1 = L1;
	this->begin = begin;
	this->correctClusters = 0;
	this->correctElements = 0;
}

void Clusterizer::clusterize() {
	vector<AffinityPropagationValue> values;
	int maxChanges;
	extractDimensionFeatures();
	maxChanges = getMaxChanges();

	for (unsigned int i = 0; i < this->vectorOfStates.size(); i++) {
			(this->vectorOfStates[i])->parseContentsFile(LABEL_POSITION);
	}

	//Stampa il tempo di esecuzione una volta estratte le feature
	double featureExtractionTime = evaluateTime(this->begin);
	printf("\nFeatures estratte in %.3f s\n", featureExtractionTime);

	//Popola il vettore di AffinityPropagationValues che verrà passato al metodo di AffinityPropagation
	for (unsigned int i = 0; i < this->vectorOfStates.size(); i++) {
		for (unsigned int j = i + 1; j < this->vectorOfStates.size(); j++) {
			float totalDistance;
			int distanceLCS = 0;
			float distanceL1 = 0;
			//Per ogni coppia (i, j) calcola la distanza LCS tra le due stringhe di struttura estratte
			if(LCS){
				distanceLCS = LCSDistance(this->vectorOfStates[i]->getStructure(), this->vectorOfStates[j]->getStructure());
			}
			if(L1){
				distanceL1 = L1_distance(this->vectorOfStates[i]->getFeatures(), this->vectorOfStates[j]->getFeatures(), maxChanges);
			}
			//La distance L1 è pesata alla metà
			totalDistance = (float) distanceLCS + distanceL1 * 1/2;
			values.push_back(*(new AffinityPropagationValue(i, j, -totalDistance)));
		}
	}

	//Stampa il tempo di esecuzione una volta estratte le feature
	double evaluatingDistancesTime = evaluateTime(this->begin) - featureExtractionTime;
	printf("Distanze calcolate in %.3f s\n", evaluatingDistancesTime);

	string mainfolder = "./Clusters/";
	mkdir(mainfolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

	ofstream f("./Clusters/result.txt"); //se il file non esiste lo crea, altrimenti lo sovrascrive!
	if (!f) {
		cout << "Errore nella creazione del file!";
		return;
	}

	printf("\n----- Risultati -----\n\n");
	printf("Numero di ritagli: %d\n", this->vectorOfStates.size());

	f << "Resoconto risultati" << endl;
	f << "Numero ritagli: " << this->vectorOfStates.size() << endl;

	//Invoca affinity propagation sul vettore di similarità
	vector<int> examplar = affinityPropagation(this->vectorOfStates.size(),
			values, 1, 0.9, 2000, 100);

	vector<int> centersOfCluster = getExamplars(examplar);

	for (unsigned int i = 0; i < centersOfCluster.size(); i++) {

		stringstream ss;
		ss << centersOfCluster[i];
		mainfolder = "./Clusters/" + ss.str() + "/";
		mkdir(mainfolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}

	for (size_t i = 0; i < examplar.size(); ++i) {
		stringstream ss;
		stringstream ss1;
		stringstream ss2;
		this->vectorOfStates[i]->setCluster(examplar[i]);

		ss << examplar[i];
		string path = "./Clusters/" + ss.str() + "/";
		string scopy = getGeneratedOutputFileName(
				vectorOfStates[i]->getSourceFile());

		ss2 << this->vectorOfStates[i]->getIndex() << "_"
				<< this->vectorOfStates[i]->getLabel();
		string name = scopy + "_" + ss2.str() + ".jpg";
		string filepath = path + name;

		//cout << filepath << endl;
		pixWrite(filepath.c_str(), this->vectorOfStates[i]->getImage(),
				IFF_JFIF_JPEG);

	}
	vector<StateImage *> auxStateImage;

	vector<int> clusterHead;
	for (int i = 0; i < examplar.size(); i++) {
		bool found = false;
		for (int j = 0; j < clusterHead.size(); j++) {
			if (examplar[i] == clusterHead[j]) {
				found = true;
			}
		}
		if (!found) {
			clusterHead.push_back(examplar[i]);
		}
	}

	sort(clusterHead.begin(), clusterHead.end());

	f << "Numero clusters: " << clusterHead.size() << endl;
	cout << "Numero clusters: " << clusterHead.size() << endl;
	f << "Media elementi per cluster: " << (float) this->vectorOfStates.size() / clusterHead.size() << endl;
	cout << "Media elementi per cluster: " << (float) this->vectorOfStates.size() / clusterHead.size() << endl;
	f.close();

	printf("\n-- Clusters--\n\n");
	for (int i = 0; i < clusterHead.size(); i++) {

		auxStateImage.clear();

		for (int j = 0; j < this->vectorOfStates.size(); j++) {

			if (this->vectorOfStates[j]->getCluster() == clusterHead[i]) {
				auxStateImage.push_back(this->vectorOfStates[j]);
			}
		}

		// abbiamo il vettore ausiliario su cui calcolare le robe che ci interessano.

		calculateOccurencies(clusterHead[i], auxStateImage);

	}

	ofstream nf("./Clusters/result.txt", ios::app); //apre il file in modalità append, lasciando intatto quello che c'è e scrivendo alla fine
	if (!nf) {
		cout << "Errore nell'apertura del file!";
		return;
	}

	cout << "\nNumero cluster corretti: " << this->correctClusters << endl;
	nf << "\nNumero cluster corretti:: " << this->correctClusters << endl;
	cout << "Elementi in cluster corretti: " << this->correctElements << endl;
	nf << "Elementi in cluster corretti: " << this->correctElements << endl;

	float prec = calculatePrecision();
	float precOld = calculatePrecisionOld();
	cout << "La precisione del clustering è: " << prec << endl;
	cout << "La precisione media del clustering è " << precOld << endl;
	nf << "La precisione del clustering è: " << prec << " %" << endl;
	nf << "La precisione media del clustering è: " << precOld << " %" << endl;
	nf.close();

}

/**
 *
 */
void Clusterizer::extractDimensionFeatures() {

	int width = 0;
	bool firstBlackPixel = false;
	bool lastBlackPixel = false;
	int height = 0;
	int countChanges = 0;
	l_uint32 backVal = 0;

	adaptStateImagesWidth();

	for (unsigned int i = 0; i < this->vectorOfStates.size(); i++) {
		std::vector<DimensionFeatures*> imageFeatures;
		PIX* auxPix = pix8Binarize((this->vectorOfStates[i])->getImage(), 150);
		pixGetDimensions(auxPix, &width, &height, NULL);
		for (int x = 0; x < width; x++) {

			DimensionFeatures* feature = new DimensionFeatures();

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

			feature->setChanges(countChanges);
			imageFeatures.push_back(feature);
			countChanges = 0;
			lastBlackPixel = false;
			firstBlackPixel = false;

		}

		if(auxPix != NULL){
			pixFreeData(auxPix);
		}


		this->vectorOfStates[i]->setFeatures(imageFeatures); //features.push_back(imageFeatures);

	}

}

int Clusterizer::getMaxChanges() {
	int max = 0;
	for (unsigned int i = 0; i < vectorOfStates.size(); i++) {
		vector<DimensionFeatures*> copy = this->vectorOfStates[i]->getFeatures();
		for (int j = 0; j < copy.size(); j++) {
			if (copy[j]->getChanges() > max) {
				max = copy[j]->getChanges();
			}
		}
	}
	return max;
}

/**
 *
 */
float Clusterizer::L1_distance(std::vector<DimensionFeatures*> a, std::vector<DimensionFeatures*> b,
		int maxChanges) {

	float distanceVal = 0;
	for (unsigned int y = 0; y < a.size(); y++) {
		distanceVal += distance(*(a[y]), *(b[y]), maxChanges);
	}
	return distanceVal;
}

/**
 *
 */
float Clusterizer::distance(DimensionFeatures a, DimensionFeatures b, int maxChanges) {
//TODO è indifferente al numero di cambiamenti, da sistemarxemplar15e la presenza di feature -1
	//return abs(a.coofs st() - b.cost());

	float diffA = a.getBottomBlack() - a.getTopBlack();

	float diffB = b.getBottomBlack() - b.getTopBlack();

	float value = pow(diffA - diffB, 2)
			+ pow(
					(float) (a.getChanges()) / maxChanges
							- (float) (b.getChanges()) / maxChanges, 2);

	return sqrt(value);

}

/**
 * return the minimum of a,b,c
 */
float Clusterizer::minimum(float a, float b, float c) {
	float minimum, temp;
	temp = std::min(a, b);
	minimum = std::min(temp, c);
	return minimum;

}

int Clusterizer::factorize(int number) {
	std::vector<int> returnVect;
	for (int i = 10; i < 50; i++) {
		if (number % i == 0) {
			return (i);
		}
	}
	return 1;
}

/**
 *
 */
vector<int> Clusterizer::getExamplars(vector<int> examplars) {
	vector<int> result;
	sort(examplars.begin(), examplars.end());
	result.push_back(examplars[0]);
	for (unsigned int i = 1; i < examplars.size(); i++) {

		if (examplars[i] != examplars[i - 1]) {
			result.push_back(examplars[i]);
		}

	}
	return result;
}

void Clusterizer::adaptStateImagesWidth() {
	std::ofstream out;
	int maxWidth = 0;
	int currentWidth = 0;
	int height, depth;
	/*Determino la massima altezza delle immagini ritagliate*/
	for (unsigned int i = 0; i < this->vectorOfStates.size(); i++) {
		pixGetDimensions(this->vectorOfStates[i]->getImage(), &currentWidth,
				&height, NULL);
		if (maxWidth < currentWidth) {
			maxWidth = currentWidth;
		}
	}

	/*Riempio di bianofs co le zone destre e sinistre delle immagini*/
	for (unsigned int i = 0; i < this->vectorOfStates.size(); i++) {
		pixGetDimensions(this->vectorOfStates[i]->getImage(), &currentWidth,
				&height, &depth);
		if (currentWidth < maxWidth) {
			int difference = maxWidth - currentWidth;
			int leftDifference, rightDifference;
			/*calcolo la dimensione delle bande bianche da aggiungere a destra e sinistra*/
			leftDifference = rightDifference = difference / 2;
			if ((difference % 2) > 0) {
				rightDifference++;
			}
			PIX* newImage = pixCreate(maxWidth, height, depth);

			/*riempio la banda sinistra*/
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < leftDifference; x++) {
					pixSetPixel(newImage, x, y, 255);
				}
			}

			/*copio l'immagine*/
			for (int y = 0; y < height; y++) {
				for (int x = leftDifference; x < leftDifference + currentWidth;
						x++) {
					l_uint32 value = 0;
					pixGetPixel(this->vectorOfStates[i]->getImage(),
							x - leftDifference, y, &value);
					pixSetPixel(newImage, x, y, value);
				}
			}

			/*riempio la banda destra*/
			for (int y = 0; y < height; y++) {
				for (int x = currentWidth + leftDifference; x < maxWidth; x++) {
					pixSetPixel(newImage, x, y, 255);
				}
			}

			/*sostituisco l'immagine*/
			this->vectorOfStates[i]->setImage(newImage);

		}
	}

}

void Clusterizer::calculateOccurencies(int exemplar,
		std::vector<StateImage*> auxStateImage) {

	std::unordered_map<string, int> m;
	vector<float> singlePrecisions;

	for (int i = 0; i < auxStateImage.size(); i++) {
		if (m.count(auxStateImage[i]->getLabel()) == 0) {
			m[auxStateImage[i]->getLabel()] = 1;
		} else {
			m[auxStateImage[i]->getLabel()]++;
		}
	}

	std::cout << "- Cluster numero " << exemplar << " ";
	ofstream f("./Clusters/result.txt", ios::app); //apre il file in modalità append, lasciando intatto quello che c'è e scrivendo alla fine
	if (!f) {
		cout << "Errore nell'apertura del file!";
		return;
	}
	f << "- Cluster numero " << exemplar << endl;
	f << "Elementi nel cluster:" << auxStateImage.size() << endl
			<< endl;
	cout << "(" << auxStateImage.size() << " elementi):" << endl;

	for (auto it = m.begin(); it != m.end(); ++it) {
		string State = it->first;
		int count = it->second;
		f << "\t" << State << "\t" << " # " << count << " ("
				<< ((float) count / (float) auxStateImage.size()) * 100 << "%)"
				<< endl;
		std::cout << " " << State << ": " << count << endl;
		singlePrecisions.push_back(
				(float) count / (float) auxStateImage.size());
	}
	f << endl;
	f.close();

	sort(singlePrecisions.begin(), singlePrecisions.end());
	this->precisionOld.push_back(singlePrecisions[singlePrecisions.size() - 1]);
	this->precisions.push_back(singlePrecisions[singlePrecisions.size() - 1] * auxStateImage.size());

	//Se il cluster ha precisione 100% aumente il numero cluster corretti
	if((int)singlePrecisions[singlePrecisions.size() - 1] == 1){
		this->correctClusters++;
		this->correctElements += auxStateImage.size();
	}
}
/**
 * percentage old
 */
float Clusterizer::calculatePrecisionOld() {

	float sumprecision = 0;

	for (int i = 0; i < this->precisionOld.size(); i++) {
		sumprecision += this->precisionOld[i];
	}

	return (sumprecision / this->precisionOld.size()) * 100;
}

/**
 * percentage new
 */
float Clusterizer::calculatePrecision() {

	float sumprecision = 0;

	for (int i = 0; i < this->precisions.size(); i++) {
		sumprecision += this->precisions[i];
	}

	return (sumprecision) * 100 / this->vectorOfStates.size();

}

Clusterizer::~Clusterizer() {
// TODO Auto-generated destructor stub
}

