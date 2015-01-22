/*
 * EditDistance.cpp
 *
 *  Created on: 21/gen/2015
 *      Author: Francesco
 */

#include "EditDistance.h"


#include <algorithm>
#include <string>
#include <vector>

using namespace std;

//esiste implementazione migliore con meno spazio richiesto min{n,m} qui n*m
//migliorabile con sostituzione costo minore
int edit_distance(const string& A, const string& B)
{
    int NA = A.size();
    int NB = B.size();

    int M[NA+1][NB+1];

    for (int a = 0; a <= NA; ++a)
        M[a][0] = a;

    for (int b = 0; b <= NB; ++b)
        M[0][b] = b;

    for (int a = 1; a <= NA; ++a)
        for (int b = 1; b <= NB; ++b)
        {
            int x = M[a-1][b] + 1;
            int y = M[a][b-1] + 1;
            int z = M[a-1][b-1] + (A[a-1] == B[b-1] ? 0 : 1);
            M[a][b] = min(min(x,y),z);
        }

    return M[A.size()][B.size()];
}
