#pragma once

#include "ofxXmlSettings.h"

class ofxIndustrialRobotPlotter {
public:
	ofxIndustrialRobotPlotter();
	ofVec3f fourthCorner();
	void loadFile(string file);

	
	ofVec3f spaceDefinition[3];
	
	ofxXmlSettings svgFile;
	bool fileLoaded;
	
	vector<vector<ofVec2f> > lines;
	
};