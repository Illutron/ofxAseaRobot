/*---------------------------------------------------
Open Frameworks 0.05
graphical user interface - Dennis Rosenfeld-2008
*///--------------------------------
#include "GuiHandler.h"


GuiHandler::GuiHandler()//constructor
{
    initialize();
}


GuiHandler::~GuiHandler()//destructor
{
}


void    GuiHandler::initialize()
{

    isHidden = false;
    hideKey = 'h';
    keysWhileHidden = true;

    isTypingBlocked = false;
	
	ofAddListener(ofEvents.mouseMoved, this, &GuiHandler::mouseMoved);
	ofAddListener(ofEvents.mousePressed, this, &GuiHandler::mousePressed);
	ofAddListener(ofEvents.mousePressed, this, &GuiHandler::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &GuiHandler::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &GuiHandler::mouseReleased);	
	ofAddListener(ofEvents.keyPressed,this,&GuiHandler::keyPressed);
	ofAddListener(ofEvents.keyReleased,this,&GuiHandler::keyReleased);
	
//    ofMouseEvents.addListener(this);
//	ofKeyEvents.addListener(this);
	//ofAppEvents.addListener(this);
    //PanelList = new GuiPanel[5];
}

GuiPanel*   GuiHandler::addPanel(string name,int x,int y,int w, int h)
{
    GuiPanel* newPanel = new GuiPanel;
    newPanel->initialize(name,x,y,w,h);
    panelList.push_back(newPanel);

    return newPanel;
}

bool    GuiHandler::updateBool(int idNum)
{
    bool tempVal = false;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueBool();
            }
        }
    }
    return tempVal;
}


float    GuiHandler::updateFloat(int idNum)
{
    float tempVal= 0;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueFloat();
            }
        }
    }
    return tempVal;
}

string    GuiHandler::updateString(int idNum)
{
    string tempVal;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueString();
            }
        }
    }
    return tempVal;
}

int    GuiHandler::updateInt(int idNum)
{
    int tempVal = 0;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->getValueInt();
            }
        }
    }
    return tempVal;
}

void    GuiHandler::draw()
{
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            panelList[i]->draw();
        }
    }
}

void    GuiHandler::hide()
{
    isHidden = true;
    isTypingBlocked = true;

}

void    GuiHandler::unHide()
{
    isHidden = false;
    isTypingBlocked = false;
}

void    GuiHandler::mouseMoved(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
            panelList[i]->setLine(true);
            panelList[i]->mouseMoved(x,y);
          }
          else
            {
            panelList[i]->setLine(false);
          }
        }
    }
}

void    GuiHandler::mousePressed(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
	int button = _args.button;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
            panelList[i]->mousePressed(x,y,button);
          }

          else //not in boundaries.
          {
            //check elements for textfields and depress them.
            for (int j = 0; j < panelList[i]->elements.size(); j++ )
            {
                if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
                {
                   panelList[i]->elements[j]->dePress();
                }
            }
          }

         }//end panel size.
    }
}

void    GuiHandler::mouseReleased(ofMouseEventArgs& _args)
{
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            panelList[i]->mouseReleased();
        }
    }
}


void    GuiHandler::mouseDragged(ofMouseEventArgs& _args)
{
	int x = _args.x;
	int y = _args.y;
	int button = _args.button;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
          if ( panelList[i]->checkBoundaries(x,y) == true)
          {
            panelList[i]->mouseDragged(x,y,button);
          }

        }
    }
}


void GuiHandler::keyPressed(ofKeyEventArgs& _args)
{
	int key = _args.key;

    bool keypressOK = true;
    if (isTypingBlocked == false)
    {
            for (int i = 0; i < panelList.size(); i++ )
            {
                for (int j = 0; j < panelList[i]->elements.size(); j++ )
                {

                    if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
                    {
                        if( panelList[i]->elements[j]->keyPressOK() == false)
                        {
                            keypressOK = false;
                            panelList[i]->elements[j]->keyPressed(key);//only pass keys to the textbox.
                        }
                    }
                }
            }
            if (keypressOK == true)
            {
                if (key == hideKey)
                {
                    if ( isHidden == false)
                    {
                     isHidden = true;
                    }
                    else if ( isHidden == true)
                    {
                     isHidden = false;
                    }

                }

                if (isHidden == false || keysWhileHidden == true)
                {
                    for (int i = 0; i < panelList.size(); i++ )
                    {
                        for (int j = 0; j < panelList[i]->elements.size(); j++ )
                        {
                            panelList[i]->elements[j]->keyPressed(key);
                            //cout<<"typing test";
                        }
                    }
                }
            }
    }
}

