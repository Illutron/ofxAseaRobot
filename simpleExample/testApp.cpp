#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    industrialRobot = new ofxIndustrialRobot(this);
    industrialRobot->setInput(ofxIndustrialRobot::Gravity);
    industrialRobot->gotoStartPosition();    
}

//--------------------------------------------------------------
void testApp::update(){
    industrialRobot->setGravityTarget(ofxVec3f(1487.6, 666.2, 1038.6), ofxVec3f(0.77, -0.44, 0.62));
    
    
	if(industrialRobot->getEmergencyStop()){
        //unlockMotors->setChecked(false);	
	}
	
	//tjeckCurrentRobotController();	
	
	//Check for changes in control mode (tilf¿jet af mads)
/*	for(int i = 0; i < 4;i++)
	{
		if(inputRadio[i]->isChecked() && controlMode != i)
		{
			setControlMode(i);
		}
	}*/
	
	
	/*if(industrialRobot->thread.lock()){
		if(loadXmlButton->isDown()){
			industrialRobot->thread.converter->loadXml();
			loadXmlButton->setDown(false);
		}
		industrialRobot->thread.unlock();
	}*/
    
	unsigned char b1=0;
	for(int i=0;i<8;i++){
		if(byteone[i]){
			b1 |= 0x01<<i;
		}
	}
	
	unsigned char b2=0;
	for(int i=0;i<8;i++){
		if(bytetwo[i]){
			b2 |= 0x01<<i;
		}
	}
	
	if(industrialRobot->thread.serial->lock()){
		industrialRobot->thread.serial->outbuf[24-3] = b1;
		//	cout<<(unsigned char )b1<<"  "<<(unsigned char )b2<<endl;
		industrialRobot->thread.serial->outbuf[24-2] = b2;
		
		for(int i=0;i<8;i++){				
			if(industrialRobot->thread.serial->returnedFlags[i])
				bytestatus[i] = true;
			else
				bytestatus[i] = false;
		}
		
		if(resetting > 0){
			switch (resetting) {
				case 1:
					unlockMotors = false;
					resetting++;
					byteone[5] = true;
					ofSleepMillis(20);
					break;
                    
				case 2:
					byteone[5] = false;                    
					resetting++;
					break;		
				case 3:
					if(bytetwo[1]){
						if(bytestatus[1]){
							resetting ++;
						}
					} else {
						bytetwo[1] = true;
						unlockMotors = (true);
					}
					break;
				case 4:
					resetting++;
					break;
				case 5:
					if(bytetwo[3]){
						if(bytestatus[3] && bytestatus[4]){
							resetting ++;
						}
					} else {
						bytetwo[3] = true;
						bytetwo[4] = true;
					}					
					break;
				case 6:
					if(bytetwo[2]){
						if(bytestatus[2]){
							resetting ++;
						}
					} else {
						bytetwo[2] = true;
                        //		ofSleepMillis(10);
					}					
					break;
				case 7:
					if(bytetwo[0]){
						if(bytestatus[0]){
							resetting ++;
						}
					} else {
						bytetwo[0] = true;
					}					
					break;
				case 8:
					for(int i=0;i<5;i++){
						bytetwo[i] = false;
					}
					industrialRobot->thread.controller->gotoResetPosition();					
					for(int i=0;i<5;i++){
						//motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation*100.0);				
					}
					resetting = 0;
					break;
				default:
					break;
			}
            
		}
		
	/*	if(industrialRobot->thread.serial->connected){
			resetMotors->setEnabled(true);
			unlockMotors->setEnabled(true);
		} else {
			resetMotors->setEnabled(false);
			unlockMotors->setEnabled(false);			
		}*/
		
		for(int i=0;i<5;i++){
			string l;
			if(industrialRobot->thread.serial->connected){
				l = "Motor "+ofToString(i,0)+": ";
				if(!byteone[7]){
					l+= "LOCKED ";
				}
				if(bytetwo[i]){
					if(bytestatus[i]){
						l+= "Resetting done...";
					} else {
						l+= "Resetting...";	
					}
				} else if(bytestatus[i]){
					l+= "OK";
				} else {
					l+= "NOT OK";	
				}
			} else {
				l = " ";
			}
			motorStatusLabel[i] = l;
		}
		
		if(!industrialRobot->thread.serial->connected){
			panicStatus = "Status: Serial not connected";		
		}else if(bytestatus[7]){
			panicStatus = "Status: PANIC!!!";
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Panic!");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else if(bytestatus[6]){
			panicStatus = ("Status: EMERGENCY STOP");
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Emergency stop pressed. Release it, and unlock the robot.");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else {
			panicStatus = ("Status: OK");	
            //			warningShown = false;
		}
		
		/*if(resetMotors->isDown()){
			resetting  = 1;
		}*/
		
		/*if(unlockMotors && !byteone[7]){
			byteone[7] = true;
		} else if(!unlockMotors && byteone[7]){
			byteone[7] = (false);
		}
		*/
		
		industrialRobot->thread.serial->unlock();
	}	
	
	if(resetting > 0){
		ofSleepMillis(200);
        
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
   industrialRobot->visualizer->draw3d(0, 0, ofGetWidth()/2, ofGetHeight()/2);
   industrialRobot->visualizer->drawside( ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight()/2);
    
    ofSetColor(255, 255, 255,255);
    for(int i=0;i<8;i++){
        if(byteone[i]){
            ofNoFill();
        } else {
            ofFill();
        }
        ofRect(10+i*40, ofGetHeight()-(10+3*40), 30, 30);

        if(bytetwo[i]){
            ofNoFill();
        } else {
            ofFill();
        }
      
        ofRect(10+i*40, ofGetHeight()-(10+2*40), 30, 30);
        if(bytestatus[i]){
            ofNoFill();
        } else {
            ofFill();
        }
        ofRect(10+i*40, ofGetHeight()-(10+1*40), 30, 30);
    }
    for(int i=0;i<5;i++){
        ofDrawBitmapString(motorStatusLabel[i], 10+200*i, ofGetHeight()-(10+4*40));
    }
            ofDrawBitmapString(panicStatus, 10, ofGetHeight()-(10+5*40));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'a'){
        resetting = 1;
    }
    
    if(key == '1'){
        byteone[0] = !byteone[0];
    }
    if(key == '2'){
        byteone[1] = !byteone[1];
    }

    if(key == '3'){
        byteone[2] = !byteone[2];
    }

    if(key == '4'){
        byteone[3] = !byteone[3];
    }

    if(key == '5'){
        byteone[4] = !byteone[4];
    }
    if(key == '6'){
        byteone[5] = !byteone[5];
    }
    if(key == '7'){
        byteone[6] = !byteone[6];
    }

    if(key == '8'){
        byteone[7] = !byteone[7];
    }
    
    if(key == 'q'){
        bytetwo[0] = !bytetwo[0];
    }
    if(key == 'w'){
        bytetwo[1] = !bytetwo[1];
    }
    
    if(key == 'e'){
        bytetwo[2] = !bytetwo[2];
    }
    
    if(key == 'r'){
        bytetwo[3] = !bytetwo[3];
    }
    
    if(key == 't'){
        bytetwo[4] = !bytetwo[4];
    }
    if(key == 'y'){
        bytetwo[5] = !bytetwo[5];
    }
    if(key == 'u'){
        bytetwo[6] = !bytetwo[6];
    }
    
    if(key == 'i'){
        bytetwo[7] = !bytetwo[7];
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

