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
	return x.length() + y.length() - 2* length;
}

//Calcola la più lunga sottosequenza di caratteri comune
int LCSLength(string x,string y){
    int m = x.size();
    int n = y.size();
    int L[m+1][n+1];

    for (int i = 0; i <=m; ++i)
    	L[i][0] = 0;

    for (int j = 0; j <= n; ++j)
    	L[0][j] = 0;

    for(int i=1; i<=m; i++){
        for(int j=1; j<=n; j++){
        	if(x[i]==y[j]){
            	L[i][j] = L[i-1][j-1]+1;
            } else {
            	L[i][j] = max(L[i-1][j],L[i][j-1]);
            }
        }
    }
    return L[m][n];
}


