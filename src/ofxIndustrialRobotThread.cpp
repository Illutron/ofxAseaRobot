
#include "ofxIndustrialRobotThread.h"

void ofxIndustrialRobotThread::start(){
	startThread(true, false);
	serial->start();
	sendToRobot = false;
	
}

ofxIndustrialRobotThread::ofxIndustrialRobotThread(){
	lastTimeMillist = 0;
	framerate = 0;
	converter = new ofxIndustrialRobotMotorConverter();
	converter->armdata = &coreData;
	panic = false;
	panicMessage = "No message";
}

void ofxIndustrialRobotThread::threadedFunction(){
	while( isThreadRunning() != 0 ){
		if( lock() ){
			if(serial->connected && sendToRobot == false){
				if(serial->lock()){
					if(serial->hasReceivenLastPosition){
						/*while(!serial->hasReceivenLastPosition){
						 cout<<"Waiting for serial.."<<endl;
						 ofSleepMillis(1);
						 }*/
						for(int i=0;i<5;i++){
							cout<<"Init value "<<serial->inValues[i]<<"   "<<endl;				
							coreData.arms[i].rotation = converter->convertO(i,serial->initValues[i]);
							cout<<coreData.arms[i].rotation<<endl;
							sendToRobot = true;
						}
					}
					serial->unlock();
				}
			} else if(!serial->connected && sendToRobot == false){
				for(int i=0;i<5;i++){
					coreData.arms[i].rotation = converter->convertO(i,0);
					sendToRobot = true;
				}
			}
			
			
			
			framerate =(float) 1000.0/(ofGetElapsedTimeMillis() - lastTimeMillist);
			if(framerate == 1000){
				framerate = 0;
			}
			lastTimeMillist = ofGetElapsedTimeMillis();
			if(framerate > 0 && framerate < 400)
				step();
			unlock();
			
			int wait = 1000/120-(ofGetElapsedTimeMillis()-lastTimeMillist);
			if(wait < 0){
				wait = 0;
			}
			
			ofSleepMillis(wait);
			
		}
	}
}


void ofxIndustrialRobotThread::step(){
	if(serial->lock()){
		if(panic){
			unsigned char b1=0;
		//	b1 |= 0x01<<7;
			serial->outbuf[24-3] = b1;
		}
		if(sendToRobot){
			for(int i=0;i<5;i++){
				serial->setValue(i, converter->convertRotation(i));
			}
		}
		
		//		serial->sendMsg(coreData.arms[0].rotation);
		
		//cout<<serial->emergencyFlag()<<"   "<<serial->lockFlag()<<endl;
		if(!serial->emergencyFlag() && !serial->lockFlag()){
			controller->step(framerate);
		}
		serial->unlock();
	}
	
}