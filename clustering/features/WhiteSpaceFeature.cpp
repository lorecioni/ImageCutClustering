/*
 * WhiteSpaceFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 *
 *      Da pensare una generalizzazione dei vari metodi, una classe StringFeature da cui derivano le altre così da richiamare sempre lo stesso metodo
 */

#include "WhiteSpaceFeature.h"
#define THRESHOLD 230

using namespace std;

WhiteSpaceFeature::WhiteSpaceFeature(){

}

WhiteSpaceFeature::~WhiteSpaceFeature(){

}

bool WhiteSpaceFeature::isWhiteSpace(PIX* pix, int offset, int width){
	int value;
	value = getAverageValue(pix, offset, width);
	if(value > THRESHOLD){
		return true;
	}
	return false;
}

