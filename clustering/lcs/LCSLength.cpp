/*
 * LCSLength.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#include "LCSLength.h"

int LCSDistance(string x, string y){
	//Calcola la distanza tra due stringhe
	int length = LCSLength(x, y);
	int difference = 0;
	//TODO da valutare come interpretare la distanza

	//così se uno è completamente contenuto in uno più grande la distanza è 0,
	//idealmente ciò permette di raggruppare oggetti uguali ma per cui in
	//uno il primo pezzo non è stato beccato,
	//MALE perchè il nostro compito è precision non recall,
	//si finisce per raggruppare una "/" con "/adsdads"
/*
	if(x.length() > y.length()) {
		difference = x.length() - y.length();
		return (y.length() - length);
	} else {
		difference = y.length() - x.length();
		return (x.length() - length) ;
	}
	*/

	//così si restituisce il numero di lettere in + da entrambi i lati
	return x.length() + y.length() - 2* length;
}

//Calcola la piÃ¹ lunga sottosequenza di caratteri comune
int LCSLength(string x,string y){
    int m = x.size();
    int n = y.size();
    int L[m+1][n+1];
    for(int i=0; i<=m; i++){
        for(int j=0; j<=n; j++){
            if(i==0 || j==0){
            	L[i][j] = 0;
            } else if(x[i]==y[j]){
            	L[i][j] = L[i-1][j-1]+1;
            } else {
            	L[i][j] = max(L[i-1][j],L[i][j-1]);
            }
        }
    }
    return L[m][n];
}


