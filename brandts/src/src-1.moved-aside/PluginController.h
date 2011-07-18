/*
 *  PluginController.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Data.h"
#include "BasePlugin.h"

class PluginController {
public:
	PluginController(myData * _data);
	myData * data;
	
};