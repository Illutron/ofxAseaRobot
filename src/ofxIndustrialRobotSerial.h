#pragma once
#include "ofMain.h"
#include "ofxIndustrialRobotMotorConverter.h"
#define DATAGRAM_LENGTH 24

/*
 Description of the bytes:

 byte 0:
    0: No speed limit
    1: bit 1 (?)
    2: bit 2 (?)
    3: bit 3 (?)
    4: bit 4 (?)
    5: Reset power
    6: Run 0 (?)
    7: Run 1 (Unlock motors)
 
 byte 1: 
    0: Reset motor 0
    1: Reset motor 1
    2: Reset motor 2
    3: Reset motor 3
    4: Reset motor 4
    5: bit 5 (?)
    6: bit 6 (?)
    7: bit 7 (?)
 
 byte status:
    0: Axis 0 OK
    1: Axis 1 OK
    2: Axis 2 OK
    3: Axis 3 OK
    4: Axis 4 OK
    5: Bit 5 (?)
    6: Manual stop (emergency stop)
    7: Hardware error (panic)
 */
 
class ofxIndustrialRobotSerial : public ofThread{
public:
	ofxIndustrialRobotSerial();

    
	bool returnedFlags[8];
    bool sendFlags[2][8];

    bool hasReceivenLastPosition;

	bool motorFlag(int n);
	bool emergencyFlag();
	bool panicFlag();
	bool lockFlag();
    
    bool resetMotorFlag(int n);
	bool speedLimitFlag();
	bool resetPowerFlag();
	bool unlockFlag();
    
    void setSpeedLimitFlag(bool flag);
    void setResetPowerFlag(bool flag);    
    void setUnlockFlag(bool flag);    
    void setResetMotorFlag(int motor, bool flag);
    
    void step();
	void recvChar(char c);
	
	
	void setValue(int n, float val);
	void sendValues();
	
	
	bool connected;
	
	void start();
	
	void stop(){
		stopThread();
	}
	
    
    int resetting;
	
	//--------------------------
	void threadedFunction();	

    int inValues[5];
    float initValues[5];


private:
	unsigned char syncstate;
	unsigned char dptr;
	unsigned char INBUF[DATAGRAM_LENGTH];

	unsigned char botStatus;

    unsigned char sendingoutbuf[DATAGRAM_LENGTH];
	
	unsigned int lastStep;
	
	int errorCounter;
    
    ofSerial	serial;
    
    int outValues[5];
    
    static const int num_axis = 5;
    unsigned char outbuf[DATAGRAM_LENGTH];

    

};