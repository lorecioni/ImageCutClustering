#include <leptonica/allheaders.h>
#include <iostream>
#include <string>
#include <vector>
#include "../preprocessing/StateImage.h"
#include <sstream>


using namespace std;


char * substring(char *string, int position, int length);
PIX * pix8Binarize(PIX* pix, l_uint32 threshold);
l_int32 countFiles(char* path, char* ext);
void reset(char** myPointer);
void reset(int** myPointer);
PIX* removeText(PIX* pix);
vector<string> splitStringBy(string stringToSplit, string regex);
string getState(string line);
string getGeneratedOutputFileName(string path);
int getAverageValue(PIX* img, int offset, int width);
