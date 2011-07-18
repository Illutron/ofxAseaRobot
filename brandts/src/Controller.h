/*
 *  Controller.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "BasePlugin.h"
#include "FaceTracker.h"
#include "Robot.h"
#include "InteractionDesign.h"
#include "DrawTestPattern.h"
#include "VideoGrabber.h"
#include "PaperTracker.h"
#include "SoundInput.h"
#include "Joystick.h"
#include "GrabTheDrawing.h"
#include "MotionDetector.h"

#include "Data.h"
#include <QGroupBox>
#include <QDockWidget>
class Controller {
public:
	Controller(myData * _data,QMainWindow * tab, QComboBox * viewCombo);
	myData * data;
	Robot * tmpRobot;
	void update();
	void draw(int drawPlugin);
	void addPlugin(BasePlugin * plugin,QMainWindow * tab,QComboBox * viewCombo);
	vector<BasePlugin*> plugins;	
	vector<QDockWidget*> docks;
	void loadSettings(QSettings * settings);
	void saveSettings(QSettings * settings);
};