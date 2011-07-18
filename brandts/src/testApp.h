#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "ofxEasyGui.h"

#include "Data.h"
#include "Defines.h"
#include "Controller.h"
#include <QTabWidget>


class testApp : public ofBaseApp{

	public:
		void setup();
		void setup(QMainWindow * tab, QComboBox * viewCombo);
		void update();
		void draw();
		void draw(int drawPlugin);
	
	float * left;
	float * right;

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	void saveSettings(QSettings * settings);
void loadSettings(QSettings * settings);
		
		
		Controller * controller;
		myData * data;
	unsigned long lastSave;
	QSettings * settings;
	QMainWindow * window;
	void audioReceived 	(float * input, int bufferSize, int nChannels);

	
	
	

	

};

#endif
