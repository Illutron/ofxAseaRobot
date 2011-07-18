

#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "Robot.h"
#include "FaceTracker.h"
#include "ofxIndustrialRobot.h"
#include "RobotControllerItem.h"

#include "Mode.h"
#include "LightButtonMode.h"
#include "IdleMode.h"
#include "KeyboardMode.h"

#define DATAGRAM_LENGTH 16



class Joystick : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	void recvChar(char c);

	
	ofSerial	serial;
	
	bool connected;

//	ofxIndustrialRobot * industrialRobot;
	myData * data;
	Robot * robot;
	
	bool returnedFlags[16];
	
	float joystick[3];

	int button8down;
		int emergencyDown;
	unsigned char outbuf[DATAGRAM_LENGTH];
	unsigned int lastStep;
	int errorCounter;
private:
	unsigned char syncstate;
	unsigned char dptr;
	unsigned char INBUF[DATAGRAM_LENGTH];
	
	unsigned char flag1, flag2;
	
	
		
};

