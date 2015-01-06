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
#include "../utils/UsClusteringUtils.h"


#include "../clustering/features/FeatureExtractor.h"


#define JPG "jpg"
#define VERSION "1.0.0"
#define PROJECT_NAME "UStateClustering"

using namespace std;

int execute(char* path, vector<dirent*>, int offset, int length);
void showInfo();
void showUsage();
void testFeatures(std::vector<StateImage*> vectorOfStates);

/* Flag set by ‘--verbose’. */
static int verbose_flag;
static vector<StateImage*> listOfCroppedStates;

static mutex mtx;

int main(int argc, char *argv[]) {


	int c;
	int N_THREAD = 2;
	char* directory;
	bool firstTime = true;
	int option_index = 0;
	while (1) {

		static struct option long_options[] = {

		{ "verbose", no_argument, &verbose_flag, 1 }, { "brief", no_argument,
				&verbose_flag, 0 }, { "directory", required_argument, 0, 'd' },
				{ "threads", required_argument, 0, 't' }, { "help", no_argument,
						0, 'h' }

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
			showInfo();
			showUsage();
			abort();

		default:
			showInfo();
			showUsage();
			abort();
		}
		firstTime = false;
	}

	/* Instead of reporting ‘--verbose’
	 and ‘--brief’ as they are encountered,
	 we report the final status resulting from them. */
	if (verbose_flag) {
		puts("verbose flag is set");
	}
	/* Print any remaining command line arguments (not options). */
	if (optind < argc) {
		showInfo();
		//showusage();
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

		l_int32 files = countFiles(directory);

		int filesPerThread = files / (N_THREAD);
		int remainingFilesPerThread = files - ((N_THREAD) * filesPerThread);

		vector<dirent*> pathVector;
		vector<thread> threads;

		while ((entGeneral = readdir(dir)) != NULL) {

			fileType[0] = entGeneral->d_name[strlen(entGeneral->d_name) - 3];
			fileType[1] = entGeneral->d_name[strlen(entGeneral->d_name) - 2];
			fileType[2] = entGeneral->d_name[strlen(entGeneral->d_name) - 1];

			if (entGeneral->d_type == DT_REG && strcmp(fileType, JPG) == 0) {

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


		testFeatures(listOfCroppedStates);

		Clusterizer* clusterizer = new Clusterizer(listOfCroppedStates);
		clusterizer->clusterize();
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

	gettimeofday(&end, NULL);
	double diff_sec = difftime(end.tv_sec, begin.tv_sec) * CLOCKS_PER_SEC;
	double diff_milli = difftime(end.tv_usec, begin.tv_usec);
	printf("tempo totale di esecuzione: %.3f s",
			(diff_sec + diff_milli) / CLOCKS_PER_SEC);

	f << "Tempo totale di esecuzione:" << (diff_sec + diff_milli) / CLOCKS_PER_SEC << endl;

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

//printf("offset: %d length: %d\n",offset,length);

	for (int index = offset; index < offset + length; index++) {

		dirent* ent = entVect[index];
		//listOfCroppedStates.clear();

		filepath = (char*) malloc(
				sizeof(char) * (arglen + strlen(ent->d_name) + 2));
		/*croppedPath =	(char*) malloc(sizeof(char) * (arglen + strlen("output/") + strlen(ent->d_name) - 4 + 2));
		 cropped		=	(char*) malloc(sizeof(char)	* (arglen + strlen("output/") + strlen(ent->d_name) + 2));*/

		strcpy(filepath, "");
		strcat(filepath, path);
		strcat(filepath, ent->d_name);
		printf("%s\n", filepath);
		PIX* pixs = pixRead(filepath);

		/*strcpy(croppedPath, "");
		 strcat(croppedPath, path);
		 strcat(croppedPath, substring(ent->d_name, 0, strlen(ent->d_name) - 4));
		 strcat(croppedPath, "-");
		 strcat(croppedPath, "output/");*/

		//mkdir(croppedPath, S_IRWXU | S_IRWXG | S_IRWXO);
		PIX* pixd = pixs;
		pixGetDimensions(pixd, &w, &h, NULL);

		extractor = new FeaturesMiner();
		extractor->setImage(pixd);
		int borderW = extractor->findBlackBorder(1);
		int borderH = extractor->findBlackBorder(0);

		if (borderW != -1) {
			//pixd = pixRemoveBorder(pixd, borderW);
			BOX* cropWindow = boxCreate(borderW, 0, w, h);
			pixd = pixClipRectangle(pixd, cropWindow, NULL);
		}
		if (borderH != -1) {
			//pixd = pixRemoveBorder(pixd, borderW);
			BOX* cropWindow = boxCreate(0, borderH, w, h);
			pixd = pixClipRectangle(pixd, cropWindow, NULL);
		}

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

			//pixWrite("bin.jpg",croppedBinarized,IFF_JFIF_JPEG);
			projector->setImage(croppedBinarized);
			projector->setSaveProjection(true);
			hproj = projector->horizontalProjection();

			extractor->setImage(croppedImage);
			extractor->setProjectionHorizontal(hproj);
			extractor->setWindowsSize(4);

			rows = extractor->findRows(5);

			PIX* croppedcroppedImage = NULL;
			PIX* auxPixs = NULL;
			for (int i = 0; i < 40; i++) {
				if (rows[i + 1] == 0) {
					break;
				}
				cropWindow = boxCreate(0, rows[i], w, rows[i + 1] - rows[i]);
				croppedcroppedImage = pixClipRectangle(croppedImage, cropWindow,
						NULL);
				if (croppedcroppedImage != NULL) {
					//strcpy(cropped, "");
					//strcat(cropped, croppeAlabamadPath);
					//char* buffer = (char*) malloc(sizeof(char) * strlen("40_crop") + 2);
					//sprintf(buffer, "%d_crop", i);
					//strcat(cropped, buffer);
					//strcat(cropped, ".jpg");
					LinesRemover *remover = new LinesRemover(
							croppedcroppedImage);
					auxPixs = remover->removeLines();
					StateImage* stateImage = new StateImage(
							new string(filepath), i, auxPixs);
					mtx.lock();
					listOfCroppedStates.push_back(stateImage);
					mtx.unlock();
					//TODO debug
					//pixWrite(cropped, auxPixs, IFF_JFIF_JPEG);
				}
			}

			if (croppedImage != NULL)
				pixFreeData(croppedImage);
			if (croppedBinarized != NULL)
				pixFreeData(croppedBinarized);
		}
		/*strcpy(cropped, "");
		 strcat(cropped, croppedPath);
		 char* buffer = (char*) malloc(sizeof(char) * strlen(ent->d_name) + 2);
		 sprintf(buffer, ent->d_name);
		 strcat(cropped, buffer);*/
		//pixWrite(cropped, pixd, IFF_JFIF_JPEG);
		if (pixs != NULL)
			pixFreeData(pixs);
		if (pixd != NULL)
			pixFreeData(pixd);

		/*Abbiamo eseguito ritaglio e pulizia dell'immagine procediamo ad estrarre le features per il clustering*/

		/*clusterizer->clusterize();*/

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
			<< "\t--threads   (-t)\t number of threads for parallel job [default value = 2]\n";
}

void testFeatures(std::vector<StateImage*> vectorOfStates){
	//Metodo main per il test delle nuove features


	int i = 1;

	//crea folder /clusters/ dentro debug
	string mainfolder = "./Test/";
	mkdir(mainfolder.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);


	string path = "./Test/";

	std::vector<PIX*> vector;
	vector =  FeatureExtractor::cutImage( vectorOfStates[i]->getImage());
	int parts =  vector.size();

	for(int j=0; j< parts ; j++){

		stringstream ss;
		ss << j;

		//crea nuovo file immagine da PIX
		string name = ss.str() + ".jpg";
		string filepath = path + name;

		cout << filepath << endl;
		pixWrite(filepath.c_str(), vector[j],
				IFF_JFIF_JPEG);


		//crea file testo

		ofstream f("./Test/"+ ss.str() + ".txt"); //se il file non esiste lo crea, altrimenti lo sovrascrive!
		if (!f) {
			cout << "Errore nella creazione del file!";
			return;
		}


		//inserisce in file testo stringa
		f << "test " +ss.str() <<  endl;
		f.close();

	}
}
