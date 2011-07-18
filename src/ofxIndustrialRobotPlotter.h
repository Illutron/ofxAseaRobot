#pragma once

#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

class ofxIndustrialRobotPlotter {
public:
	ofxIndustrialRobotPlotter();
	ofxVec3f fourthCorner();
	void loadFile(string file);

	
	ofxVec3f spaceDefinition[3];
	
	ofxXmlSettings svgFile;
	bool fileLoaded;
	
	vector<vector<ofxVec2f> > lines;
	
};