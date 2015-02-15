/*
 * HorizontalStrokeFeature.cpp
 *
 *  Created on: 17/gen/2015
 *      Author: francesco
 */

#include "HorizontalStrokeFeature.h"

#include <leptonica/allheaders.h>
#include <iostream>
#include <sstream>

using namespace std;

#define BLACK_THRES 100
#define BLACK_THRESLOW 100
#define LOW_WIDTH 13
#define HIGH_WIDTH 20

HorizontalStrokeFeature::HorizontalStrokeFeature() {
}

HorizontalStrokeFeature::~HorizontalStrokeFeature() {
}

string HorizontalStrokeFeature::isHorizontal(PIX* image, int offset, int width){

	string found;

	int x2;
	int x1;

	int height;
	pixGetDimensions(image, NULL, &height, NULL);

	bool firstBlackFound = false;

	unsigned int val = 0;

	for(int j=0; j< height -6 ; j=j+2){

		//cout << "dentro for" <<endl;
		int i = offset+1;
		bool foundRow = false;

		while(i < offset+width-6){
			while(firstBlackFound == false && i < offset+width-6){

				pixGetPixel(image, i, j, &val);
				if(val<BLACK_THRES){
					unsigned int val1;
					pixGetPixel(image, i+2, j, &val1);
					unsigned int val2;
					pixGetPixel(image, i+3, j, &val2);
					if( val1 < BLACK_THRESLOW && val2 < BLACK_THRESLOW){//prima accettabilità
						firstBlackFound = true;
						x1 = i;
						x2 = i;

						stringstream ss1;
						ss1 << j;
						//cout << "Height attuale è " + ss1.str() <<endl;
					}
				}
				i+=2;
			}

			if(firstBlackFound==true){
				//cout << "trovato primo nero accettab" <<endl;
				while(val<BLACK_THRESLOW && x2<offset+width-4){

					pixGetPixel(image, x2+2, j , &val);
					if(val < BLACK_THRESLOW){
						x2 +=2;
					}else{
						pixGetPixel(image, x2+3, j , &val);
						if(val < BLACK_THRESLOW){
						x2 +=3;
						}
					}

					stringstream ss2;
					ss2 << x2;
					//cout << "Nero_x2 " + ss2.str() <<endl;
				}
				int w = x2-x1;
				if( w > LOW_WIDTH +3  ){ //3 per ricordare il salto fatto "nel vuoto"
					foundRow = true;
					if( j < height/2){
						found+="HH";
					}
					if(w > HIGH_WIDTH){
						found +="__--";

						//cout << "trovata riga_grande" <<endl;
					}else{
						found +="--";
						//cout << "trovata riga_picc" <<endl;
					}
				}
				firstBlackFound = false;
				i=x2;
			}
			if(foundRow==true){
				j=j+3; //mi allontano in h
				break; //non cerco più sullo stesso w
			}
		}
	}

	//cout << "fuori da for" <<endl;
	return found;
}

