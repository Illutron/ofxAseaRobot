/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "glwidget.h"
#include "window.h"

//! [0]

void Window::setApp(testApp * app){
	ofApp = app;
	glWidget->ofApp = ofApp;
}

int Window::getGlWidth(){
	return glWidget->width();
}
int Window::getGlHeight(){
	return glWidget->height(); 
}
float Window::getGlFrameRate(){
	return glWidget->frameRate;

}

Window::Window(testApp * _app)
{
	
	QWidget * content = new QWidget();

	ofApp = _app;
//	tab = new QTabWidget;
	viewCombo = new QComboBox();

    glWidget = new GLWidget(ofApp);
	glWidget->setFixedWidth(500);
	glWidget->setFixedHeight(500);
	
//	content->setFixedWidth(500);
//	content->setFixedHeight(500);
	
	glWidget->viewCombo = viewCombo;
	
	QTimer *timer = new QTimer(this);
    timer->setInterval(20);
	QObject::connect(timer, SIGNAL(timeout()), glWidget, SLOT(update()));
	QObject::connect(timer, SIGNAL(timeout()), glWidget, SLOT(updateGL()));
	timer->start();
	
    setWindowTitle(tr("Illutron Robot"));
	
	QTimer *settingsTimer = new QTimer(this);
    settingsTimer->setInterval(1000);
	QObject::connect(settingsTimer, SIGNAL(timeout()), this, SLOT(saveSettings()));
	settingsTimer->start();


	
	
	//GENERAL SETTINGS TAB
	QLabel *fpsLabel = new QLabel(tr("FPS"));
	
	QWidget *settingsGroupBox = new QWidget();
	QGridLayout *settingsGrid = new QGridLayout;
	settingsGrid->setAlignment(Qt::AlignTop);
	settingsGrid->addWidget(new QLabel(tr("FPS:")), 0, 0);
	settingsGrid->addWidget(fpsLabel, 0,1);
	settingsGrid->setColumnStretch(1,20);
	settingsGroupBox->setLayout(settingsGrid);
	
	//tab->addTab(settingsGroupBox, "General");
	
	/*
	QDockWidget *dockWidget = new QDockWidget(tr("General"), this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
								Qt::RightDockWidgetArea);
	dockWidget->setWidget(settingsGroupBox);*/
	//addDockWidget(Qt::RightDockWidgetArea, dockWidget);
	
	setTabPosition(Qt::RightDockWidgetArea,QTabWidget::East);

	
	

	//SETUP MAIN LAYOUT
    QHBoxLayout *mainLayout = new QHBoxLayout;
	QWidget *glGroupBox = new QWidget();
	QVBoxLayout *glLayout = new QVBoxLayout;
	
	
	glGroupBox->setLayout(glLayout);
	glLayout->addWidget(viewCombo);
	glLayout->addWidget(glWidget);
	glLayout->setAlignment(Qt::AlignTop);
	
	mainLayout->addWidget(glGroupBox);
//	mainLayout->addWidget(tab);
	
    content->setLayout(mainLayout);
	
	setCentralWidget(content);
	
	
	//GENERAL EVENT CONNECTIONS
	//connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int))); //This line connects the x slider to the testApp
   // connect(rotateSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setRotateSlider(int)));
	connect(glWidget, SIGNAL(frameRateChanged(double)), fpsLabel, SLOT(setNum(double)));
	//connect(tabLayout, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));

	settings = new QSettings("BrandtsRobot", "Illutron");
	
	setUnifiedTitleAndToolBarOnMac(true);
	
	lock = new QAction(QIcon(QString(ofToDataPath("lock.png", true).c_str())), tr("Lock"),this);
	lock->setShortcut( QKeySequence(Qt::ControlModifier + Qt::Key_L));

	QObject::connect(lock, SIGNAL(triggered()), this, SLOT(toggleLock()));

	toolbar = addToolBar(tr("Toolbar"));
	setIconSize(QSize(20,20));
	toolbar->addAction(lock);
	statusBar()->showMessage(tr("Locked"));
	
	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks);

}
void Window::saveSettings()
{
	ofApp->saveSettings(settings);
	settings->setValue("mainwindow/size", size());
	settings->setValue("mainwindow/pos", pos());
//	settings->setValue("maintab/index", tab->currentIndex());
	settings->setValue("viewCombo/index", viewCombo->currentIndex());
	settings->setValue("layout/state", 	saveState());
}

void Window::loadSettings()
{
	// SETTINGS
	
	ofApp->loadSettings(settings);
	resize(settings->value("mainwindow/size", QSize(400, 400)).toSize());
	move(settings->value("mainwindow/pos", QPoint(200, 200)).toPoint());
//	tab->setCurrentIndex(settings->value("maintab/index").toInt());
	viewCombo->setCurrentIndex(settings->value("viewCombo/index").toInt());
	restoreState((settings->value("layout/state").toByteArray()));
}

void Window::keyPressEvent(QKeyEvent *event){
	ofApp->keyPressed(event->key());
}

void Window::KeyReleaseEvent(QKeyEvent *event){
	ofApp->keyReleased(event->key());
}

void Window::toggleLock(){
	if(((Robot*)ofApp->controller->tmpRobot)->unlockMotors->isChecked()){
		((Robot*)ofApp->controller->tmpRobot)->unlockMotors->setChecked(false);
		lock->setIcon(QIcon(QString(ofToDataPath("lock.png", true).c_str())));

	} else {
		((Robot*)ofApp->controller->tmpRobot)->unlockMotors->setChecked(true);	
		lock->setIcon(QIcon(QString(ofToDataPath("unlock.png", true).c_str())));

	}
}
