#pragma once

#include "ofxIndustrialRobotCoreData.h"
#include "ofxXmlSettings.h"


class ofxIndustrialRobotMotorConverter {
public:
	ofxIndustrialRobotCoreData * armdata;
	ofxIndustrialRobotMotorConverter();
	float multFactor[5], addFactor[5];
	float lengtha[5], lengthb[5], angleAdd[5];	
	
	ofxXmlSettings *XML;
	void loadXml();
	float convertRotation(int n);
	float convertRotation(int n, float val);
	float convertRotation(int n, float val, float values[5]);
	float convertO(int n, float val);
	float convertO(int n, float val, float values[5]);


	float getMinRotation(int n, float v[5]);
	float getMaxRotation(int n, float v[5]);
};