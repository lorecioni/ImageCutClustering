/*

 * DotFeature.cpp
 *
 *  Created on: 16/gen/2015
 *      Author: lorenzocioni
 */

#include "DotFeature.h"

#define BLACK_THRES 30
#define RADIUS 4 //TODO da testare
#define STEP 5

DotFeature::DotFeature(){
}

string DotFeature::isDot(PIX* img, int offset, int width){
	int imgWidth, imgHeight;
	pixGetDimensions(img, &imgWidth, &imgHeight, NULL);
	int j;
	unsigned int val = 0;
	//Scorre l'immagine orizzontalmente di STEP
	for(int i = (offset + STEP); i < (offset + width); i++){
		j = 0;
		//Scorre l'immagine verticalmente
		while(j < imgHeight){
			pixGetPixel(img, i, j, &val);
			if(val < BLACK_THRES){
				//Ho trovato un primo pixel nero, controllo se è un punto
				if(checkDot(img, i, j, offset, width)){
					return ".";
				}
			}
			//Incremento di uno STEP > RADIUS in verticale
			j += STEP;
		}
	}

	return "";
}

bool DotFeature::checkDot(PIX* image, int i, int j, int offset, int width){
	int imgWidth, imgHeight;
	pixGetDimensions(image, &imgWidth, &imgHeight, NULL);

	bool top, left, right, bottom = false;

	//Controlla se il cerchio di raggio RADIUS è all'interno della sezione dell'immagine esaminata
	if((i + RADIUS) < (offset + width)){
		right = true;
	}
	if((i - RADIUS) > offset){
		left = true;
	}
	if((j + RADIUS) < imgHeight){
		top = true;
	}
	if((j - RADIUS) > 0){
		bottom = true;
	}
	unsigned int val;

	//Verifica, in base alla porzione di immagine, se è un punto
	if(right){
		//Settore circolare destro
		pixGetPixel(image, i + (RADIUS/2), j, &val);
		//Verifica che alla metà del raggio sia ancora un pixel nero...
		if(val > BLACK_THRES){
			return false;
		} else {
			//...e che dopo il raggio ci sia una transizione da nero a bianco
			pixGetPixel(image, i + RADIUS, j, &val);
			if(val < BLACK_THRES){
				return false;
			}
		}
		if(top){
			pixGetPixel(image, i + (RADIUS/2), j + (RADIUS/2), &val);
			if(val > BLACK_THRES){
				return false;
			} else {
				pixGetPixel(image, i + RADIUS, j + RADIUS, &val);
				if(val < BLACK_THRES){
					return false;
				}
			}
		}
		if(bottom){
			pixGetPixel(image, i + (RADIUS/2), j - (RADIUS/2), &val);
			if(val > BLACK_THRES){
				return false;
			}  else {
				pixGetPixel(image, i + RADIUS, j - RADIUS, &val);
				if(val < BLACK_THRES){
					return false;
				}
			}
		}
	}

	//Il processo viene ripetuto per gli altri settori circolari
	if(left){
		//Settore circolare sinistro
		pixGetPixel(image, i - (RADIUS/2), j, &val);
		if(val > BLACK_THRES){
			return false;
		} else {
			pixGetPixel(image, i - RADIUS, j, &val);
			if(val < BLACK_THRES){
				return false;
			}
		}
		if(top){
			pixGetPixel(image, i - (RADIUS/2), j + (RADIUS/2), &val);
			if(val > BLACK_THRES){
				return false;
			} else {
				pixGetPixel(image, i - RADIUS, j - RADIUS, &val);
				if(val < BLACK_THRES){
					return false;
				}
			}
		}
		if(bottom){
			pixGetPixel(image, i - (RADIUS/2), j - (RADIUS/2), &val);
			if(val > BLACK_THRES){
				return false;
			} else {
				pixGetPixel(image, i - RADIUS, j - RADIUS, &val);
				if(val < BLACK_THRES){
					return false;
				}
			}
		}
	}

	if(top){
		//Controlla il punto di testa
		pixGetPixel(image, i, j + (RADIUS/2), &val);
		if(val > BLACK_THRES){
			return false;
		} else {
			pixGetPixel(image, i, j + RADIUS, &val);
			if(val < BLACK_THRES){
				return false;
			}
		}
	}

	if(bottom){
		//Controlla al di sotto
		pixGetPixel(image, i, j - (RADIUS/2), &val);
		if(val > BLACK_THRES){
			return false;
		} else {
			pixGetPixel(image, i, j - RADIUS, &val);
			if(val < BLACK_THRES){
				return false;
			}
		}
	}

	return true;
}

DotFeature::~DotFeature(){}


