/*
 * DiagonalsAndCrossesFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 */

#include "DiagonalsAndCrossesFeature.h"

#include <leptonica/pix.h>
#include <iostream>
#include <sstream>

#define BLACK_THRES 40
#define BLACK_THRESLOW 60
using namespace std;

DiagonalsAndCrossesFeature::DiagonalsAndCrossesFeature(){

}
DiagonalsAndCrossesFeature::~DiagonalsAndCrossesFeature(){

}

//TRUE se partono Up in met� superiore DW in met� inferiore



// x1,y1 corrispondono al primo punto a sx   x2,y2 corrispondono al secondo punto a dx a prescindere se sopra o sotto
//ricordiamo white 255 black 0

bool DiagonalsAndCrossesFeature::isUpwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2, bool over, int offset, int width){

    int a1,a2,b1,b2; //a=x b=y

 cout << "dentro Up" <<endl;
	int height;
	pixGetDimensions(image, NULL, &height, NULL);

	int start = 0;
	int end = height / 2;
	if(over == true){
		start = height / 2;
		end = height;
	}

	bool firstBlackFound = false;
	int i=offset;

	while(i< offset +(width/2) ){
		unsigned int val = 0;

		cout << "dentro while" <<endl;
		for (int j = start+1; j < end - 6; j=j+2) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACK_THRES){

				unsigned int val1;
				pixGetPixel(image, i+2, j+2, &val1);
				unsigned int val2;
				pixGetPixel(image, i+3, j+3, &val2); //TODO da testare
				if( val1 < BLACK_THRESLOW && val2 < BLACK_THRESLOW){
					firstBlackFound = true;
					b1= j; b2 =j;
					a1 = i; a2 =i;

					stringstream ss4;
					ss4 << start;

					cout << "Prima_start è " + ss4.str() <<endl;

					start = j + 2;

					stringstream ss1;
					ss1 << a1;
					stringstream ss2;
					ss2 << b1;
					cout << "a1 " + ss1.str() <<endl;
					cout << "b1 " + ss2.str() <<endl;

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

				cout << "trovato primo segmento" <<endl;
				*x1 = a1;
				*x2 = a2;
				*y1 = b1;
				*y2 = b2;
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

bool DiagonalsAndCrossesFeature::isDownwardDiag(PIX* image, int* x1,int* x2,int* y1,int* y2 , bool over, int offset, int width){
	int a1,a2,b1,b2; //a=x b=y


	int height;
	pixGetDimensions(image, NULL, &height, NULL);

	int start = height;
	int end = height / 2;
	if(over == true){
		start = height / 2;
		end = 0;
	}

	bool firstBlackFound = false;
	int i=offset;

	while(i< offset +(width/2) ){
		unsigned int val = 0;

		for (int j = start -1; j > end + 6; j=j-2) {
			pixGetPixel(image, i, j, &val);
			if(val<BLACK_THRES){

				unsigned int val1;
				pixGetPixel(image, i+2, j-2, &val1);
				unsigned int val2;
				pixGetPixel(image, i+3, j-3, &val2);

				if(val1 < BLACK_THRESLOW && val2 < BLACK_THRESLOW){
					firstBlackFound = true;
					b1= j; b2 =j;
					a1 = i; a2 =i;
					start = j - 2;
					break;
				}
			}
		}

		if(firstBlackFound==true){
			while(val<BLACK_THRESLOW && b2>height-3 && a2< offset+width-3){
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
			firstBlackFound = false;
		}else{
			if(over == true){
					start = height / 2;
				}else start = height;
			i += 3;  //vo abbastanza avanti in width a cercare un punto nero risalendo /
		}
	}
	return false;
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

	bool a= isUpwardDiag(image, &d1.x, &d1.X, &d1.y, &d1.Y, false, offset, width);
	Diag d2;
	bool b= isUpwardDiag(image,&d2.x, &d2.X, &d2.y, &d2.Y, true, offset, width);
	Diag d3;
	bool c= isDownwardDiag(image,&d3.x, &d3.X, &d3.y, &d3.Y, false, offset, width);
	Diag d4;
	bool d= isDownwardDiag(image,&d4.x, &d4.X, &d4.y, &d4.Y, true, offset, width);

	bool crossx = false;
	bool crossX = false;
	if(a== true){
		if(c == true){
			if(crossing(d1.x,d1.X,d1.y,d1.Y,d3.x,d3.X,d3.y,d3.Y) == true){crossx = true;}
		}else if(d == true){
			if(crossing(d1.x,d1.X,d1.y,d1.Y,d4.x,d4.X,d4.y,d4.Y) == true){crossx = true;}
		}
	}else if (b == true){
		if(c == true){
			if(crossing(d2.x,d2.X,d2.y,d2.Y,d3.x,d3.X,d3.y,d3.Y) == true){crossX = true;}
		}else if(d == true){
			if(crossing(d2.x,d2.X,d2.y,d2.Y,d4.x,d4.X,d4.y,d4.Y) == true){crossx = true;}
		}
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
