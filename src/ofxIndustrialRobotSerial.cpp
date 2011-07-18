#include "ofxIndustrialRobotSerial.h"



ofxIndustrialRobotSerial::ofxIndustrialRobotSerial(){
	serial.setVerbose(false);
	connected = serial.setup("/dev/tty.usbserial-A7007bv1", 115200);	
	
	for(int i=0;i<5;i++){
		inValues[i] = 0;
		outValues[i] = 0;
	}
	syncstate = 0;
	dptr = 0;
	botStatus = 0;
	hasReceivenLastPosition = false;
	errorCounter = 0;
    
    for(int i=0;i<8;i++){
        sendFlags[0][i] = false;
        sendFlags[1][i] = false;
    }
    
    resetting = 0;
}

void ofxIndustrialRobotSerial::start(){
	startThread(true, false);   // blocking, verbose
}




void ofxIndustrialRobotSerial::sendValues(){
	bool ok = true;
	if(connected){
		for(int i=0;i<num_axis;i++){
			if(outValues[i] != outValues[i]){ //Ehh?
				ok = false;
			}
			if(outValues[i] > 5000){
				outValues[i] = 5000;	
			}
			if(outValues[i] < -5000){
				outValues[i] = -5000;	
			}
			((float*)outbuf)[i]=outValues[i];
		}
        
        unsigned char b1=0;
        for(int i=0;i<8;i++){
            if(sendFlags[0][i]){
                b1 |= 0x01<<i;
            }
        }
        unsigned char b2=0;
        for(int i=0;i<8;i++){
            if(sendFlags[1][i]){
                b2 |= 0x01<<i;
            }
        }

        outbuf[DATAGRAM_LENGTH-4]='*';
        outbuf[DATAGRAM_LENGTH-3] = b1;
        outbuf[DATAGRAM_LENGTH-2] = b2;
		
		if(ok){            
            for(int i=0;i<DATAGRAM_LENGTH;i++){
                sendingoutbuf[i] = outbuf[i];
            }            
            serial.writeBytes(sendingoutbuf, DATAGRAM_LENGTH);
		}
	}
	
}

void ofxIndustrialRobotSerial::step(){
	if(connected){
		char recv;
		while(serial.available())
		{
			recv = serial.readByte();
			recvChar(recv);
		}
		//serial.flush(true, false);
		if(hasReceivenLastPosition){
			sendValues();
		}
		//	cout<<"In: "<<inValues[1]<<endl;
        
        if(resetting > 0){
			switch (resetting) {
				case 1: //Reset power
                    setUnlockFlag(false);
					resetting++;
					setResetPowerFlag(true);
					ofSleepMillis(20);
					break;                    
				case 2: //Disable reset power
                    setResetPowerFlag(false);              
					resetting++;
					break;		
				case 3: //Reset 1
					if(resetMotorFlag(1)){
						if(motorFlag(1)){ //Check if ok
							resetting ++;
						}
					} else {
                        setResetMotorFlag(1, true);
                        setUnlockFlag(true);
					}
					break;
				case 4: //Nothing
					resetting++;
					break;
				case 5: //reset 3 & 4
					if(resetMotorFlag(3)){
						if(motorFlag(3) && motorFlag(4)){
							resetting ++;
						}
					} else {
						setResetMotorFlag(3, true);
						setResetMotorFlag(4, true);
					}					
					break;
				case 6: //Reset 2
					if(resetMotorFlag(2)){
						if(motorFlag(2)){
							resetting ++;
						}
					} else {
						setResetMotorFlag(2, true);
                        //		ofSleepMillis(10);
					}					
					break;
				case 7:
					if(resetMotorFlag(0)){
						if(motorFlag(0)){
							resetting ++;
						}
					} else {
						setResetMotorFlag(0, true);
					}					
					break;
				case 8:
					for(int i=0;i<5;i++){
						setResetMotorFlag(i, false);
					}
					//industrialRobot->thread.controller->gotoResetPosition();					
					for(int i=0;i<5;i++){
						//motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation*100.0);				
					}
					resetting = 0;
					break;
				default:
					break;
			}
            
		}
	}
	
}

