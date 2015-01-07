/*
 * DiagonalsAndCrossesFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 */

#include "DiagonalsAndCrossesFeature.h"
#define BLACK_THRES 60

DiagonalsAndCrossesFeature::DiagonalsAndCrossesFeature(){

}
DiagonalsAndCrossesFeature::~DiagonalsAndCrossesFeature(){

}

//TRUE se partono Up in metà superiore DW in metà inferiore



// x1,y1 corrispondono al primo punto a sx   x2,y2 corrispondono al secondo punto a dx a prescindere se sopra o sotto
//ricordiamo white 255 black 0

bool DiagonalsAndCrossesFeature::isUpwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2, bool over, int offset, int width){

    int a1,a2,b1,b2; //a=x b=y


	int height;
	pixGetDimensions(image, NULL, &height, NULL);

	int start = 0;
	int end = height / 2;
	if(over == true){
		start = height / 2;
		end = end *2;
	}

	bool firstBlackFound = false;
	int i=offset;

	while(i< offset +(width/2) ){
		unsigned int val = 0;

		for (int j = start+1; j < end - 6; j++) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACK_THRES){

				if(pixGetPixel(image, i+2, j+2, &val) < BLACK_THRES && pixGetPixel(image, i+5, j+5, &val) < BLACK_THRES){
					firstBlackFound = true;
					b1= j; b2 =j;
					a1 = i; a2 =i;
					break;
				}
			}
		}

		if(firstBlackFound==true){
			while(val<BLACK_THRES && b2<height-3 && a2< offset+width-3){
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
			}
		}else i += 3;  //vo abbastanza avanti in width a cercare un punto nero risalendo /
	}
	return false; //
}

bool DiagonalsAndCrossesFeature::isDownwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2 , bool over, int offset, int width){
	int a1,a2,b1,b2; //a=x b=y


	int height;
	pixGetDimensions(image, NULL, &height, NULL);

	int start = height;
	int end = height / 2;
	if(over == true){
		start = start / 2;
		end = 0;
	}

	bool firstBlackFound = false;
	int i=offset;

	while(i< offset +(width/2) ){
		unsigned int val = 0;

		for (int j = start -1; j > end + 6; j--) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACK_THRES){

				if(pixGetPixel(image, i+2, j-2, &val) < BLACK_THRES && pixGetPixel(image, i+5, j-5, &val) < BLACK_THRES){
					firstBlackFound = true;
					b1= j; b2 =j;
					a1 = i; a2 =i;
					break;
				}
			}
		}

		if(firstBlackFound==true){
			while(val<BLACK_THRES && b2>height-3 && a2< offset+width-3){
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
			if( a2-b2-a1+b1> width/2 ){
				*x1 = a1;
				*x2 = a2;
				*y1 = b1;
				*y2 = b2;
				return true;
			}
		}else i += 3;  //vo abbastanza avanti in width a cercare un punto nero risalendo /
	}
	return false; //
}



string DiagonalsAndCrossesFeature::isCross(PIX* image, int offset, int width){
	string report;

	struct diag{
		int x;
		int y;
		int X;
		int Y;
	};
	typedef diag Diag;

	Diag d1;

	bool a= isUpwardDiag(image,&d1.x, &d1.X, &d1.y, &d1.Y, NULL, offset, width);
	Diag d2;
	bool b= isUpwardDiag(image,&d2.x, &d2.X, &d2.y, &d2.Y, true, offset, width);
	Diag d3;
	bool c= isDownwardDiag(image,&d3.x, &d3.X, &d3.y, &d3.Y, NULL, offset, width);
	Diag d4;
	bool d= isDownwardDiag(image,&d4.x, &d4.X, &d4.y, &d4.Y, true, offset, width);

	bool crossx = false;
	bool crossX = false;
	if(a== true){
		if(c == true){
			if(crossing(d1.x,d1.X,d1.y,d1.Y,d3.x,d3.X,d3.y,d3.Y) == true){crossx = true;}
		}else if(d == true)	if(crossing(d1.x,d1.X,d1.y,d1.Y,d4.x,d4.X,d4.y,d4.Y) == true){crossx = true;}
	}else if (b == true){
		if(c == true){
			if(crossing(d2.x,d2.X,d2.y,d2.Y,d3.x,d3.X,d3.y,d3.Y) == true){crossX = true;}
		}else if(d == true)	if(crossing(d2.x,d2.X,d2.y,d2.Y,d4.x,d4.X,d4.y,d4.Y) == true){crossx = true;}
	}

	if(a==true) report+= "s";
	if(b==true) report+= "S";
	if(c==true) report+= "U";
	if(d==true) report+= "u";
	if(crossx==true) report+= "x";
	if(crossX==true) report+= "X";

	return report;
}

bool DiagonalsAndCrossesFeature::crossing(int x1,int x2,int y1,int y2, int x11,int x22,int y11,int y22){
	if(x2<x11 || y1>y11 || y2<y22 || x1>x22){ return false;}
	return true;
}
