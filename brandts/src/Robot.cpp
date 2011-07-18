/*
 *  Robot.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Robot.h"

void Robot::init(myData * _data,  QVBoxLayout * boxLayout)
{
	
	initFlag = true;
	addBaseGui(boxLayout);
	data = _data;
	industrialRobot = new ofxIndustrialRobot(data->baseApp);
	nameid = "Robot";
	
	QTabWidget *robotTab = new QTabWidget();
	
	QWidget * robotGeneralTab = new QWidget();
	QGridLayout *robotGeneralTabLayout = new QGridLayout();
	
	//
	//Motor control
	//
	
	QGroupBox * robotMotorArea = new QGroupBox("Motors");
	QGridLayout *robotMotorLayout = new QGridLayout;
	
	//	if(industrialRobot->thread.lock()){
	
	for(int i=0;i<5;i++){
		motorSlider[i] = new QSlider(Qt::Horizontal);
		//			motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation);
		//	motorSlider[i]->setFixedWidth(220);
		motorLabel[i] = new QLabel("");
		motorLabel[i]->setFixedWidth(55);
		motorLabel2[i] = new QLabel("");
		motorLabel2[i]->setFixedWidth(55);
		setValue[i] = new QPushButton("Set");
		robotMotorLayout->addWidget( new QLabel(("Motor "+ofToString(i, 0)).c_str()),i,0);	
		robotMotorLayout->addWidget(motorSlider[i],i,1);
		robotMotorLayout->addWidget(motorLabel[i],i,2);
		robotMotorLayout->addWidget(motorLabel2[i],i,3);
		robotMotorLayout->addWidget(setValue[i],i,4);
	} 
	loadXmlButton = new QPushButton("Load xml");
	robotMotorLayout->addWidget(loadXmlButton,6,0);
	
	variantSlider = new QSlider(Qt::Horizontal);
	variantSlider->setEnabled(false);
	variantSlider->setMinimum(0);
	variantSlider->setMaximum(1000.0);
	robotMotorLayout->addWidget( new QLabel("Variant"),5,0);	
	robotMotorLayout->addWidget(variantSlider,5,1);
	
	
	//	}
	robotMotorLayout->setAlignment(Qt::AlignTop);
	robotMotorArea->setLayout(robotMotorLayout);	
	robotMotorArea->setMinimumWidth(460);
	
	robotGeneralTabLayout->addWidget(robotMotorArea,0,0);	
	
	//
	//Mode/input selector
	//
	
	QWidget * robotModePositionArea = new QWidget();
	QGridLayout *robotModePositionLayout = new QGridLayout;	
	
	
	QGroupBox * robotModeArea = new QGroupBox("Mode");
	QGridLayout *robotModeLayout = new QGridLayout;	
	
	inputRadio[0] = new QRadioButton("Timeline", robotModeArea);
	inputRadio[1] = new QRadioButton("Manual Motor", robotModeArea);	
	inputRadio[1]->setChecked(true);
	inputRadio[2] = new QRadioButton("Gravity", robotModeArea);	
	inputRadio[3] = new QRadioButton("Manual position", robotModeArea);	
//	inputRadio[4] = new QRadioButton("Change Variant", robotModeArea);	

	setControlMode(CONTROL_MODE_MOTOR);
	
	
	robotModeLayout->addWidget(inputRadio[0],0,0);
	robotModeLayout->addWidget(inputRadio[1],1,0);
	robotModeLayout->addWidget(inputRadio[2],2,0);
	robotModeLayout->addWidget(inputRadio[3],3,0);	
//	robotModeLayout->addWidget(inputRadio[4],4,0);		
	//	robotModeArea->setMinimumWidth(460);
	robotModeArea->setLayout(robotModeLayout);
	robotModePositionLayout->addWidget(robotModeArea,0,0);	
	
	
	
	QGroupBox * robotPositionArea = new QGroupBox("Position");
	QGridLayout *robotPositionLayout = new QGridLayout;	
	
	robotPositionArea->setLayout(robotPositionLayout);
	robotModePositionLayout->addWidget(robotPositionArea,0,1);	
	
	int n = 0, l =0;
	for(int i=0;i<6;i++){
		manualPosition[i] = new QDoubleSpinBox();
		robotPositionLayout->addWidget(manualPosition[i],l,1+n);
		if(i>2){
			manualPosition[i]->setSingleStep(0.1);
		}
		n++;
		if(n>2){
			n = 0;
			l++;
		}
	}
	manualPosition[0]->setMinimum(-300.0);
	manualPosition[0]->setMaximum(300.0);	
	manualPosition[1]->setMinimum(00.0);
	manualPosition[1]->setMaximum(300.0);	
	manualPosition[2]->setMinimum(-300.0);
	manualPosition[2]->setMaximum(300.0);	
	for(int i=3;i<6;i++){
		manualPosition[i]->setMinimum(-1.0);
		manualPosition[i]->setMaximum(1.0);	
	}
	
	robotPositionLayout->addWidget(new QLabel("Pos"),0,0);
	robotPositionLayout->addWidget(new QLabel("Dir"),1,0);
	
	
	
	
	robotModePositionArea->setLayout(robotModePositionLayout);	
	robotGeneralTabLayout->addWidget(robotModePositionArea,1,0);
	
	
	//
	//Serial control
	//
	QWidget * robotGeneralTabBottomWidget = new QWidget();
	QGridLayout *robotGeneralTabBottomLayout = new QGridLayout;	
	QWidget * robotSerialControlArea = new QWidget();
	QGridLayout *robotSerialControlLayout = new QGridLayout;	
	
	
	for(int i=0;i<5;i++){
		motorStatusLabel[i] = new QLabel("");
		robotSerialControlLayout->addWidget(motorStatusLabel[i],i,0);
	}
	
	panicStatus =  new QLabel("Status: OK");
	resetMotors = new QPushButton("Reset all motors");
	
	unlockMotors = new QPushButton("Un&lock");
	unlockMotors->setCheckable(true);
	
	robotSerialControlLayout->addWidget(panicStatus,6,0);
	robotSerialControlLayout->addWidget(resetMotors,7,0);	
	robotSerialControlLayout->addWidget(unlockMotors,8,0);	
	
	robotSerialControlArea->setLayout(robotSerialControlLayout);	
	robotGeneralTabBottomLayout->addWidget(robotSerialControlArea,0,0);
	robotGeneralTabBottomWidget->setLayout(robotGeneralTabBottomLayout);
	
	robotGeneralTabLayout->addWidget(robotGeneralTabBottomWidget,2,0);
	
	QWidget * robotControllerWidget = new QWidget();
	robotControllerLayout = new QVBoxLayout();
	
	robotControllerLayout->setAlignment(Qt::AlignTop);
	robotControllerWidget->setLayout(robotControllerLayout);
	robotGeneralTabBottomLayout->addWidget(robotControllerWidget,0,1);
	
	controllerItemLocked = new QCheckBox("Locked");
	controllerItemLocked->setEnabled(false);
	robotControllerLayout->addWidget(controllerItemLocked);
	
	
	
	QWidget * robotSerialTab = new QWidget();
	QGridLayout *robotSerialTabLayout = new QGridLayout();
	
	for(int i=0;i<8;i++){
		byteone[i] = new QCheckBox(("Byte 1-"+ofToString(i, 0)).c_str());
		bytetwo[i] = new QCheckBox(("Byte 2-"+ofToString(i, 0)).c_str());
		bytestatus[i] = new QCheckBox(("Byte status-"+ofToString(i, 0)).c_str());
		robotSerialTabLayout->addWidget(byteone[i],i,0);
		robotSerialTabLayout->addWidget(bytetwo[i],i,1);
		robotSerialTabLayout->addWidget(bytestatus[i],i,2);
	}
	
	
	
	robotGeneralTabLayout->setAlignment(Qt::AlignTop);
	robotGeneralTab->setMinimumWidth(500);
	robotGeneralTab->setLayout(robotGeneralTabLayout);
	
	robotSerialTabLayout->setAlignment(Qt::AlignTop);
	robotSerialTab->setMinimumWidth(500);
	robotSerialTab->setLayout(robotSerialTabLayout);
	
	
	robotTab->addTab(robotGeneralTab,"General");
	robotTab->addTab(robotSerialTab,"Serial");
	//robotTab->addTab(robotControllerTab,"Robot controller");
	
	robotGeneralTabLayout->setAlignment(Qt::AlignTop);
	//	robotInputTab->setLayout(robotInputTabLayout);
	//	robotTab->addTab(robotInputTab,"Inputs");
	
	
	
	
	boxLayout->addWidget(robotTab);
	boxLayout->setAlignment(Qt::AlignTop);
	
	//	lastReverseHead = true;
	resetting = 0;
	
	
	manualControllerItem =addRobotController("Manual", CONTROL_MODE_MOTOR);
	currentControllerItem = manualControllerItem;
	setRobotController(manualControllerItem);
	
	warningShown = false;
}


void Robot::setControlMode(int mode)
{
	if(controlMode != mode)
	{
		controlMode = mode;
		industrialRobot->thread.controller->setInput(controlMode);
		inputRadio[controlMode]->setChecked(true);
		
	}
}

int Robot::getControlMode()
{
	return controlMode;
}


void Robot::update(){
	
	if(industrialRobot->getEmergencyStop()){
		unlockMotors->setChecked(false);	
	}
	
	tjeckCurrentRobotController();	
	
	//Check for changes in control mode (tilføjet af mads)
	for(int i = 0; i < 4;i++)
	{
		if(inputRadio[i]->isChecked() && controlMode != i)
		{
			setControlMode(i);
		}
	}
	
	
	if(industrialRobot->thread.lock()){
		if(loadXmlButton->isDown()){
			industrialRobot->thread.converter->loadXml();
			loadXmlButton->setDown(false);
		}
		industrialRobot->thread.unlock();
	}/*
	 if(pause->isChecked()){
	 industrialRobot->thread.timeline->pause();
	 } else {
	 industrialRobot->thread.timeline->play();	
	 }
	 */
	
	unsigned char b1=0;
	for(int i=0;i<8;i++){
		if(byteone[i]->isChecked()){
			b1 |= 0x01<<i;
		}
	}
	
	unsigned char b2=0;
	for(int i=0;i<8;i++){
		if(bytetwo[i]->isChecked()){
			b2 |= 0x01<<i;
		}
	}
	
	if(industrialRobot->thread.serial->lock()){
		industrialRobot->thread.serial->outbuf[24-3] = b1;
		//	cout<<(unsigned char )b1<<"  "<<(unsigned char )b2<<endl;
		industrialRobot->thread.serial->outbuf[24-2] = b2;
		
		for(int i=0;i<8;i++){				
			if(industrialRobot->thread.serial->returnedFlags[i])
				bytestatus[i]->setChecked(true);
			else
				bytestatus[i]->setChecked(false);
		}
		
		if(resetting > 0){
			switch (resetting) {
				case 1:
					unlockMotors->setChecked(false);
					resetting++;
					byteone[5]->setChecked(true);
					ofSleepMillis(20);
					break;

				case 2:
/*					industrialRobot->thread.controller->gotoResetPosition();					
					for(int i=0;i<5;i++){
						motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation*100.0);				
					}*/
					byteone[5]->setChecked(false);

					resetting++;
					break;		
				case 3:
					if(bytetwo[1]->isChecked()){
						if(bytestatus[1]->isChecked()){
							resetting ++;
						}
					} else {
						bytetwo[1]->setChecked(true);
			//			ofSleepMillis(10);
						unlockMotors->setChecked(true);
					}
					break;
				case 4:
					resetting++;
					break;
				case 5:
					if(bytetwo[3]->isChecked()){
						if(bytestatus[3]->isChecked() && bytestatus[4]->isChecked()){
							resetting ++;
						}
					} else {
						bytetwo[3]->setChecked(true);
						bytetwo[4]->setChecked(true);
			//			ofSleepMillis(10);
					}					
					break;
				case 6:
					if(bytetwo[2]->isChecked()){
						if(bytestatus[2]->isChecked()){
							resetting ++;
						}
					} else {
						bytetwo[2]->setChecked(true);
				//		ofSleepMillis(10);
					}					
					break;
				case 7:
					if(bytetwo[0]->isChecked()){
						if(bytestatus[0]->isChecked()){
							resetting ++;
						}
					} else {
						bytetwo[0]->setChecked(true);
				//		ofSleepMillis(10);
					}					
					break;
				case 8:
					for(int i=0;i<5;i++){
						bytetwo[i]->setChecked(false);
					}
					industrialRobot->thread.controller->gotoResetPosition();					
					for(int i=0;i<5;i++){
						motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation*100.0);				
					}
					resetting = 0;
					break;
				default:
					break;
			}

		}
		
		/*bool removeReset = true;
		for(int i =0;i<5;i++){
			if(!bytestatus[i]->isChecked() || !bytetwo[i]->isChecked()){
				removeReset = false;
			}
		}
		if(!removeReset){
			if(resetting == 1){
				resetting = 2;
			} 	
		}
		
		if(removeReset){
			if(resetting == 2){
				for(int i =0;i<5;i++){
//					bytetwo[i]->setChecked(false);
				}
			}
			
		}
		*/
		//	if(!allOk){
		//		industrialRobot->thread.timeline->pause();	
		//	} else {
		//		industrialRobot->thread.timeline->play();	
		//
		//		}
		
		if(industrialRobot->thread.serial->connected){
			resetMotors->setEnabled(true);
			unlockMotors->setEnabled(true);
		} else {
			resetMotors->setEnabled(false);
			unlockMotors->setEnabled(false);			
		}
		
		for(int i=0;i<5;i++){
			string l;
			if(industrialRobot->thread.serial->connected){
				l = "Motor "+ofToString(i,0)+": ";
				if(!byteone[7]->isChecked()){
					l+= "LOCKED ";
				}
				if(bytetwo[i]->isChecked()){
					if(bytestatus[i]->isChecked()){
						l+= "Resetting done...";
					} else {
						l+= "Resetting...";	
					}
				} else if(bytestatus[i]->isChecked()){
					l+= "OK";
				} else {
					l+= "NOT OK";	
				}
			} else {
				l = " ";
			}
			motorStatusLabel[i]->setText((l).c_str());
		}
		
		if(!industrialRobot->thread.serial->connected){
			panicStatus->setText("Status: Serial not connected");		
		}else if(bytestatus[7]->isChecked()){
			panicStatus->setText("Status: PANIC!!!");
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Panic!");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else if(bytestatus[6]->isChecked()){
			panicStatus->setText("Status: EMERGENCY STOP");
			/*industrialRobot->thread.serial->unlock();
			 if(!warningShown){
			 QMessageBox msgBox;
			 msgBox.setText("Emergency stop pressed. Release it, and unlock the robot.");
			 msgBox.exec();
			 warningShown = true;
			 }
			 industrialRobot->thread.serial->lock();*/
		} else {
			panicStatus->setText("Status: OK");	
//			warningShown = false;
		}
		
		if(resetMotors->isDown()){
			resetting  = 1;
		/*	for(int i=0;i<5;i++){
				bytetwo[i]->setChecked(true);
			}
			resetMotors->setDown(false);
			if(byteone[7]->isChecked()){
				unlockMotors->setDown(true);
			}
	
		*/	

			// Mads HERTIL
		}
		
		if(unlockMotors->isChecked() && !byteone[7]->isChecked()){
			byteone[7]->setChecked(true);
		} else if(!unlockMotors->isChecked() && byteone[7]->isChecked()){
			byteone[7]->setChecked(false);
		}
		
		
		industrialRobot->thread.serial->unlock();
	}	
	
	if(resetting > 0){
		ofSleepMillis(200);
	
	}
	
	/*
	 Fjernet af mads
	 int input = 0;
	 
	 
	 if(inputRadio[0]->isChecked()){
	 input = 0;
	 } else if(inputRadio[1]->isChecked()){
	 input = 1;
	 }  else if(inputRadio[2]->isChecked()){
	 input = 2;
	 } else if(inputRadio[3]->isChecked()){
	 input = 3;
	 } 
	 */
	
	
	/*
	 
	 //Fjernet af mads
	 if(inputSet != input){
	 industrialRobot->thread.controller->setInput(input);
	 inputSet = input;
	 }*/
	
	if(industrialRobot->thread.lock()){
		if(industrialRobot->thread.panic){
		 unlockMotors->setChecked(false);
		 if(!warningShown){
			 industrialRobot->thread.unlock();
			 QMessageBox msgBox;
			 msgBox.setText("PANIC!!");
			 ofxVec3f curPos = industrialRobot->getCurrentTarget(false);
			 ofxVec3f curDir = industrialRobot->getCurrentDir(false);
			 msgBox.setInformativeText(QString((industrialRobot->thread.panicMessage+"\n\nCurrent position: "+ofToString(curPos.x,1)+" , "+ofToString(curPos.y,1)+" , "+ofToString(curPos.z,1)+"\nCurrent direction: "+ofToString(curDir.x,2)+" , "+ofToString(curDir.y,2)+" , "+ofToString(curDir.z,2)).c_str()));
			 msgBox.exec();
			 warningShown = true;
			 industrialRobot->thread.lock();
		//	 industrialRobot->thread.panic = false;
		//	 industrialRobot->thread.panicMessage = "No message";

		 }
		} else {
		//	warningShown = false;
		}
		float rotations[5];
		for(int i=0;i<5;i++){
			rotations[i] = industrialRobot->thread.coreData.arms[i].rotation;
		}
		
		for(int i=0;i<5;i++){
			
			motorSlider[i]->setMinimum(industrialRobot->thread.converter->getMinRotation(i, rotations)*100.0);
			motorSlider[i]->setMaximum(industrialRobot->thread.converter->getMaxRotation(i, rotations)*100.0);
			if(getControlMode() == 0 || getControlMode() >= 2 || initFlag){
				//			cout<<industrialRobot->thread.coreData.arms[i].rotation<<endl;
				motorSlider[i]->setValue(industrialRobot->thread.coreData.arms[i].rotation*100.0);
				motorSlider[i]->setEnabled(false);
			} else if(getControlMode() == 1){
				motorSlider[i]->setEnabled(true);
				if(i == 2 && motorSlider[i]->value() == 10030){
					industrialRobot->thread.coreData.arms[i].rotation =  100;					
				} else {
					industrialRobot->thread.coreData.arms[i].rotation = motorSlider[i]->value() / 100.0;
				}
				motorLabel[i]->setText(ofToString(industrialRobot->thread.coreData.arms[i].rotation, 0).c_str());
				
			}
			float v1 = industrialRobot->thread.coreData.arms[i].rotation;
			float v2 = industrialRobot->thread.converter->convertRotation(i);
			motorLabel[i]->setText((ofToString(v1, 1)).c_str());
			
			motorLabel2[i]->setText(("("+ofToString(v2, 1)+")").c_str());
			if(setValue[i]->isDown()){
				industrialRobot->thread.unlock();
				
				bool ok;
				double d = QInputDialog::getDouble(new QWidget, QString("Set angle"), "Angle", motorSlider[i]->value(), -10000, 10000, 2, &ok);
				if (ok)
					//doubleLabel->setText(QString("$%1").arg(d));
					motorSlider[i]->setValue(d*100.0);
				
				
				setValue[i]->setDown(false);
				industrialRobot->thread.lock();
			}
		}
		
		variantSlider->setValue(industrialRobot->thread.coreData.reverseHeadPercent*1000.0);
		
		if(getControlMode() == 3){
			for(int i=0;i<6;i++){
				manualPosition[i]->setEnabled(true);	
			}	
			
			ofxVec3f target = ofxVec3f(manualPosition[0]->value()*10.0, manualPosition[1]->value()*10.0, manualPosition[2]->value()*10.0);
			ofxVec3f targetDir = ofxVec3f(manualPosition[3]->value(), manualPosition[4]->value(), manualPosition[5]->value());
			industrialRobot->thread.unlock();
			bool couldAdd = industrialRobot->setGravityTarget(target, targetDir, 1.5, 3.0);
			industrialRobot->thread.lock();
			
			/*	if(industrialRobot->thread.controller->legalPosition(target,  targetDir, 1.0) || industrialRobot->thread.controller->legalPosition(target,  targetDir, 0.0)){
			 industrialRobot->thread.controller->gravityTarget = target;
			 industrialRobot->thread.controller->gravityTargetDir = targetDir;
			 }
			 */
			manualPosition[0]->setValue(industrialRobot->thread.controller->gravityTarget.x*0.1);
			manualPosition[1]->setValue(industrialRobot->thread.controller->gravityTarget.y*0.1);
			manualPosition[2]->setValue(industrialRobot->thread.controller->gravityTarget.z*0.1);
			manualPosition[3]->setValue(industrialRobot->thread.controller->gravityTargetDir.x);
			manualPosition[4]->setValue(industrialRobot->thread.controller->gravityTargetDir.y);
			manualPosition[5]->setValue(industrialRobot->thread.controller->gravityTargetDir.z);
			
			
			
			
		} else {
			for(int i=0;i<6;i++){
				manualPosition[i]->setEnabled(false);	
			}
			manualPosition[0]->setValue(industrialRobot->thread.controller->targetPosition.x*0.1);
			manualPosition[1]->setValue(industrialRobot->thread.controller->targetPosition.y*0.1);
			manualPosition[2]->setValue(industrialRobot->thread.controller->targetPosition.z*0.1);
			manualPosition[3]->setValue(industrialRobot->thread.controller->targetDir.x);
			manualPosition[4]->setValue(industrialRobot->thread.controller->targetDir.y);
			manualPosition[5]->setValue(industrialRobot->thread.controller->targetDir.z);
			
		}
		
		if(currentControllerItem->locked){
			controllerItemLocked->setChecked(true);
		} else {
			controllerItemLocked->setChecked(false);			
		}
		
		industrialRobot->thread.unlock();
	}
	
	
	initFlag = false;
	
	
}



