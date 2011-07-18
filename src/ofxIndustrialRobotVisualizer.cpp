
#include "ofxIndustrialRobotVisualizer.h"

ofxIndustrialRobotVisualizer::ofxIndustrialRobotVisualizer(ofxIndustrialRobotArmHelper * _helper,  ofxIndustrialRobotController * _controller, ofBaseApp * _app, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter){
	helper = _helper;
	controller = _controller;
	app = _app;
	timeline = _timeline;
	plotter = _plotter;
	
	rotationSpeed = 0.3;	
	camera.position(ofGetWidth()/2.0, ofGetHeight()/2.00+100, 800);
	light1.specular(255, 255, 255);
	
	
//	model.loadModel("/Users/jonas/Desktop/test2.3ds", 1.0);
	
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
	//    model.setRotation(0, 90, 1, 0, 0);
	//  model.setRotation(1, 270, 0, 0, 1);
	// model.setScale(0.9, 0.9, 0.9);
	//    model.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
}

//---

void ofxIndustrialRobotVisualizer::drawGeneric(){
	
	//Base
	ofSetColor(0, 255, 255);	
	glTranslated(0, -25, 0);
	//ofxBox(ofxVec3f(0,25,0), ofxVec3f(200, 50, 200));	
	
	//model.draw();
	ofSetColor(255, 0, 0);
	glPushMatrix();
	glScaled(1.0, -1.0, 1.0);
	ofSetColor(255, 255, 255);

//	model.draw();
	glPopMatrix();
	//	ofxSphere(ofxVec3f(0,0,0), ofxVec3f(200, 200, 200));
	
	glRotated(180, 0.0, 0.0, -1.0); 
	glPushMatrix();
		glScaled(1.0, 1.0, -1.0);
	//	glScaled(0.1, 0.1, 0.1);
	drawArm0();
	drawArm1();
	drawArm2();
	drawArm3();
	drawArm4();
	//Draw tool
	ofSetColor(255, 0, 0);
	float l = helper->getTool()->l; //The length of the tool
	ofxVec3f rotation = helper->getTool()->getRotation();
	glRotated(rotation.z, 0.0, 0.0, 1.0);
	ofxBox(ofxVec3f(helper->getTool()->h/2.0,l/2,0), ofxVec3f(helper->getTool()->h, l, helper->getTool()->w), ofxVec3f(0.0, 0.0, 0.0));
	//	ofxVec3f axis = helper->getAxis(i); //The axis the arm rotates around
	//	ofxVec3f offset = helper->getOffset(i);
	//	glRotated(helper->getRotationAngle(i), axis.x, axis.y, axis.z); // Rotate it around the axis
	//	glTranslated(0, l, 0);
	//	ofxBox(ofxVec3f(0,-l/2.0,0), ofxVec3f(60, l, 60), ofxVec3f(0.0, 0.0, 0.0));
	//	glTranslated(-offset.y, offset.x, offset.z);	
	
	glPopMatrix();
	glScaled(1.0, 1.0, -1.0);

	//Target
	glRotated(180, 0.0, 1.0, 0.0); 
	ofSetColor(255, 0, 0);
	ofxSphere(controller->targetPosition, ofxVec3f(20, 20, 20));
	glBegin(GL_LINE_STRIP);
	glVertex3d(controller->targetPosition.x, controller->targetPosition.y, controller->targetPosition.z);
	ofxVec3f temp = controller->targetPosition+ controller->targetDir*500.0;
	glVertex3d(temp.x,temp.y, temp.z);
	glEnd();
	
	glPushMatrix();
	ofSetColor(0, 255, 0);
	for(int i=0;i<timeline->numberPositionCues(); i++){
		ofxVec3f vec = timeline->getPositionCue(i)->position;
		ofxSphere(vec, ofxVec3f(10, 10, 10));	
	}
	timeline->spline.drawRaw(10, 5);
	ofSetColor(255, 255, 0, 100);
	timeline->spline.drawSmooth(50, 0, 10);
	glLineWidth(1);
	ofSetColor(255, 255, 255, 255);
	ofFill();
	//	glDisable(GL_DEPTH_TEST);
	
/*	glBegin(GL_POLYGON);
	for(int i=0;i<3;i++){
		glVertex3f(plotter->spaceDefinition[i].x,plotter->spaceDefinition[i].y,plotter->spaceDefinition[i].z);
	}
	glVertex3f(plotter->fourthCorner().x,plotter->fourthCorner().y,plotter->fourthCorner().z);
	glEnd();
*/
	for(int i=0;i<plotter->lines.size();i++){
		glBegin(GL_LINE_STRIP);
		ofxVec3f x = (plotter->spaceDefinition[1]-plotter->spaceDefinition[0]).normalized();
		ofxVec3f y = (plotter->spaceDefinition[2]-plotter->spaceDefinition[1]).normalized();
		for(int u=0;u<plotter->lines[i].size();u++){
			ofxVec3f point = x * plotter->lines[i][u].x + y * plotter->lines[i][u].y + plotter->spaceDefinition[0];
			glVertex3f(point.x,point.y,point.z);
		}

		glEnd();
	}
	
	glPopMatrix();
	
	
}

