#include <leptonica/allheaders.h>

#include <string>
#include <vector>
#include "../preprocessing/StateImage.h"



using namespace std;


char * substring(char *string, int position, int length);
PIX * pix8Binarize(PIX* pix, l_uint32 threshold);
l_int32 countFiles(char* path);
void reset(char** myPointer);
void reset(int** myPointer);
PIX* removeText(PIX* pix);
vector<string> splitStringBy(string stringToSplit, string regex);
string getState(string line);
string getGeneratedOutputFileName(string path);
