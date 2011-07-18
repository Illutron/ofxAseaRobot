 #include "ofxIndustrialRobotSerial.h"



ofxIndustrialRobotSerial::ofxIndustrialRobotSerial(){
	serial.setVerbose(false);
	connected = serial.setup("/dev/tty.usbserial-A7007bv1", 115200);	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
	
	for(int i=0;i<5;i++){
		inValues[i] = 0;
		outValues[i] = 0;
	}
	syncstate = 0;
	dptr = 0;
	botStatus = 0;
	hasReceivenLastPosition = false;
	//goodCounter = 0;
	errorCounter = 0;
}

void ofxIndustrialRobotSerial::start(){
	startThread(true, false);   // blocking, verbose
}

void ofxIndustrialRobotSerial::setValue(int n, float val){
	outValues[n] = val;
}


void ofxIndustrialRobotSerial::sendValues(){
	bool ok = true;
	if(connected){
		//((float*)outbuf)[3]=(float)(sin(ofGetElapsedTimeMillis()/1000.0)*TWO_PI);
		for(int i=0;i<num_axis;i++){
			if(outValues[i] != outValues[i]){
			//	cout<<"ARHRHRHIUAOSHDIUASHDIUH"<<endl;
				ok  =false;
			}
			if(outValues[i] > 5000){
				outValues[i] = 5000;	
			}
			if(outValues[i] < -5000){
				outValues[i] = -5000;	
			}
//			cout<<outValues[i] <<endl;
			((float*)outbuf)[i]=outValues[i];
		}
		
		if(ok){
		outbuf[DATAGRAM_LENGTH-4]='*';
		
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