void ofxIndustrialRobotVisualizer::draw3d(int x, int y, int w, int h){	
	//	camera.orbitAround(ofxVec3f(ofGetWidth()/2.0,ofGetHeight()/2.0,0), ofxVec3f(0.0,1.0,0.0), rotationSpeed);
	light1.directionalLight(255, 255, 255, 0.0, 0.3, 1.0);
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glTranslated(x, y, 0);
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 20);
	ofRect(0, 0, w, h);
	glPushMatrix();
	glViewport(x, ofGetHeight()-h-y, w, h);
	glEnable(GL_DEPTH_TEST);
	glTranslated(0, 1000, -2500);
	
	ofxLightsOn();
	ofFill();
	ofSetColor(0, 255, 0);	
	ofxSphere(ofGetWidth()/2.0, ofGetHeight()/2.0, 0, 500, 10, 500);
	ofSetColor(0, 255, 255);	
	glTranslated(ofGetWidth()/2.0, ofGetHeight()/2.0, 0);
	
	drawGeneric();
	
	glColor3f(255, 255, 255);	
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	glDisable(GL_DEPTH_TEST);
	
	ofNoFill();
	ofSetColor(255, 255, 255,255);
	ofxLightsOff();
	glPopMatrix();
	ofRect(0, 0, w, h);
	ofDrawBitmapString("Robot Visualizer", x+10, y+15);
	glPopMatrix();
}


void ofxIndustrialRobotVisualizer::drawtop(int x, int y, int w, int h){	
	topView.setPosAndSize(x,y,w, h);
	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	float scale = 0.09; // Same as to zoom
	ofxVec3f translate = ofxVec3f((ofGetWidth()/2.0)/mult, ofGetHeight()/2.0,0);
	
	if(topView.click){
		float cx = -(1.0- 2*((float)app->mouseX - x)/w);
		float cy = 1.0- 2*((float)app->mouseY - y)/h;
		float a1 = -atan2(cx,cy);
		float a2 = atan2(controller->targetPosition.x,controller->targetPosition.z)-HALF_PI;
		//		controller->targetPosition = ofxVec3f(((cx*ofGetWidth()/mult)/scale)/2.0, controller->targetPosition.y, (cy*ofGetHeight()/scale)/2.0);
		ofxVec3f target = ofxVec3f((float)((cx*ofGetWidth()/mult)/scale)/2.0,(float) controller->targetPosition.y, -(float)(cy*ofGetHeight()/scale)/2.0);
		controller->mousePressed(target);
		topView.click = false;
	}
	glPushMatrix();
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0.0, (float)ofGetWidth(), 0.0, (float)ofGetHeight(), 10.0, 2500);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1000,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
	
	glViewport(x, ofGetHeight()- y-h, w, h);
	ofFill();
	ofSetColor(255, 255, 0);
	
	glScaled(1.0*mult, 1.0, 1.0);	
	glTranslated(0, -ofGetHeight(), 0);
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofSetRectMode(1);
	glTranslated(translate.x,translate.y, 0);
	glScaled(scale, scale, scale);
	
	glRotated(-90, 1.0, 0, 0.0);
	
	drawGeneric();
	
	ofSetRectMode(0);
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Top view", x+10, y+15);
	
	glPopMatrix();	
}

void ofxIndustrialRobotVisualizer::drawside(int x, int y, int w, int h){	
	sideView.setPosAndSize(x,y,w, h);
	
	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	float scale = 0.17; // Same as to zoom
	ofxVec3f translate = ofxVec3f((ofGetWidth()/2.0)/mult-200, ofGetHeight()/2.0+200,0);
	
	if(sideView.click){
		float cx = ((float)app->mouseX - x)/w;
		float cy = ((float)app->mouseY - y)/h;
		float a = atan2(controller->targetPosition.x,controller->targetPosition.z)-HALF_PI;
		ofxVec3f target = ofxVec3f((float)((cx*ofGetWidth()/(float)mult)-translate.x)/scale,(float) -((cy*ofGetHeight())-translate.y)/scale, 0.0);
		target.rotateRad(a, ofxVec3f(0.0,1.0,0.0));
		controller->mousePressed(target);
		
		sideView.click = false;
	}
	
	glPushMatrix();
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(0.0, (float)ofGetWidth(), 0.0, (float)ofGetHeight(), 10.0, 2500);
	
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1000,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
	
	glViewport(x, ofGetHeight()- y-h, w, h);
	
	ofFill();
	ofSetColor(255, 255, 0);
	
	
	glScaled(1.0*mult, 1.0, 1.0);	
	glTranslated(0, -ofGetHeight(), 0);
	
	glPushMatrix();
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, ofGetWidth()/mult, ofGetHeight());
	ofSetRectMode(1);
	
	glTranslated(translate.x, translate.y, 0);
	glScaled(scale, scale, scale);
	glRotated(-helper->getRotationAngle(0), 0.0,1.0,0.0); // Rotate it around the axis
			ofSetColor(0, 255, 0,255);