void GuiHandler::keyReleased(ofKeyEventArgs& _args)
{
	int key = _args.key;
    if (isHidden == false)
    {
        for (int i = 0; i < panelList.size(); i++ )
        {
            for (int j = 0; j < panelList[i]->elements.size(); j++ )
            {
                   panelList[i]->elements[j]->keyReleased(key);
            }
        }
    }
}

void GuiHandler::setHideKey(int key)
{
    hideKey = key;
}


bool    GuiHandler::listenForTrigger(int idNum)
{
    int tempVal = 0;

    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( idNum == panelList[i]->elements[j]->getID() )
            {
                tempVal = panelList[i]->elements[j]->listenForTrigger();
            }
        }
    }
    return tempVal;
}


bool   GuiHandler::isSafeToType()
{
    bool keypressOK = true;
    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
            if ( panelList[i]->elements[j]->checkType() == "TEXTFIELD")
            {
                if( panelList[i]->elements[j]->keyPressOK() == false)
                {
                    keypressOK = false;
                }
            }
        }
    }
    return keypressOK;
}

bool      GuiHandler::isGuiHidden()
{
 return isHidden;
}



void        GuiHandler::disableKeys()
{
    isTypingBlocked = true;
}
void        GuiHandler::enableKeys()
{
    isTypingBlocked = false;
}


//------------XML SETTINGS-------------------------------------------------
//-------------------------------------------------------------------------------


void GuiHandler::saveToXML(string file) {
	int which = 0;
	int idNum =0;
	float tempVal =0;
    for (int i = 0; i < panelList.size(); i++ )
    {
        for (int j = 0; j < panelList[i]->elements.size(); j++ )
        {
          // if ( panelList[i]->elements[j]->checkType() == "SLIDER") 
            {
				idNum = panelList[i]->elements[j]->getID() ;
              
				XML.setValue("element:myvalue", (double)panelList[i]->elements[j]->getValueFloat(), which);
				XML.setValue("element:mystate", panelList[i]->elements[j]->getValueBool(), which);
				XML.setValue("element:mytext", panelList[i]->elements[j]->getValueString(), which);
				XML.setValue("element:myselection", panelList[i]->elements[j]->getValueInt(), which);
				
				XML.setValue("element:id", idNum, which);
				XML.popTag();
				which++;
            }
			
        }
		
    }
	
	cout << file;
	XML.saveFile(file);
	cout << " --- Settings Saved ---\n";
	

	
}

void GuiHandler::loadFromXML(string file) {
	/*
	 bool            myState;
	 float           myValue;
	 string          myText;
	 */
	
	if(XML.loadFile(file)) 
	{
		int numElements  = XML.getNumTags("element");
		for(int b=0; b<numElements; b++) 
		{
			int node_id		 = XML.getValue("element:id", 0, b);
		
			
			bool foundElement = false;
			for (int i = 0; i < panelList.size(); i++ )
			{
				for (int j = 0; j < panelList[i]->elements.size(); j++ )
				{
					if(panelList[i]->elements[j]->getID() == node_id)
					{
						panelList[i]->elements[j]->myValue =  XML.getValue("element:myvalue", 0.0, b);
						panelList[i]->elements[j]->myState =  XML.getValue("element:mystate", 0, b);
						panelList[i]->elements[j]->myText =  XML.getValue("element:mytext", "", b);
						panelList[i]->elements[j]->mySelection =  XML.getValue("element:myselection", 0, b);
						
						
						foundElement = true;
						break;
					}
					
				}
				if(foundElement) break;
			}
	
		}
	}
}


