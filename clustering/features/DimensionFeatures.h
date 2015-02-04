/*
 * Feature.h
 *
 *  Created on: 14/mar/2014
 *      Author: alessio
 */

#ifndef DIMENSION_FEATURES_H_
#define DIMENSION_FEATURES_H_

class DimensionFeatures {
private:
	float topBlack;
	float bottomBlack;
	int changes;
public:
	DimensionFeatures();
	virtual ~DimensionFeatures();
	float getBottomBlack() const;
	void setBottomBlack(float bottomBlack);
	int getChanges() const;
	void setChanges(int changes);
	float getTopBlack() const;
	void setTopBlack(float topBlack);
	int cost();
	bool isWhite();
};

#endif /* FEATURE_H_ */
