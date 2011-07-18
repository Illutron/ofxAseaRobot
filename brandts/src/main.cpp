
#include "ofMain.h"
#include "testApp.h"
#include "ofAppQtWindow.h"



//========================================================================
int main( ){
	ofAppBaseWindow* pWindow = new ofAppQtWindow();
	//    ofAppBaseWindow* pWindow = new ofAppGlutWindow();
	
	ofSetupOpenGL(pWindow, 600,320, OF_WINDOW);			// <-------- setup the GL context
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());
	
	
	
}

