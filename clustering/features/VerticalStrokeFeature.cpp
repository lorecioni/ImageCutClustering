/*
 * VerticalStrokeFeature.cpp
 *
 *  Created on: 16/gen/2015
 *      Author: francesco
 */

#include "VerticalStrokeFeature.h"

#include <leptonica/pix.h>
#include <iostream>
#include <sstream>

#define BLACK_THRES 40
#define BLACK_THRESLOW 60
#define LOW_HEIGHT 30
#define HIGH_HEIGHT 60

using namespace std;

VerticalStrokeFeature::VerticalStrokeFeature() {
}

VerticalStrokeFeature::~VerticalStrokeFeature() {
}


//TODO aggiungere check che il tratto verticale sia effetivamente un tratto e non una "macchia"
//che si prolunga verso l'alto,  ?? necessario?? se macchia lasciarla come macchia= agglomerato di feature
//può essere un idea


string VerticalStrokeFeature::isVertical(PIX* image, int offset, int width){

	string found;

	int y2;
	int y1;

	int height;///TODO toglimi w
	int w;
	pixGetDimensions(image, &w, &height, NULL);

	stringstream ss;
		ss << w;
		//cout << "Limite x " + ss.str() <<endl;


	bool firstBlackFound = false;

	unsigned int val = 0;

	for(int i=offset+1; i< offset+width-6 ; i=i+2){

		//cout << "dentro for" <<endl;
		int j = 0;
		bool foundColumn = false;

		while(j < height-6){
			while(firstBlackFound == false && j < height-6){

				pixGetPixel(image, i, j, &val);
				if(val<BLACK_THRES){
					unsigned int val1;
					pixGetPixel(image, i, j+2, &val1);
					unsigned int val2;
					pixGetPixel(image, i, j+3, &val2);
					if( val1 < BLACK_THRESLOW && val2 < BLACK_THRESLOW){//prima accettabilità
						firstBlackFound = true;
						y1 = j;
						y2 = j;

						stringstream ss1;
						ss1 << i;
						//cout << "Width attuale è " + ss1.str() <<endl;
					}
				}
				j+=2;
			}

			if(firstBlackFound==true){
				//cout << "trovato primo nero accettab" <<endl;
				while(val<BLACK_THRESLOW && y2<height-4){

					pixGetPixel(image, i, y2+2 , &val);
					if(val < BLACK_THRESLOW){
						y2 +=2;
					}else{
						pixGetPixel(image, i, y2+3 , &val);
						if(val < BLACK_THRESLOW){
						y2 +=3;
						}
					}

					stringstream ss2;
					ss2 << y2;
					//cout << "Nero_y2 " + ss2.str() <<endl;
				}
				int h = y2-y1;
				if( h > LOW_HEIGHT +3  ){ //3 per ricordare il salto fatto "nel vuoto"
					foundColumn = true;
					if(h > HIGH_HEIGHT){
						found +="I";

						//cout << "trovata colonna_grande" <<endl;
					}else{
						found +="i";
						//cout << "trovata colonna_picc" <<endl;
					}
				}
				firstBlackFound = false;
				j=y2;
			}
			if(foundColumn==true){
				i=i+3; //mi allontano in w
				break; //non cerco più sullo stesso h
			}
		}
	}

	//cout << "fuori da for" <<endl;
	return found;
}
