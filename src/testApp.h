#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxIndustrialRobot.h"

class testApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
    
    ofxIndustrialRobot * industrialRobot;
    
    bool byteone[8];
	bool bytetwo[8];
	bool bytestatus[8];
    
    string motorStatusLabel[5];
    string panicStatus;

    int resetting;
    
    bool unlockMotors;


};

#endif
