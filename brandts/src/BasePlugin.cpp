/*
 *  BasePlugin.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "BasePlugin.h"


void BasePlugin::init(myData * _data ,  QVBoxLayout * boxLayout){
	data = _data;
	
	
}
void BasePlugin::hookupWidget(QWidget * qWidget, string name, int _typeid )
{
	widgetHookedUp * a = new widgetHookedUp();
	a->widget = qWidget;
	a->name = name;
	a->myTypeid = _typeid;
	hookedUpItems.push_back(a);
}

void BasePlugin::hookupValue(float * val, string name){
	valueHookedUp * a = new valueHookedUp();
	a->value = val;
	a->name = name;
	a->myTypeid = TYPE_VALUE;
	hookedUpItems.push_back(a);
}


void BasePlugin::loadSettings(QSettings * settings)
{
//	for(list<widgetHookedUp*>::iterator it=widgetsHookedUp.begin(); it!=widgetsHookedUp.end(); it++){
	for(int i=0;i<hookedUpItems.size();i++){
		hookedUpItem * it = hookedUpItems[i];
		//cout << (*it)->myTypeid ;
		int v ;
		switch ((it)->myTypeid)
		{
			case TYPE_SLIDER:
				((QSlider*)((widgetHookedUp*)it)->widget)->setValue(settings->value((((widgetHookedUp*)it)->name + nameid + "/value").c_str()).toInt());
				break;
			case TYPE_CHECKBOX:
				((QCheckBox*)((widgetHookedUp*)it)->widget)->setChecked(settings->value((((widgetHookedUp*)it)->name + nameid + "/value").c_str()).toBool());
				break;
				
			case TYPE_RADIOBUTTON:	
				((QRadioButton*)((widgetHookedUp*)it)->widget)->setChecked(settings->value(((it)->name + nameid + "/value").c_str()).toBool());
				break;
			case TYPE_TAB_POSITION:
				((QTabWidget*)((widgetHookedUp*)it)->widget)->setCurrentIndex(settings->value(((it)->name + nameid + "/value").c_str()).toInt());				
				break;
			case TYPE_VALUE:
				cout<<"load   "<<(settings->value(((it)->name + nameid + "/value").c_str()).toDouble())		<<endl;
				*((float*)((valueHookedUp*)it)->value) = (settings->value(((it)->name + nameid + "/value").c_str()).toDouble());				
				break;
			case TYPE_TRISTATE_CHECKBOX:	
				v = settings->value((((widgetHookedUp*)it)->name + nameid + "/value").c_str()).toInt();
				Qt::CheckState q;
				if(v == 0){
					q =  Qt::Unchecked;
				}
				if(v == 1){
					q =  Qt::PartiallyChecked;
				}
				if(v == 2){
					q =  Qt::Checked;
				}
				
				((QCheckBox*)((widgetHookedUp*)it)->widget)->setCheckState(q);

				break;
			default:
				cout << "Ups no defintion of hookup type (Baseplugin.cpp calling) \n";
				break;
		}
	}
	//chkEnabled->setChecked(settings->value((nameid + "/value").c_str()).toBool());	
}

void BasePlugin::saveSettings(QSettings * settings)
{
	for(int i=0;i<hookedUpItems.size();i++){
		hookedUpItem * it = hookedUpItems[i];
		//cout << (*it)->myTypeid ;
		switch ((it)->myTypeid)
		{
			case TYPE_SLIDER:
				settings->setValue(((it)->name + nameid + "/value").c_str(), ((QSlider*)((widgetHookedUp*)it)->widget)->value());
				break;
			case TYPE_CHECKBOX:	
				settings->setValue(((it)->name + nameid + "/value").c_str(), ((QCheckBox*)((widgetHookedUp*)it)->widget)->isChecked());
				break;
			case TYPE_RADIOBUTTON:				
				settings->setValue(((it)->name + nameid + "/value").c_str(), ((QRadioButton*)((widgetHookedUp*)it)->widget)->isChecked());
				break;
			case TYPE_TAB_POSITION:
				settings->setValue(((it)->name + nameid + "/value").c_str(), ((QTabWidget*)((widgetHookedUp*)it)->widget)->currentIndex());
				break;
			case TYPE_VALUE:
				settings->setValue(((it)->name + nameid + "/value").c_str(), (double)*((float*)((valueHookedUp*)it)->value));
				break;
			case TYPE_TRISTATE_CHECKBOX:	
				int v;
				if(((QCheckBox*)((widgetHookedUp*)it)->widget)->checkState() == Qt::Unchecked){
					v = 0;
				}
				if(((QCheckBox*)((widgetHookedUp*)it)->widget)->checkState() == Qt::PartiallyChecked){
					v = 1;
				}
				if(((QCheckBox*)((widgetHookedUp*)it)->widget)->checkState() == Qt::Checked){
					v = 2;
				}
				settings->setValue(((it)->name + nameid + "/value").c_str(), v);
				break;
			default:
				cout << "Ups no defintion of hookup type (Baseplugin.cpp calling) \n";
				break;
		}
	}
	//settings->setValue((nameid + "/value").c_str(),chkEnabled->isChecked());
}


void  BasePlugin::addBaseGui( QVBoxLayout * _boxLayout)
{
	boxLayout =_boxLayout;
	chkEnabled = new QCheckBox("Enabled");
	boxLayout->addWidget(chkEnabled);
	hookupWidget(chkEnabled, "enabled", TYPE_CHECKBOX );
		
}
void BasePlugin::update(){
	
};

void BasePlugin::draw(){
};

bool  BasePlugin::tryLock()
{
	return (0 == pthread_mutex_trylock(&myMutex)); 
};

bool BasePlugin::isLocked()
{
	if (0 == pthread_mutex_trylock(&myMutex))
	{
		pthread_mutex_unlock(&myMutex);
		return false;
	}	
	return true;

};


bool BasePlugin::isLocked(pthread_mutex_t * _mutex)
{
	if (0 == pthread_mutex_trylock(_mutex))
	{
		pthread_mutex_unlock(_mutex);
		return false;
	}	
	return true;
	
};
/*
BasePlugin* BasePlugin::getFilter(string _name){
 for(list<BasePlugin*>::iterator it=plugins->begin(); it!=plugins->end(); it++){
	 if((*it)->name == _name){
		 return (*it);
	 }
	}
	return NULL;
};
*/

QSlider * BasePlugin::AddQSlider(string name, int value, int min, int max, QGridLayout * layout, int pos,bool autoSave)
{	
	QSlider * tmpSlider = new QSlider(Qt::Horizontal);
	tmpSlider->setMinimum(min);
	tmpSlider->setMaximum(max);
	tmpSlider->setValue(value);
	layout->addWidget( new QLabel(name.c_str()),pos,0);	
	layout->addWidget(tmpSlider,pos,1);
	if(autoSave)
	{
		hookupWidget(tmpSlider, name, TYPE_SLIDER);
	}
	return tmpSlider;
}


