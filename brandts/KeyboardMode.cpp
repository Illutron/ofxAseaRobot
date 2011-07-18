/*
 *  LightButtonMode.cpp
 *  openframeworksQT
 *
 *  Created by Jonas Jongejan on 27/07/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "KeyboardMode.h"


KeyboardMode::KeyboardMode(Robot * _robot, myData * _data){
	name = "Keyboard";
	robot = _robot;
	robotData = _data;
	rate = 0;
	
	pressedTimer = 0;
	buttonQ = ofxVec3f(1487.6, 666.2, 1038.6);
	buttonP = ofxVec3f(1376.6,597.2,1143.0);
	buttonA = ofxVec3f(1482.6, 646.2, 1032.6);
	buttonZ = ofxVec3f(1479.6, 628.2, 1022.6);
	buttonDir = ofxVec3f(0.77, -0.44, 0.62);
	curButton = 0;
	foundMasterSafePos = false;
	curRow = 0;
	init();
}


void KeyboardMode::init(){
	controllerItem = robot->addRobotController("Keyboard", CONTROL_MODE_GRAVITY);
}

void KeyboardMode::activate(){
	robot->setRobotController(controllerItem);
}
bool KeyboardMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}

void KeyboardMode::step(){
//	rate += ofRandom(0,0.09);	
	
	char keys[]  = "qwertyuiopåasdfghjklæøzxcvbnm,. ";
	string s = "im a robot. ";
	if(isActive()){	
		if(!foundMasterSafePos){
			ofxVec3f safeDist = buttonQ + ofxVec3f(-30,0,-30);
			robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);

			if(robot->industrialRobot->isRobotReady(5.0)){
				foundMasterSafePos = true;
			}

		} else {
			int button = strcspn(keys,((string)""+s[curButton]).c_str());
			int row = 0;
			if(button == 34){
				row = 3;
				button = 5;
			}else if(button > 24){
				row = 2;
				button -= 25;
			}
			else if(button > 11){
				row = 1;
				button -= 12;
			} 
			ofxVec3f dir = buttonP - buttonQ;
			dir /= 9.0;		
			ofxVec3f buttonPosition = buttonQ + dir*button;
			if(row == 1){
				buttonPosition += buttonA - buttonQ;
			}
			if(row == 2){
				buttonPosition += buttonZ - buttonQ;
			}
			if(row == 3){
				buttonPosition += buttonA - buttonQ;
				buttonPosition += buttonZ - buttonQ;
				
				
			}
			
			ofxVec3f safeDist = buttonPosition + ofxVec3f(-14,0,-5);
			if(pressedTimer == 0){	
				if(!foundToSafePos){
					controllerItem->locked = false;
					robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);
					if(robot->industrialRobot->isRobotReady(20)){
						foundToSafePos = true;
					}
				} else {
					controllerItem->locked = true;
					robot->industrialRobot->setGravityTarget(buttonPosition, buttonDir,0.1, 1.0, ofxIndustrialRobotDefines::Down);
					if(robot->industrialRobot->isRobotReady(2.0)){
						pressedTimer = 1;
					}
				}
			} else {
				foundToSafePos = false;
				controllerItem->locked = false;
				if((robot->industrialRobot->getCurrentTarget() - buttonPosition).length() < 3.0){
					controllerItem->locked = true;
					robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.1, 1.0, ofxIndustrialRobotDefines::Down);
				} else {
					pressedTimer = 0;
					curButton ++;
					if(curButton > s.length()-1){
						curButton = 0;
						rate = 0;
					}
					//	curButton = int( ofRandom(0, 10));
					//	curRow = int(ofRandom(0, 3));
				}
			}
		}
	} else {
	}
	
	
}