/*	for(int i=0;i<3000;i+=100){
		for(int u=0;u<3000;u+= 100){
			if(controller->legalPosition(ofxVec3f(i,u,0), controller->targetDir,0.0) || controller->legalPosition(ofxVec3f(i,u,0), controller->targetDir,1.0))
				ofRect(i, -u, 100, 100);
		}
	}*/
	drawGeneric();
	
	ofFill();
	glRotated(helper->getRotationAngle(0), 0.0,1.0,0.0); // Rotate it around the axis
	ofSetColor(255, 255, 255);
	ofLine(-1000, 0, 1000, 0);
	ofLine(0,-1000, 0, 1000);
	ofSetRectMode(0);	
	
	glPopMatrix();	
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Side view", x+10, y+15);
	
	glPopMatrix();	
	
}


void ofxIndustrialRobotVisualizer::drawGraphs(int x, int y, int w, int h){
	float aspect = (float)ofGetWidth()/ofGetHeight(); //1.333...
	float aspect2 = (float)w/h; //1.0000
	float mult = (float)aspect/aspect2; //Factor to multiply x coordinates with
	
	glPushMatrix();
	glTranslated(x, y, 0);
	
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofRect(0, 0, w, h);
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	
	
	glBegin(GL_LINE_STRIP);
	int start = helper->getCoreData()->armSpeedHistory.size() - w;
	if(start < 0)
		start = 0;
	for(int i=start;i<helper->getCoreData()->armSpeedHistory.size();i++){
		glVertex3d(i-start, h-helper->getCoreData()->armSpeedHistory[i].length()/10.0,0);
	}
	
	glEnd();
	
	glColor3f(255, 255, 0);
	glBegin(GL_LINE_STRIP);
	start = helper->getCoreData()->armAccHistory.size() - w;
	if(start < 0)
		start = 0;
	for(int i=start;i<helper->getCoreData()->armAccHistory.size();i++){
		glVertex3d(i-start, h-helper->getCoreData()->armAccHistory[i]*100.0,0);
	}
	
	glEnd();
	
	ofDrawBitmapString(ofToString(helper->getCoreData()->armSpeedHistory.size(), 0), x+10, y+30);
	
	glPopMatrix();
	
	ofNoFill();
	ofSetColor(255, 255, 255);
	ofRect(0, 0, w, h);
	ofDrawBitmapString("Robot Graphs", x+10, y+15);
	
}


void ofxIndustrialRobotVisualizer::drawArm(int i, int w, int h){
	float l = helper->getLength(i); //The length of the arm
	ofxVec3f axis = helper->getAxis(i); //The axis the arm rotates around
	ofxVec3f offset = helper->getOffset(i);
	float r = helper->getRotationAngle(i);
	glRotated(r, axis.x, axis.y, axis.z); // Rotate it around the axis
	glTranslated(0, l, 0);
	ofxBox(ofxVec3f(0,-l/2.0,0), ofxVec3f(w, l, h), ofxVec3f(0.0, 0.0, 0.0));
	glTranslated(-offset.y, offset.x, offset.z);	
}

void ofxIndustrialRobotVisualizer::drawArm0(){
	ofSetColor(255, 0, 0);
	drawArm(0, 400, 400);
}
void ofxIndustrialRobotVisualizer::drawArm1(){
	ofSetColor(255, 255, 0);
	drawArm(1, 300, 300);
}

void ofxIndustrialRobotVisualizer::drawArm2(){
	ofSetColor(0, 255, 0);
	drawArm(2, 300, 300);
}

void ofxIndustrialRobotVisualizer::drawArm3(){
	ofSetColor(0, 255, 255);
	drawArm(3, 200, 200);
}

void ofxIndustrialRobotVisualizer::drawArm4(){
	ofSetColor(0, 0, 255);
	drawArm(4, 250, 250);
}