void Robot::threadedFunction(){
	
}


void Robot::draw()
{
	int x = 0, y=0, w=500, h=500;
	if(industrialRobot->thread.lock()){
		industrialRobot->visualizer->draw3d(x, y, w/2.0-2, h/2.0-2);
		industrialRobot->visualizer->drawtop(x+(w/2.0)+2, y, w/2.0-2, h/2.0-2);
		industrialRobot->visualizer->drawside(x, y+(h/2.0)+2, w/2.0-2, h/2.0-2);
		industrialRobot->visualizer->drawGraphs(x+(w/2.0)+2, y+(h/2.0)+2, w/2.0-2, h/2.0-2);
		industrialRobot->thread.unlock();
		
	}
}


//ROBOT CONTROLLER SYSTEM

void Robot::tjeckCurrentRobotController()
{
	if(!currentControllerItem->radioButton->isChecked())
	{
		for(list<RobotControllerItem*>::iterator it=robotControllers.begin(); it!=robotControllers.end(); it++){
			if((*it)->radioButton->isChecked())
			{
				setRobotController((*it));
			}
		}
	}
}


bool Robot::setRobotController(RobotControllerItem * controllerItem)
{
	if(!currentControllerItem->locked){
		controllerItem->radioButton->setChecked(true);
		setControlMode(controllerItem->mode);
		currentControllerItem = controllerItem;
		return true;
	}  else
		return false;
}

RobotControllerItem * Robot::addRobotController(char*  _name, int mode)
{
	RobotControllerItem * tmpItem = new RobotControllerItem();
	tmpItem->radioButton = new  QRadioButton(_name);
	tmpItem->mode = mode;
	robotControllers.push_back(tmpItem);
	robotControllerLayout->addWidget(tmpItem->radioButton);
	return tmpItem;
}