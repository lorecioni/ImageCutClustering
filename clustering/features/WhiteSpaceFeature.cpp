/*
 * WhiteSpaceFeature.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: francesco
 *
 *      Da pensare una generalizzazione dei vari metodi, una classe StringFeature da cui derivano le altre così da richiamare sempre lo stesso metodo
 */

#include "WhiteSpaceFeature.h"

#define THRESHOLD 15

WhiteSpaceFeature::WhiteSpaceFeature(){

}

WhiteSpaceFeature::~WhiteSpaceFeature(){

}



std::string static WhiteSpaceFeature::isWhiteSpace(PIX* pix){
	unsigned int value;
	int r = pixGetAverageValue(pix, 0, 0, &value);
	if(r != -1){
		if(value > THRESHOLD){
			return " ";
		}
	}
	return "";

	//trovare funzione (?? pixColumnStats??) che dia una somma / media sull'intero PIX del colore, se esso è relativamente
	//tutto bianco allora si spara bianco e fine
}

