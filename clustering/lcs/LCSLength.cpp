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
	if(x.length() > y.length()) {
		return y.length() - length;
	} else {
		return x.length() - length;
	}
}

//Calcola la più lunga sottosequenza di caratteri comune
int LCSLength(string x,string y){
    int m = x.size();
    int n = y.size();
    int L[m+1][n+1];
    for(int i=0; i<=m; i++){
        for(int j=0; j<=n; j++){
            if(i==0 || j==0){
            	L[i][j] = 0;
            } else if(x[i-1]==y[j-1]){
            	L[i][j] = L[i-1][j-1]+1;
            } else {
            	L[i][j] = max(L[i-1][j],L[i][j-1]);
            }
        }
    }
    return L[m][n];
}


