/*
 * Projection.h

 *
 *  Created on: 27/gen/2014
 *  Author: Alessio Melani
 *  Author: Moreno Niccolai
 */

#include <leptonica/allheaders.h>


#ifndef PROJECTION_H_
#define PROJECTION_H_

class Projector {

private:

	PIX* imgToProject;
	int width;
	int height;
	int density;
	bool isSave;

public:

	Projector(PIX* myImage,bool save);
	unsigned int* horizontalProjection();
	unsigned int* verticalProjection();
	void setImage(PIX* myImage);
	void setSaveProjection(bool save);
	virtual ~Projector();
};

#endif /* PROJECTION_H_ */
