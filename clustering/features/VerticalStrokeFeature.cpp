/*
 * VerticalStrokeFeature.cpp
 *
 *  Created on: 16/gen/2015
 *      Author: francesco
 */

#include "VerticalStrokeFeature.h"

#include <leptonica/allheaders.h>
#include <cmath>
#include <iostream>

#define NONE 0
#define LITTLE 1
#define LONG 2
#define BLACK_THRES 40
#define BLACK_THRESLOW 60
#define LOW_HEIGHT

VerticalStrokeFeature::VerticalStrokeFeature() {
}

VerticalStrokeFeature::~VerticalStrokeFeature() {
}


//TODO aggiungere check che il tratto verticale sia effetivamente un tratto e non una "macchia"
//che si prolunga verso l'alto,  ?? necessario?? se macchia lasciarla come macchia= agglomerato di feature
//può essere un idea


int findStroke(PIX* image, int offset, int width){
		int height;
		pixGetDimensions(image, NULL, &height, NULL);

		bool firstBlackFound = false;
		int currentWidth;
		int start=offset+1;
		int j = 0;
		while(j < height - 7){
			j += 6;  // scansiono in orizzontale ogni 6 pixel
			unsigned int val = 0;

			cout << "dentro while" <<endl;
			for(int i=start; i< offset+width-3 ; i=i+2){

				pixGetPixel(image, i, j, &val);
				if(val<BLACK_THRES){

					unsigned int val1;
					pixGetPixel(image, i+1, j+2, &val1);//checkko da sinistra verso dx se è spesso è spesso a dx
					unsigned int val2;
					pixGetPixel(image, i+2, j+3, &val2); //TODO da testare
					if( val1 < BLACK_THRESLOW && val2 < BLACK_THRESLOW){
						currentWidth = i+1;

						stringstream ss4;
						ss4 << start;
						cout << "Prima_start è " + ss4.str() <<endl;

						stringstream ss1;
						ss4 << currentWidth;
						cout << "Width attuale è " + ss1.str() <<endl;

						start = i + 2;

						stringstream ss3;
						ss3 << start;
						cout << "start è " + ss3.str() <<endl;
						break;
					}
				}
			}

			if(firstBlackFound==true){
				cout << "trovato primo nero" <<endl;
				while(val<BLACK_THRESLOW && b2<height-3 && a2< offset+width-3){
					unsigned int a,b,c;
					pixGetPixel(image, a2+1 ,b2+2 , &a);
					pixGetPixel(image, a2+1 ,b2+1 , &b);
					pixGetPixel(image, a2+2 ,b2+1 , &c);
					if(a<=b && a<=c){
						val=a;
						a2 +=1;
						b2 +=2;
					}else if(b<=c){
						val=b;
						a2 +=1;
						b2 +=2;
					}else{
						val=c;
						a2 +=2;
						b2 +=1;
					}

					stringstream ss1;
					ss1 << a2;
					stringstream ss2;
					ss2 << b2;
					cout << "Nero_a2 " + ss1.str() <<endl;
					cout << "Nero_b2 " + ss2.str() <<endl;

				}
				if( a2+b2-a1-b1> width/2 ){

					cout << "trovata colonna" <<endl;
						return true;
				}
				firstBlackFound = false;
			}else {

				cout << "dopo else" <<endl;
				i += 3;  //vo abbastanza avanti in width a cercare un punto nero risalendo /

				if(over == true){
					start = height / 2;
				}else start = 0;

			}
		}

		cout << "fuori da while" <<endl;
		return false;
}



string isVertical(PIX* image, int offset, int width){
	int result = findStroke(image,offset,width);
	if(result==LITTLE)return "i";
	else if(result==LONG)return "I";
	else return "";
}
