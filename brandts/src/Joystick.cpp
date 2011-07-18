/*
 *  InteractionDesign.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Joystick.h"
#include "ButtonMatrixMode.h"


void Joystick::init(myData * _data, QVBoxLayout * _boxLayout)
{
	
	addBaseGui(_boxLayout);
	
	data = _data;
	nameid = "Joystick";
	
	serial.setVerbose(false);
	connected = serial.setup("/dev/tty.usbserial-A5002W0F", 115200);	
	
	syncstate = 0;
	dptr = 0;
	flag1 = flag2 = 0;
	//	hasReceivenLastPosition = false;
	//	goodCounter = 0;
	errorCounter = 0;
	joystick[0] = 0;
	joystick[1] = 0;
	joystick[2] = 0;
	button8down = 0;
	emergencyDown = 0;
	
}


void Joystick::update()
{
	if(connected){
	 	serial.writeByte('0');
		
		char recv;
		bool somethingIn = false;
		while(serial.available())
		{
			recv = serial.readByte();
			recvChar(recv);
			somethingIn = true;
		}
		
		if(somethingIn && ofGetElapsedTimeMillis() > 4000){
			
			for(int i=0;i<16;i++){
				//			cout<<i<<"  "<<returnedFlags[i]<<endl;		
			}
			//cout<<returnedFlags[8]<<endl;
			
			if(!returnedFlags[8]){
				if(!button8down){
				button8down ++;
				//if(button8down == 1){
					robot->unlockMotors->toggle();	
				}
//				}
			} else {
				button8down = 0;
			}
			
			/*	if(!returnedFlags[7]){
			 cout<<"RIIING "<<ofGetElapsedTimeMillis()<<endl;
			 }*/
			if(returnedFlags[5]){
				emergencyDown ++;
				if(emergencyDown == 10){
				robot->unlockMotors->setChecked(false);
					robot->industrialRobot->panic("Emergency stop on joystick");
				}
			} else
				emergencyDown = 0;
			
			if(returnedFlags[4] && !returnedFlags[2]){
				//		if(robot->industrialRobot->thread.lock()){
				if(robot->industrialRobot->thread.controller->input == ofxIndustrialRobotController::ManualPosition){
					if(returnedFlags[3]){
						robot->manualPosition[0]->setValue(robot->manualPosition[0]->value() - joystick[0]*0.005) ;
						robot->manualPosition[1]->setValue(robot->manualPosition[1]->value() + joystick[1]*0.005) ;				
						robot->manualPosition[2]->setValue(robot->manualPosition[2]->value() + joystick[2]*0.005) ;				
					} else {
						robot->manualPosition[3]->setValue(robot->manualPosition[3]->value() - joystick[0]*0.0001) ;
						robot->manualPosition[4]->setValue(robot->manualPosition[4]->value() + joystick[1]*0.0001) ;				
						robot->manualPosition[5]->setValue(robot->manualPosition[5]->value() + joystick[2]*0.0001) ;				
						
					}
				}
				/*	if(robot->industrialRobot->thread.controller->input == ofxIndustrialRobotController::Slider){
				 if(returnedFlags[3]){
				 robot->motorSlider[1]->setValue(robot->motorSlider[1]->value() - joystick[0]*0.005);
				 robot->motorSlider[2]->setValue(robot->motorSlider[2]->value() + joystick[1]*0.005);				
				 
				 //				robot->manualPosition[2]->setValue(robot->manualPosition[2]->value() + joystick[2]*0.005) ;				
				 } else {
				 robot->motorSlider[1]->setValue(robot->motorSlider[1]->value() - joystick[0]*0.05);
				 robot->motorSlider[2]->setValue(robot->motorSlider[2]->value() + joystick[1]*0.05);				
				 
				 }
				 cout<<robot->motorSlider[1]->value()<<"   "<<robot->motorSlider[2]->value()<<endl;
				 cout<<joystick[0]<<endl;
				 }*/
				//			robot->industrialRobot->thread.unlock();
				//	}
			}
		}
	}
	
}



void Joystick::draw()
{
	
	
}

void Joystick::recvChar(char c){
	//	if((int)syncstate != 1)
	//cout<<(int)syncstate<<endl;
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
					
					flag1 = INBUF[DATAGRAM_LENGTH-1];
					flag2 = INBUF[DATAGRAM_LENGTH-2];
					for(int i=0;i<3;i++){
						joystick[i] = ((float*)INBUF)[i];
						//cout<< ((float*)INBUF)[i]<<"   ";
					}
					//		cout<<endl;
					
					
					/*		for(int i =0;i<num_axis;i++){
					 //	cout<<inValues[i]<<endl;
					 inValues[i] = ((float*)INBUF)[i];
					 }*/
					//HAPPY
					//	printf("%02x\r\n",botStatus);
					for(int i=0;i<8;i++){				
						if(flag1 & 0x01<<i)
							returnedFlags[i] = true;
						else
							returnedFlags[i] = false;
					}
					for(int i=0;i<8;i++){				
						if(flag2 & 0x01<<i)
							returnedFlags[i+8] = true;
						else
							returnedFlags[i+8] = false;
					}
					
					//		cout<<flag1<<"  "<<flag2<<endl;
					
					dptr=0;
					
					
					
				}
				else
				{
					//	goodCounter = 0;
					cout<<"ERROR "<<errorCounter<<endl;
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