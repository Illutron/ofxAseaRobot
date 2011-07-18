/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "cv.h"
#include "ofxVectorMath.h"


class oRect {
public:
	ofxPoint2f *p;
	int avgCounter;
	oRect(){
		p = new ofxPoint2f[4];
		p[0] = ofxPoint2f();
		p[1] = ofxPoint2f();
		p[2] = ofxPoint2f();
		p[3] = ofxPoint2f();
		avgCounter = 1;
		
	}
	oRect(ofxPoint2f p1, ofxPoint2f p2, ofxPoint2f p3, ofxPoint2f p4){
		p = new ofxPoint2f[4];
		p[0] = p1;
		p[1] = p2;
		p[2] = p3;
		p[3] = p4;
		
		avgCounter = 1;
	}
	
	float w(){
		ofxVec2f v = ofxVec2f((p[1]-p[0]).x, (p[1]-p[0]).y);
		ofxVec2f v2 = ofxVec2f((p[3]-p[2]).x, (p[3]-p[2]).y);
		if(v.length()< v2.length())
			return v.length();
		else 
			return v2.length();
	}
	float h(){
		ofxVec2f v = ofxVec2f((p[2]-p[1]).x, (p[2]-p[1]).y);
		ofxVec2f v2 = ofxVec2f((p[0]-p[3]).x, (p[0]-p[3]).y);
		if(v.length()< v2.length())
			return v.length();
		else 
			return v2.length();	
	}
	
	float a(){	
		ofxVec2f v = ofxVec2f((p[1]-p[0]).x, (p[1]-p[0]).y);
		
		return  v.angle(ofxVec2f(1.0,0.0));
	}
	
	void add(oRect r){
		avgCounter++;
		for(int i=0;i<4;i++){
			//			if(r.p[i].x != 0 && r.p[i].y != 0){
			
			p[i] = ofxPoint2f(p[i]*(1.0-1.0/(double)avgCounter) + r.p[i]*(1.0/(double)avgCounter)); 
			//			}
		}
	}
};


class PaperTracker : public BasePlugin {
public:
	
	virtual void init(myData * _data,  QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	virtual void threadedFunction();
	

	void canny(ofxCvGrayscaleImage * img,int lower, int higher);
	CvSeq* HoughLines(ofxCvGrayscaleImage * img,int    method, double rho,  double theta, int    threshold, int param1, int param2);
	myData * data;
	
protected:
	
	CvSeq* lines;
	bool anyLines;
	vector<vector<CvPoint*> > pLines;
	vector<ofxPoint2f> rectPoints;
	
	bool drawMonitor;
	
	bool doCanny;
	int lowerCanny;
	int higherCanny;
	
	float speed;
	
	bool doHough;
	bool linesBeingUpdate;
	int houghMethod;
	double houghRho;
	double houghTheta;
	int houghthreshold;
	double houghParam1;
	double houghParam2;
	
	bool doRectTracking;
	float parAMargin;
	float parMinDist;
	float parMaxDist;
	
	float rectAMargin;
	oRect * avgRect;
	oRect * capturedRect;
	float captureCounter;
	vector<vector<CvPoint*> > findParalLines();	
	vector<ofxPoint2f> findRect(vector<vector<CvPoint*> >);
	
	float getA(CvPoint* line);
	float getB(CvPoint* line);
	
	float dist(int x1, int y1, int x2, int y2);
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayImageScaled;
	ofxCvGrayscaleImage		grayImageScaled2;
	QSlider * sliderHughParam1;
	QSlider * sliderHughParam2;
	QSlider * sliderHoughthreshold;
	QSlider * sliderHoughRho;
	QSlider * sliderHoughTheta;
	QSlider * sliderLowerCanny;
	QSlider * sliderHigherCanny;
	
	
	
};
