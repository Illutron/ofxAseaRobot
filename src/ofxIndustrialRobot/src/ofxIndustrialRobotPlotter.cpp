
#include "ofxIndustrialRobotPlotter.h"

ofxIndustrialRobotPlotter::ofxIndustrialRobotPlotter(){
	spaceDefinition[0] = ofxVec3f(1900,500, -500);
	spaceDefinition[1] = ofxVec3f(1900,500, 500);
	spaceDefinition[2] = ofxVec3f(1400,500, 500);
	loadFile("/Users/jonas/Desktop/Untitled-1.svg");
};


ofxVec3f ofxIndustrialRobotPlotter::fourthCorner(){
	ofxVec3f t = spaceDefinition[1] - spaceDefinition[0];
	return spaceDefinition[2] - t;
}


void ofxIndustrialRobotPlotter::loadFile(string file){
	/*svgFile.loadFile(file);
	fileLoaded = true;
	
	svgFile.pushTag("svg", 0);
	int numLines =  svgFile.getNumTags("line");
	for(int i=0;i<numLines;i++){
		vector<ofxVec2f> newLine;
		newLine.push_back(ofxVec2f(svgFile.getAttribute("line", "x1", 0.0, i), svgFile.getAttribute("line", "y1", 0.0, i)));
		newLine.push_back(ofxVec2f(svgFile.getAttribute("line", "x2", 0.0, i), svgFile.getAttribute("line", "y2", 0.0, i)));
		lines.push_back(newLine);
	}
	
	svgFile.popTag();*/
}
