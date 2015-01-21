/*
 * EditDistance.cpp
 *
 *  Created on: 21/gen/2015
 *      Author: Francesco
 */

#include "EditDistance.h"

#include <stddef.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

//esiste implementazione migliore con meno spazio richiesto min{n,m} qui n*m
//migliorabile con sostituzione costo minore
size_t edit_distance(const string& A, const string& B)
{
    size_t NA = A.size();
    size_t NB = B.size();

    vector<vector<size_t>> M(NA + 1, vector<size_t>(NB + 1));

    for (size_t a = 0; a <= NA; ++a)
        M[a][0] = a;

    for (size_t b = 0; b <= NB; ++b)
        M[0][b] = b;

    for (size_t a = 1; a <= NA; ++a)
        for (size_t b = 1; b <= NB; ++b)
        {
            size_t x = M[a-1][b] + 1;
            size_t y = M[a][b-1] + 1;
            size_t z = M[a-1][b-1] + (A[a-1] == B[b-1] ? 0 : 1);
            M[a][b] = min(x,y,z);
        }

    return M[A.size()][B.size()];
}
