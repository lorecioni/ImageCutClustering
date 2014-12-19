/*
 * Feature.h
 *
 *  Created on: 14/mar/2014
 *      Author: alessio
 */

#ifndef FEATURE_H_
#define FEATURE_H_

class Feature {
private:
	float topBlack;
	float bottomBlack;
	int changes;
public:
	Feature();
	virtual ~Feature();
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
