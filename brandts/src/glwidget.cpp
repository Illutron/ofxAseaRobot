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

#include "ofMain.h"
#include <QtGui>
#include <QtOpenGL>
#include <QtGlobal>

#include <math.h>

#include "glwidget.h"


GLWidget::GLWidget(testApp * app, QWidget *parent): QGLWidget(parent)
{
		qt_set_sequence_auto_mnemonic(true);
	ofApp = app;
	drawMethod = (&testApp::draw);
	frameRate = 0;
	frames = 0;
	mousePressed = 0;	
	setMouseTracking(true);
	makeCurrent();
	
}

GLWidget::~GLWidget()
{
    makeCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::setXRotation(int angle)
{
//	((testApp*)ofApp)->slider1 = angle;
	
}
void GLWidget::setRotateSlider(int angle)
{
//	((testApp*)ofApp)->rotateSlider = angle;	
}

void GLWidget::initializeGL()
{	
}

void GLWidget::update(){
	static ofEventArgs voidEventArgs;
	ofApp->update();
#ifdef OF_USING_POCO
	ofNotifyEvent( ofEvents.update, voidEventArgs );
#endif
}

void GLWidget::paintGL()
{
	makeCurrent();
	static ofEventArgs voidEventArgs;
	
	//Update framerate
	frameRate = frames /(time.elapsed() / 1000.0);
	frameRateChanged(frameRate);
	if (!(frames % 100)) {
		time.start();
		frames = 0;
	}
	frames ++;
	
	//Draw
	glPushMatrix();
	if(ofbClearBg()){
		glClearColor(ofBgColorPtr()[0],ofBgColorPtr()[1],ofBgColorPtr()[2], ofBgColorPtr()[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	
	ofApp->draw(viewCombo->currentIndex());
	//(ofApp->*drawMethod)();
#ifdef OF_USING_POCO
	ofNotifyEvent( ofEvents.draw, voidEventArgs );
#endif
	
	glPopMatrix();
}

void GLWidget::resizeGL(int width, int height)
{
	ofApp->windowResized(width, height);
	
	glViewport(0, 0, width, height);
	ofSetupScreen();			
	glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	mousePressed = event->button();
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = event->pos().x();
	mouseEventArgs.y = event->pos().y();
	mouseEventArgs.button = mousePressed;
	ofApp->mousePressed(event->pos().x(), event->pos().y(), event->button());
#ifdef OF_USING_POCO
	ofNotifyEvent( ofEvents.mousePressed, mouseEventArgs );
#endif
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	mousePressed = 0;

	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = event->pos().x();
	mouseEventArgs.y = event->pos().y();
	mouseEventArgs.button = mousePressed;
	ofApp->mouseReleased(event->pos().x(), event->pos().y(), event->button());
#ifdef OF_USING_POCO
	ofNotifyEvent( ofEvents.mouseReleased, mouseEventArgs );
#endif
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	static ofMouseEventArgs mouseEventArgs;
	mouseEventArgs.x = event->pos().x();
	mouseEventArgs.y = event->pos().y();
	mouseEventArgs.button = mousePressed;

	ofApp->mouseX = event->pos().x();
	ofApp->mouseY = event->pos().y();
	if(mousePressed == 0){
		ofApp->mouseMoved(event->pos().x(), event->pos().y());
#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.mouseMoved, mouseEventArgs );
#endif
	} else {
		ofApp->mouseDragged(event->pos().x(), event->pos().y(), mousePressed);
#ifdef OF_USING_POCO
		ofNotifyEvent( ofEvents.mouseDragged, mouseEventArgs );
#endif
	}
	lastPos = event->pos();
}


