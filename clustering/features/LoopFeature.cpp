/*
 * LoopFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#include "LoopFeature.h"

#include <leptonica/pix.h>
#include <iostream>

#define BLACK_THRES 30
#define H_STEP 8 //Impostato a 8 per i test da risultati verosimili
#define V_STEP 8
using namespace std;

LoopFeature::LoopFeature(){
}

string LoopFeature::isLoop(PIX* image, int offset, int width){
	int imgWidth, imgHeight;
	pixGetDimensions(image, &imgWidth, &imgHeight, NULL);
	int j;
	unsigned int val = 0;
	//Scorre l'immagine orizzontalmente di H_STEP (non considera loop troppo piccoli)
	for(int i = (offset + H_STEP); i < (offset + width) && i < imgWidth; i++){
		j = 0;
		while(j < imgHeight){
			pixGetPixel(image, i, j, &val);
			if(val < BLACK_THRES){
				//Ho trovato un primo pixel nero, controllo se è il contorno di un loop
				if(checkLoop(image, i, j + V_STEP, offset, width)){
					return "L";
				}
			}
			j++;
		}
	}

	return "";
}

bool LoopFeature::checkLoop(PIX* image, int i, int j, int offset, int width){
	int w, h;
	pixGetDimensions(image, &w, &h, NULL);
	int start_i = i;
	int start_j = j;
	bool isLoop = false;
	unsigned int val, transitions = 0;
	while(j < h){
		pixGetPixel(image, i, j, &val);
		if(val > BLACK_THRES){
			//Sono passato da nero a bianco, aumento le transizioni
			transitions++;
			if(transitions == 2){
				//Ho trovato due transizioni, da nero a bianco e viceversa, potrebbe essere un loop
				isLoop = true;
				break;
			}
		}
		//Incremento l'indice verticale del passo
		j += V_STEP;
	}

	if(isLoop){
		//Controllo ora se anche orizzontalmente potrebbe trattarsi di un loop
		i = start_i;
		j = start_j + V_STEP; //Porto il puntatore ad una certa altezza
		bool transitionFound = false;
		while(j < h){
			if (i > 0 && i < w){
				pixGetPixel(image, i, j, &val);
				if(val > BLACK_THRES){
					//Trovato un pixel bianco
					while(!transitionFound && i > offset && i > 0){
						//Scorro verso sinistra
						pixGetPixel(image, i, j, &val);
						if(val < BLACK_THRES){
							//Bordo sinistro!
							transitionFound = true;
						} else {
							i -= H_STEP;
						}
					}

					if(transitionFound){
						i = start_i;
						transitionFound = false;
						while(!transitionFound && i < (offset + width) && i < w){
							//Scorro verso sinistra
							pixGetPixel(image, i, j, &val);
							if(val < BLACK_THRES){
								//Anche il bordo destro è presente, è un loop
								return true;
							} else {
								i += H_STEP;
							}
						}
					}
				}
			}
			j += V_STEP;
		}

	}

	return false;
}

LoopFeature::~LoopFeature(){
}



