/*
 *  IdleMode.cpp
 *  openframeworksQT
 *
 *  Created by Jonas Jongejan on 27/07/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "IdleMode.h"

IdleMode::IdleMode(Robot * _robot, myData * _data){
	name = "Idle";
	robot = _robot;
	robotData = _data;
	rate = 20;
	idlePosition = ofxVec3f(1648, 877, 46);

	init();
}

void IdleMode::init(){
	controllerItem = robot->addRobotController("IdleMode", CONTROL_MODE_GRAVITY);
}
void IdleMode::activate(){
	robot->setRobotController(controllerItem);
}
bool IdleMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}
void IdleMode::step(){
	if( rate > 95)
		rate = 95;
	
	if(rate < 0)
	{
		rate = 0;
	}
	
	if(!motionDetector->isAreaActive(0, 5, 4,5, 0.4) && !motionDetector->isAreaActive(5, 0, 5, 3, 0.4) )
	{
		rate +=0.05;
		
		//	cout << "motion to drawingmode";
	}
	else
	{
		rate -= 10;
	}
	if(isActive()){
		
		idleDirection = ofxVec3f(0.7,-1,sin(ofGetElapsedTimeMillis()/2000.0)).normalized();
		if( !robot->industrialRobot->setGravityTarget(idlePosition, idleDirection, 0.1, 1, ofxIndustrialRobotDefines::Up)){
			cout<<"Could not add facetracker"<<endl;
		}
		
	}
}