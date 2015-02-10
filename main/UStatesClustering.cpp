#include <dirent.h>
#include <getopt.h>
#include <leptonica/allheaders.h>
#include <math.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

#include "../preprocessing/FeaturesMiner.h"
#include "../preprocessing/LinesRemover.h"
#include "../preprocessing/Projector.h"
#include "../clustering/Clusterizer.h"
#include "../clustering/FeatureExtractor.h"
#include "../utils/UsClusteringUtils.h"


#define EXTENSION "jpg"
#define VERSION "1.5.0"
#define PROJECT_NAME "UStateClustering"
#define NUM_ROWS 50

#define BORDER_OFFSET 50

using namespace std;

int execute(char* path, vector<dirent*>, int offset, int length);
void showInfo();
void showUsage();

/* Flag set by ‘--verbose’. */
static int verbose_flag;
static vector<StateImage*> listOfCroppedStates;

static mutex mtx;

int main(int argc, char *argv[]) {

	string ext = EXTENSION;
	int c;
	int N_THREAD = 2;
	char* directory;
	bool firstTime = true;
	int option_index = 0;
	/* Flags for calculating distances */
	bool LCS = true;
	bool L1 = true;

	showInfo();

	//TODO:toglimi PER DEBUG
	string mainfolder = "./Test/";
	mkdir(mainfolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);


	while (1) {

		static struct option long_options[] = {

				{ "verbose", no_argument, &verbose_flag, 1 }, { "brief", no_argument,
						&verbose_flag, 0 }, { "directory", required_argument, 0, 'd' },
						{ "threads", required_argument, 0, 't' }, { "help", no_argument,
								0, 'h' }, { "lcs", no_argument, 0, 'l' }, { "l1", no_argument, 0, 'L' }

		};
		/* getopt_long stores the option index here. */

		c = getopt_long(argc, argv, "d: t: ", long_options, &option_index);

		/* Detect the end of the options. */

		if (c == -1) {
			if (firstTime) {
				showInfo();
				showUsage();
				return -1;
			}
			break;
		}

		switch (c) {
		case 'd':
			if (optarg) {
				directory = optarg;
			}
			break;

		case 't':
			if (optarg) {
				N_THREAD = atoi(optarg);
			}
			break;
		case 'h':
			showUsage();
			abort();

		case 'l':
			L1 = false;
			break;

		case 'L':
			LCS = false;
			break;

		default:
			showUsage();
			abort();
		}
		firstTime = false;
	}

	/* Print evaluating distance metrics */
	if(LCS && L1){
		cout << "Using LCS and L1 distance" << endl;
	} else if(LCS && !L1){
		cout << "Using only LCS distance" << endl;
	} else {
		cout << "Using only L1 distance" << endl;
	}

	/* Instead of reporting ‘--verbose’
	 and ‘--brief’ as they are encountered,
	 we report the final status resulting from them. */
	if (verbose_flag) {
		puts("verbose flag is set");
	}

	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	DIR *dir;
	struct dirent *entGeneral;
	if ((dir = opendir(directory)) != NULL) {
		char* fileType;
		char*path = directory;
		fileType = (char*) malloc(3 * sizeof(char*));
		/* print all the files and directories within directory */

		l_int32 files = countFiles(directory, ext);

		int filesPerThread = files / (N_THREAD);
		int remainingFilesPerThread = files - ((N_THREAD) * filesPerThread);

		vector<dirent*> pathVector;
		vector<thread> threads;

		while ((entGeneral = readdir(dir)) != NULL) {

			fileType[0] = entGeneral->d_name[strlen(entGeneral->d_name) - 3];
			fileType[1] = entGeneral->d_name[strlen(entGeneral->d_name) - 2];
			fileType[2] = entGeneral->d_name[strlen(entGeneral->d_name) - 1];

			if (entGeneral->d_type == DT_REG && strncmp(fileType, ext.c_str(),3) == 0) {

				pathVector.push_back(entGeneral);

				//execute(path, entGeneral);
			}

		}

		int indexFiles = 0;
		for (int index = 0; index < N_THREAD; index++) {

			threads.push_back(
					thread(execute, path, pathVector, indexFiles,
							filesPerThread));

			//printf("Thread %d partito!!!\n",index);
			indexFiles += filesPerThread;

		}
		threads.push_back(
				thread(execute, path, pathVector,
						files - remainingFilesPerThread,
						remainingFilesPerThread));
		//printf("Thread %d partito!!!\n",N_THREAD-1);

		for (int index = 0; index < threads.size(); index++) {
			threads[index].join();
			//printf("Thread %d arrivato!!!\n",index);
		}

		if(listOfCroppedStates.size() != 0){

			Clusterizer* clusterizer = new Clusterizer(listOfCroppedStates, LCS, L1, begin);
			clusterizer->clusterize();

		}else{
			cout << "Nessun ritaglio estratto";
		}
		closedir(dir);

	} else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}

	ofstream f("./Clusters/result.txt", ios::app); //apre il file in modalità append, lasciando intatto quello che c'è e scrivendo alla fine
	if (!f) {
		cout << "Errore nell'apertura del file!";
		return -1;
	}

	double executionTime = evaluateTime(begin);
	printf("Tempo totale di esecuzione: %.3f s", executionTime);

	f << "Tempo totale di esecuzione:" << executionTime << endl;

	return 0;

}

