/*
 *  BasePlugin.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "Data.h"
#include "ofxThread.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QTGui>
#include <QCheckBox>

#define TYPE_SLIDER 1
#define TYPE_CHECKBOX 2
#define TYPE_RADIOBUTTON 3
#define TYPE_TAB_POSITION 4
#define TYPE_VALUE 5
#define TYPE_TRISTATE_CHECKBOX 6

class hookedUpItem {
	public:
	int myTypeid;
	string name;
};

class widgetHookedUp : public hookedUpItem
{
public:
	QWidget * widget;
};

class valueHookedUp : public hookedUpItem
{
	public:
	float * value;
};

class BasePlugin : public ofxThread {
public:
	virtual void init(myData * _data,  QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	void loadSettings(QSettings * settings);
	void saveSettings(QSettings * settings);
	void hookupWidget(QWidget * qWidget, string name, int _typeid);
	void hookupValue(float * val, string name);	
	void addBaseGui(QVBoxLayout * boxLayout);
	bool tryLock();
	bool isLocked(pthread_mutex_t * _mutex);
	bool isLocked();
	BasePlugin* getFilter(string _name);
	myData * data;
	string nameid;
	QCheckBox *chkEnabled;
	QBoxLayout *boxLayout;
	vector<hookedUpItem*> hookedUpItems;
	QSlider * AddQSlider(string name, int value, int min, int max, QGridLayout * layout, int pos, bool autosave);
	

};