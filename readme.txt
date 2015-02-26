ImageCutClustering
==================

An application for the extraction of primitive features from images of handwritten words in order to generate clusters of similar elements, in our case the names of American States, extracted from forms of the 1930' census

Compiling and running
-----------------------

This software has been developed in C++ version 11 and with leptonica library (v 1.70) ((http://www.leptonica.com/)).
To compile use the makefile in Debug folder. 
Make sure you have -std=c++0x and __GXX_EXPERIMENTAL_CXX0X__ options enabled.

To run the program invoke

.\ImageCutClustering -d IMAGE_DIR -t THREADS


where IMAGE_DIR is the directory where images and text files are and THREADS is the numer of threads to use.
In order to use only LCS or Euclidean distance use --lcs or --l1 options.


##Project structure

ImageCutClustering
	-> clustering/
		-> affinitypropagation/
			-> AffinityPropagationValue.cpp
			-> AffinityPropagationValue.h
			-> ap.cpp
		-> features/
			-> DiagonalsAndCrossesFeature.cpp
			-> DiagonalsAndCrossesFeature.h
			-> DimensionFeatures.cpp
			-> DimensionFeatures.h
			-> DotFeature.cpp
			-> DotFeature.h
			-> HorizontalStrokeFeature.cpp
			-> HorizontalStrokeFeature.h
			-> LoopFeature.cpp
			-> LoopFeature.h
			-> TrashFeature.cpp
			-> TrashFeature.h
			-> VerticalStrokeFeature.cpp
			-> VerticalStrokeFeature.h
			-> WhiteSpaceFeature.cpp
			-> WhiteSpaceFeature.h
		-> lcs/
			-> EditDistance.cpp
			-> EditDistance.h
			-> LCSLength.cpp
			-> LCSLength.h
		-> Clusterizer.cpp
		-> Clusterizer.h
		-> FeatureExtractor.cpp
		-> FeatureExtractor.h
	-> Debug/
		-> makefile
	-> doc/
		-> images/
		-> doc.tex
		-> doc.pdf
	-> main/
		-> UStatesClustering.cpp
	-> preprocessing/
		-> FeaturesMiner.cpp
		-> FeaturesMiner.h
		-> LinesRemover.cpp
		-> LinesRemover.h
		-> Projection.cpp
		-> Projector.h
		-> StateImage.cpp
		-> StateImage.h
	-> Results/
		-> results16_both.log
		-> results16_l1.log
		-> results16_lcs.log
		-> results160_both.log
		-> results160_l1.log
		-> results160_lcs.log
		-> results32_both.log
		-> results32_lcs.log
		-> results700_both.log
		-> results700_l1.log
		-> results700_lcs.log
		-> results80_both.log
		-> results80_l1.log
		-> results80_lcs.log
	-> utils/
		-> UsClusteringUtils.cpp
		-> UsClusteringUtils.h
	-> README.md
	-> results.xlsx	