int execute(char* path, vector<dirent*> entVect, int offset, int length) {
	char* filepath;
	char* croppedPath;
	unsigned int *hproj;
	unsigned int *vproj;
	int* stateColumns;
	char* cropped;
	int w, h;
	size_t arglen = strlen(path);
	Projector *projector;
	FeaturesMiner *extractor;
	int * rows;

	for (int index = offset; index < offset + length; index++) {

		dirent* ent = entVect[index];

		filepath = (char*) malloc(
				sizeof(char) * (arglen + strlen(ent->d_name) + 2));

		strcpy(filepath, "");
		strcat(filepath, path);
		strcat(filepath, ent->d_name);
		printf("%s\n", filepath);

		PIX* pixs = pixRead(filepath);
		pixGetDimensions(pixs, &w, &h, NULL);

		//Inserito un offset nel bordo per evitare
		BOX* cropWindow = boxCreate(BORDER_OFFSET, BORDER_OFFSET, w, h);
		PIX* pixd = pixClipRectangle(pixs, cropWindow, NULL);

		extractor = new FeaturesMiner();
		extractor->setImage(pixd);
		int borderW = extractor->findBlackBorder(1);
		int borderH = extractor->findBlackBorder(0);

		if (borderW != -1) {
			BOX* cropWindow = boxCreate(borderW, 0, w, h);
			pixd = pixClipRectangle(pixd, cropWindow, NULL);
		}
		if (borderH != -1) {
			BOX* cropWindow = boxCreate(0, borderH, w, h);
			pixd = pixClipRectangle(pixd, cropWindow, NULL);
		}
			/*
		//FIXME DEBUG
		static int exeNum = 0;
		cout << "croppedImage null" << (pixd == NULL) << endl;
		static int test = 0;
		if(test ==0){
			string a ="/fast/snoopy_b/santoni/ImageCutClustering/Test/";
			stringstream ss;//create a stringstream
			ss << exeNum;//add number to the stream
			exeNum++;

			a += ss.str();
			a += ".jpg";
			pixWrite(a.c_str(), pixd,IFF_JFIF_JPEG);
			test =1;
		}
		//FIXME ENDDEBUG */

		projector = new Projector(pixd, false);

		vproj = projector->verticalProjection();
		hproj = projector->horizontalProjection();

		extractor->setImage(pixd);
		extractor->setWindowsSize(10);
		extractor->setProjectionHorizontal(hproj);
		extractor->setProjectionVertical(vproj);
		stateColumns = extractor->findColums();

		if (stateColumns[0] != 0 && stateColumns[1] != 0) {

			BOX* cropWindow = boxCreate(stateColumns[0], 0,
					stateColumns[1] - stateColumns[0], h);
			PIX* croppedImage = pixClipRectangle(pixd, cropWindow, NULL);

			//get horizontalstring projection of cropped image
			pixGetDimensions(croppedImage, &w, &h, NULL);

			l_float32 angle, conf;
			PIX* pixf = pixThresholdToBinary(croppedImage, 80);
			if (pixFindSkew(pixf, &angle, &conf) == 0) {
				//printf("%s %g\n", "Skew angle:", angle);
				croppedImage = pixRotateAMGray(croppedImage,
						(M_PI / 180) * angle, (l_uint8) 0);
			}


			//pixDestroy(&pixf);
			if (pixf != NULL)
				pixFreeData(pixf);
			//PIX* imageWithoutText=removeText(croppedImage);

			PIX*croppedBinarized = pix8Binarize(croppedImage, 150);

			projector->setImage(croppedBinarized);
			projector->setSaveProjection(true);
			hproj = projector->horizontalProjection();

			extractor->setImage(croppedImage);
			extractor->setProjectionHorizontal(hproj);
			extractor->setWindowsSize(4);

			rows = extractor->findRows(5);

			PIX* croppedcroppedImage = NULL;
			PIX* auxPixs = NULL;
			for (int i = 0; i < NUM_ROWS; i++) {
				if (rows[i + 1] == 0) {
					break;
				}
				cropWindow = boxCreate(0, rows[i], w, rows[i + 1] - rows[i]);
				croppedcroppedImage = pixClipRectangle(croppedImage, cropWindow,
						NULL);
				if (croppedcroppedImage != NULL) {
					LinesRemover *remover = new LinesRemover(
							croppedcroppedImage);
					auxPixs = remover->removeLines();
					StateImage* stateImage = new StateImage(
							new string(filepath), i, auxPixs);

					//Estrazione features dalle immagini
					FeatureExtractor::extractFeatures(stateImage);

					mtx.lock();
					listOfCroppedStates.push_back(stateImage);
					mtx.unlock();
				}
			}

			if (croppedImage != NULL)
				pixFreeData(croppedImage);
			if (croppedBinarized != NULL)
				pixFreeData(croppedBinarized);
		}

		cout << "Numero ritagli: " << listOfCroppedStates.size() << endl;

		if (pixs != NULL)
			pixFreeData(pixs);
		if (pixd != NULL)
			pixFreeData(pixd);
	}

	return 1;
}

void showInfo() {

	cout << PROJECT_NAME << " " << VERSION << "\n";

}

void showUsage() {

	cout << "usage: " << PROJECT_NAME << " [-option ...]\n"
			<< "where option include:\n"
			<< "\t--directory (-d)\t directory that contains the files to be analyzed\n"
			<< "\t--threads   (-t)\t number of threads for parallel job [default value = 2]\n"
			<< "\t--LCSdistance   (--lcs)\t use only LCS distance\n"
			<< "\t--L1distance   (--l1)\t use only L1 distance\n";
}
