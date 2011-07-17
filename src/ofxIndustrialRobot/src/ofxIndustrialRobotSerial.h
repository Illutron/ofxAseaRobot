#pragma once
#include "ofMain.h"
#include "ofxThread.h"
#include "ofxIndustrialRobotMotorConverter.h"
#define DATAGRAM_LENGTH 24

class ofxIndustrialRobotSerial : public ofxThread{
public:
	ofSerial	serial;
	
	
	ofxIndustrialRobotSerial();
	void step();
	
	bool		bSendSerialMessage;			// a flag for sending serial
	char		bytesRead[1];				// data from serial, we will be trying to read 3
	char		bytesReadString[4];			// a string needs a null terminator, so we need 3 + 1 bytes
	int			nBytesRead;					// how much did we read?
	int			nTimesRead;					// how many times did we read?
	float		readTime;					// when did we last read?	
	
	static const int num_axis = 5;
	float inValues[5];
	float initValues[5];

	float outValues[5];	
	bool returnedFlags[8];
	
	bool motorFlag(int n);
	bool emergencyFlag();
	bool panicFlag();
	bool lockFlag();
	
	void recvChar(char c);
	
	bool hasReceivenLastPosition;
	
	void setValue(int n, float val);
	void sendValues();
	
	
	
	bool connected;
	
	void start();
	
	void stop(){
		stopThread();
	}
	

	
	//--------------------------
	void threadedFunction();	
	unsigned char outbuf[DATAGRAM_LENGTH];
	unsigned char sendingoutbuf[DATAGRAM_LENGTH];
	
	unsigned int lastStep;
	
	int errorCounter;
private:
	unsigned char syncstate;
	unsigned char dptr;
	unsigned char INBUF[DATAGRAM_LENGTH];

	unsigned char botStatus;
	
};