/*
 * TrashFeature.h
 *
 *  Created on: 16/gen/2015
 *      Author: lorenzocioni
 */

#ifndef CLUSTERING_FEATURES_TRASHFEATURE_H_
#define CLUSTERING_FEATURES_TRASHFEATURE_H_

#include <leptonica/allheaders.h>
#include <string>
#include<vector>

class TrashFeature {
	public:
		TrashFeature();
		virtual ~TrashFeature();
		static bool isTrash(PIX* image);
		static std::string getTrashStructure();
	private:
};



#endif /*CLUSTERING_FEATURES_TRASHFEATURE_H_ */
