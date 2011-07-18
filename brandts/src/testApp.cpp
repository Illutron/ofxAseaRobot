#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	
	
}



void testApp::setup(QMainWindow * tab, QComboBox * viewCombo)
{
	ofSetFrameRate(40);	
	data = new myData(this);
	
	controller = new Controller(data,tab,viewCombo);	
	ofxLightsOff();


	if(USE_SOUND_INPUT)
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
	
	

}

//--------------------------------------------------------------
void testApp::update(){

	
	
	controller->update();
	
	
}

//--------------------------------------------------------------
void testApp::draw(){

}


void testApp::draw(int drawPlugin){
	
	ofBackground(0, 0, 0);
	controller->draw(drawPlugin);
	
	
	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 0), 15, 30);

	
}

void testApp::saveSettings(QSettings * settings)
{
	controller->saveSettings(settings);

}
void testApp::loadSettings(QSettings * settings)
{
	controller->loadSettings(settings);

}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == 'S'){
	//	vidGrabber.videoSettings();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	/*if(mouseDown){
		robot.controller->targetPosition = ofxVec3f(x, y, 0);
	}*/
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	mouseDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	mouseDown = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
//--------------------------------------------------------------

//AUDIO HANDLE FOR SOUND INPUT PLUGIN


void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		data->soundLeft[i] = input[i*2];
		data->soundRight[i] = input[i*2+1];
	}
//	bufferCounter++;
	data->newSound = true;
	
	
}
