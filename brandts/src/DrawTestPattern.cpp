
/*
 *  InteractionDesign.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "DrawTestPattern.h"


void DrawTestPattern::init(myData * _data, QVBoxLayout * boxLayout)
{
	
	addBaseGui(boxLayout);
	data = _data;
	nameid = "TestPattern";
	
	patternControllerItem = robot->addRobotController("Draw Pattern", CONTROL_MODE_TIMELINE);
	
	
}

void DrawTestPattern::update()
{
	
	//	if(patternControllerItem->isActive() &&  robot->industrialRobot->timelineCuesLeft() < 4){
	if(patternControllerItem->isActive()){
		ofxVec3f target = (ofxVec3f(1940,820,-30));
		
//		if( !robot->industrialRobot->setGravityTarget(target, ofxVec3f((sin(ofGetElapsedTimef()/4.0)+1.5)/2.0,-0.5-0.05,2.0*(sin(ofGetElapsedTimef()/4.0))).normalized(), 1.0, 1.0, ofxIndustrialRobotDefines::Down)){
		ofxVec3f dir = ofxVec3f(1.0,0,0);
//		dir = dir.rotated(60.0*sin(ofGetElapsedTimef()/20.0), ofxVec3f(0,1,0));
		dir = dir.rotated(50.0*sin(ofGetElapsedTimef()/20.0), ofxVec3f(0,0,1));
		if( !robot->industrialRobot->setGravityTarget(target, dir, 1.0, 1.0, ofxIndustrialRobotDefines::Down)){
			cout<<"No go"<<endl;
		}
		float speed = 50;
		
		/*		ofxVec3f target = ofxVec3f(1569.0,700.824,0);
		 if( !robot->industrialRobot->setGravityTarget(target, ofxVec3f(sin(ofGetElapsedTimef()/4.0),-1.0,sin(ofGetElapsedTimef()/2.0)).normalized())){
		 cout<<"Could not add facetracker"<<endl;
		 }
		 */		
		ofxVec3f s = ofxVec3f(1812.4,1467.9,688.1);
		
		ofxVec3f r = (ofxVec3f(1722.4,1507.9,-561.9) - s).normalized();
		
		/*		if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1812.1,1460.0,647.4), ofxVec3f(1.0,-0.2,0),false,false)){
		 cout<<"Could not add 1"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1612.1,1515.0,-1092.6), ofxVec3f(1.0,-0.2,0),false,false)){
		 cout<<"Could not add 2"<<endl;
		 }*/
		
		
		/*	for(int i=0;i<7;i++){
		 if(!robot->industrialRobot->addPositionCue(speed*4, r*100*i + s - ofxVec3f(50,0,0), ofxVec3f(1.0,-0.2,0),true,false)){
		 cout<<"Could not add 2"<<endl;
		 }	
		 if(!robot->industrialRobot->addPositionCue(speed*1.2, r*100*i + s , ofxVec3f(1.0,-0.2,0),false,false,true)){
		 cout<<"Could not add 2"<<endl;
		 }	
		 if(!robot->industrialRobot->addPositionCue(speed*3, r*100*i + s - ofxVec3f(50,0,0), ofxVec3f(1.0,-0.2,0),true,false)){
		 cout<<"Could not add 2"<<endl;
		 }			
		 
		 
		 }*/
		
		
		//	}
		/*	if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,700.824,400.0), ofxVec3f(0.0,-1.0,0),false,false)){
		 cout<<"Could not add 2"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,700.824,-400.0), ofxVec3f(0.0,-1.0,0),false,false)){
		 cout<<"Could not add 3"<<endl;
		 }
		 *//*	if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,300.824,-400.0), ofxVec3f(0.0,-1.0,0),false,true)){
		  cout<<"Could not add 4"<<endl;
		  }*/
		/*	if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,1200.824,000.0), ofxVec3f(0.0,-1.0,0),false,false)){
		 cout<<"Could not add 4"<<endl;
		 }*/
		
		/*	if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,1200.824,000.0), ofxVec3f(0.0,-1.0,0),true,false)){
		 cout<<"Could not add 4"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,1200.824,000.0), ofxVec3f(1.0,-1.0,0),true,false)){
		 cout<<"Could not add 4"<<endl;
		 }*/
		/*
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1169.0,800.824,-400.0), ofxVec3f(0.0,-1.0,0),false)){
		 cout<<"Could not add 5"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,800.824,-400.0), ofxVec3f(0.0,-1.0,0),false)){
		 cout<<"Could not add 6"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1569.0,800.824,0.0), ofxVec3f(0.0,-1.0,0),false)){
		 cout<<"Could not add 7"<<endl;
		 }
		 if(!robot->industrialRobot->addPositionCue(speed,  ofxVec3f(1169.0,800.824,0.0), ofxVec3f(0.0,-1.0,0),false)){
		 cout<<"Could not add 8"<<endl;
		 }*/
		
	}
	
}



void DrawTestPattern::draw()
{
	
	
}