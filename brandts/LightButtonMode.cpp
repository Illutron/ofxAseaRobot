/*
 *  LightButtonMode.cpp
 *  openframeworksQT
 *
 *  Created by Jonas Jongejan on 27/07/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "LightButtonMode.h"


LightButtonMode::LightButtonMode(Robot * _robot, myData * _data){
	name = "LightBulb";
	robot = _robot;
	robotData = _data;
	rate = 0;
	
	pressedTimer = 0;
	buttonPosition = ofxVec3f(1560.0, 834.1, 1119.2);
	buttonDir = ofxVec3f(0.73, -0.15, 0.67);


	init();
}


void LightButtonMode::init(){
	controllerItem = robot->addRobotController("LightBulb", CONTROL_MODE_GRAVITY);
}

void LightButtonMode::activate(){
	robot->setRobotController(controllerItem);
}
bool LightButtonMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}

void LightButtonMode::step(){
//	rate += ofRandom(0,0.07);	

	if(isActive()){	
		ofxVec3f safeDist = buttonPosition + ofxVec3f(-10,0,-10);

		if(pressedTimer < 100){	
			if(!foundToSafePos){
				controllerItem->locked = false;
				robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);
				if(robot->industrialRobot->isRobotReady(10)){
					foundToSafePos = true;
				}
			} else {
				controllerItem->locked = true;
				pressedTimer ++;
				robot->industrialRobot->setGravityTarget(buttonPosition, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);
			}
		} else {
			foundToSafePos = false;
			controllerItem->locked = false;
			if((robot->industrialRobot->getCurrentTarget() - buttonPosition).length() < 7.0){
				controllerItem->locked = true;
				robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);
			} else {
				pressedTimer = 0;
				rate = 0;
			}
		}
	} else {
	}
	
	
}
