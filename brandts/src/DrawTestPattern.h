/*
 *  InteractionDesign.h
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "Robot.h"
#include "ofxIndustrialRobot.h"

class DrawTestPattern : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	
	
	
	myData * data;
	Robot * robot;
	RobotControllerItem * patternControllerItem;

	
	
	
};