void ofxIndustrialRobotSerial::recvChar(char c){
	//	if((int)syncstate != 1)
	//	cout<<(int)syncstate<<endl;
	//cout<<(unsigned char)c<<endl;
	switch(syncstate)
	{
        case 0: //unsync
        {
			if(c=='*')
			{
				syncstate=1;
				dptr=0;
			}
        };break;
        case 1: //confirm
        {
			dptr++;
			
			if((dptr==DATAGRAM_LENGTH)&&(c=='*'))
			{
				
				syncstate=2;
				dptr=0;
			}
			if(dptr > DATAGRAM_LENGTH){
				syncstate = 0;
				cout<<"Confirm Error"<<endl;
			}
        };break;
        case 2: //rollout
        {
			dptr++;
			if(dptr==3)
			{
				syncstate=3;
				dptr=0;
			}
        };break;
        case 3:
        {
			INBUF[dptr++]=c;
			if(dptr>=DATAGRAM_LENGTH)
			{
				if(INBUF[DATAGRAM_LENGTH-4]=='*')
				{		
                    
                    botStatus = INBUF[DATAGRAM_LENGTH-1];
                    for(int i =0;i<num_axis;i++){
                        //	cout<<inValues[i]<<endl;
                        inValues[i] = ((float*)INBUF)[i];
                    }
                    //HAPPY
                    if(!hasReceivenLastPosition){
                        for(int i =0;i<num_axis;i++){
                            initValues[i] = inValues[i];
                        }
                        hasReceivenLastPosition = true;
                        
                    }
                    //	printf("%02x\r\n",botStatus);
                    for(int i=0;i<8;i++){				
                        if(botStatus & 0x01<<i)
                            returnedFlags[i] = true;
                        else
                            returnedFlags[i] = false;
                    }
                    
                    if(botStatus != 0x1f){
						//	printf("%0x2f:%0x2f:%0x2f:%0x2f\r\n",INBUF[DATAGRAM_LENGTH-4],INBUF[DATAGRAM_LENGTH-3],INBUF[DATAGRAM_LENGTH-2],INBUF[DATAGRAM_LENGTH-1]);
                    }
					dptr=0;
                    
                    
					
				}
				else
				{
                    //		goodCounter = 0;
					cout<<"Error in serialcommunication nr. "<<errorCounter<<endl;
					errorCounter++;
					if(INBUF[DATAGRAM_LENGTH-5]=='*'){
						cout<<"Lost one"<<endl;
					}
					if(INBUF[DATAGRAM_LENGTH-6]=='*'){
						cout<<"Lost two"<<endl;
					}
					syncstate=0;
				}
			}
        };break;
	}
}


bool ofxIndustrialRobotSerial::motorFlag(int n){
	return returnedFlags[n];
}
bool ofxIndustrialRobotSerial::emergencyFlag(){
	return returnedFlags[6];
}
bool ofxIndustrialRobotSerial::panicFlag(){
	return returnedFlags[7];	
}

bool ofxIndustrialRobotSerial::lockFlag(){
	return !outbuf[24-3];
}

bool ofxIndustrialRobotSerial::resetMotorFlag(int n){
    return sendFlags[1][n];
}

bool ofxIndustrialRobotSerial::speedLimitFlag(){
    return sendFlags[0][0];
}
bool ofxIndustrialRobotSerial::resetPowerFlag(){
    return sendFlags[0][5];
}
bool ofxIndustrialRobotSerial::unlockFlag(){
    return  sendFlags[0][7];
}

void ofxIndustrialRobotSerial::setValue(int n, float val){
	outValues[n] = val;
}

void ofxIndustrialRobotSerial::setSpeedLimitFlag(bool flag){
    sendFlags[0][0] = flag;
}

void ofxIndustrialRobotSerial::setResetPowerFlag(bool flag){
    sendFlags[0][5] = flag;
}

void ofxIndustrialRobotSerial::setUnlockFlag(bool flag){
    sendFlags[0][7] = flag;
}

void ofxIndustrialRobotSerial::setResetMotorFlag(int motor, bool flag){
    sendFlags[1][motor] = flag;
}



void ofxIndustrialRobotSerial::threadedFunction(){
	
	while( isThreadRunning() != 0 ){
		if( lock() ){
			//				count++;
            //			lastStep = ofGetElapsedTimeMillis();
			step();
			unlock();
			ofSleepMillis(8);
			
			
		}
	}
	
}
