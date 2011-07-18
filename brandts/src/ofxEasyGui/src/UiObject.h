#ifndef UIOBJECT_H_INCLUDED
#define UIOBJECT_H_INCLUDED

#include "ofMain.h"

class   UiObject
{
    //Handles overall control of the User interface.

    protected:
        //VARIABLES:--------------------------------------------------

        int             myID;
        int             myX;
        int             myY;
        int             myWidth;
        int             myHeight;
        string          myType;
        string          myName;

        int             myKey;  //1st key
        int             myKey2; //second key

        bool            HoverOver;

        
        ofTrueTypeFont          myFont;

         float  colorBase[4];
         float  colorHighlight1[4];
         float  colorHighlight2[4];
         float  colorOver[4];

    public:
		int      mySelection;
		bool            myState;
		float           myValue;
		string          myText;
		bool            isDraggable;
	
        UiObject();                           //constructor
        ~UiObject();                          //destructor
        virtual void             initialize(string name,int x,int y,int w, int h,int idNum);          //initialize objects.

        //METHODS:--------------------------------------------------------
        void            loadDefaults(string name,int x,int y,int w, int h,int idNum);//called by initialize.
        void            update();
        virtual void    draw();

        virtual bool    checkBoundaries(int x, int y);               //check to see if mouse is within boundaries of object.
        virtual string  checkType();
        virtual bool    keyPressOK();
        void            setLine (bool OnOff);
        virtual void    press();
        virtual void    dePress();

        virtual void    keyPressed(int key);
        virtual void    keyReleased(int key);
        virtual void    drag(int x, int y);
        virtual void    release();

        virtual bool    getValueBool();
        virtual string  getValueString();
        virtual float   getValueFloat();
        virtual int     getValueInt();
        int             getID();
        int             getHeight();
        int             getWidth();
        virtual bool    listenForTrigger();

        virtual void    addItem(int idNum, string name);
        void            bindKey(int key);
        void            bindKey2(int key);
        virtual void    setFont( string fontString, int fontSize);

        //void            keyPressed(int key );
        //void            mousePressed(x, y, button);
        //void            mouseDragged(x, y, button);
        //void            mouseReleased(mouseX, mouseY, button);

};


#endif // UIOBJECT_H_INCLUDED
