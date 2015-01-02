/*
 * LCSLength.cpp
 *
 *  Created on: 30/dic/2014
 *      Author: lorenzocioni
 */

#include "LCSLength.h"

//added whitespace "support"

int LCSLength(string X,string Y){

	string x1;
	string y1;
	int j=0;
	int k=0;
	for (string::size_type i = 0; i < X.length(); i++)
	{
		if(X[i]!=" "){
			x1[j++] =(X[i]);
		}
	}
	for (string::size_type i = 0; i < Y.length(); i++)
	{
		if(Y[i]!=" "){
			y1[k++] =(Y[i]);
		}
	}

	int m = x1.size();
	int n = y1.size();
	int L[m+1][n+1];

	for(int i=0; i<=m; i++){
		for(int j=0; j<=n; j++){
			if(i==0 || j==0)
                L[i][j] = 0;
            else if(x1[i-1]==y1[j-1])
                L[i][j] = L[i-1][j-1]+1;
            else
                L[i][j] = max(L[i-1][j],L[i][j-1]);
        }
    }
    return L[m][n];
}


