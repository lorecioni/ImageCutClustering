/*
 * DiagonalsAndCrossesFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 */

#include "DiagonalsAndCrossesFeature.h"
#define TRIES 15
#define BLACKTHRES 60

DiagonalsAndCrossesFeature::DiagonalsAndCrossesFeature(){

}
DiagonalsAndCrossesFeature::~DiagonalsAndCrossesFeature(){

}

// x1,y1 corrispondono al primo punto a sx   x2,y2 corrispondono al secondo punto a dx a prescindere se sopra o sotto
//ricordiamo white 255 black 0

bool DiagonalsAndCrossesFeature::isUpwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2){

    int a1,a2,b1,b2; //a=x b=y

	int width;
	int height;
	pixGetDimensions(image, &width, &height, NULL);
	bool firstBlackFound = false;
	int i=0;
	int tries= TRIES;
	while(i<(width/2) && tries>0){
		unsigned int val = 0;

		for (int j =0; j < height; j++) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACKTHRES){
				firstBlackFound = true;
				b1= j; b2 =j;
				a1 = i; a2 =i;
				break;
			}
		}

		if(firstBlackFound==true){
			while(val<BLACKTHRES && b2<=height && a2<= width){
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
			}
			if( a2+b2-a1-b1> width/2 ){
				*x1 = a1;
				*x2 = a2;
				*y1 = b1;
				*y2 = b2;
				return true;
			}else tries = tries -1;

		}else i += width/8;  //vo abbastanza avanti in width a cercare un punto nero risalendo /
	}
	return false;
}

bool DiagonalsAndCrossesFeature::isDownwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2){
	int a1,a2,b1,b2; //a=x b=y

	int width;
	int height;
	pixGetDimensions(image, &width, &height, NULL);
	bool firstBlackFound = false;
	int i=0;
	int tries= TRIES;
	while(i<(width/2) && tries>0){
		unsigned int val = 0;

		for (int j =height; j >0; j--) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACKTHRES){
				firstBlackFound = true;
				b1= j; b2 =j;
				a1 = i; a2 =i;
				break;
			}
		}

		if(firstBlackFound==true){
			while(val<BLACKTHRES && b2<=height && a2<= width){
				unsigned int a,b,c;
				pixGetPixel(image, a2+1 ,b2-2 , &a);
				pixGetPixel(image, a2+1 ,b2-1 , &b);
				pixGetPixel(image, a2+2 ,b2-1 , &c);
				if(a<=b && a<=c){
					val=a;
					a2 +=1;
					b2 -=2;
				}else if(b<=c){
					val=b;
					a2 +=1;
					b2 -=2;
				}else{
					val=c;
					a2 +=2;
					b2 -=1;
				}
			}
			if( a2+b1-a1-b2> width/2 ){
				*x1 = a1;
				*x2 = a2;
				*y1 = b1;
				*y2 = b2;
				return true;
			}else tries = tries -1;

		}else i += width/8;  //vo abbastanza avanti in width a cercare un punto nero Scendendo "\"
	}
	return false;

}


string DiagonalsAndCrossesFeature::isCross(PIX* image){
	string report;

	int x1,x2,y1,y2;
	int x11,x22,y11,y22;

	bool Up = isUpwardDiag(image,&x1,&x2,&y1,&y2);
	bool Dw = isDownwardDiag(image,&x11,&x22,&y11,&y22);
	if(Up != NULL){
		report += "s";

		if(Dw != NULL){
			if(crossing( x1,x2,y1,y2, x11,x22,y11,y22)){
				report+="XS";
			}else report+="S";
		}
	}else{
		if(Dw != NULL){
			report+="S";
		}
	}
	return report;
}

bool DiagonalsAndCrossesFeature::crossing(int x1,int x2,int y1,int y2, int x11,int x22,int y11,int y22){
	if(x2<x11 || y1>y11 || y2<y22 || x1>x22){ return false;}
	return true;
}
