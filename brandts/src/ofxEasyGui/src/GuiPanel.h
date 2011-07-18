#ifndef GUIPANEL_H_INCLUDED
#define GUIPANEL_H_INCLUDED

#include "ofMain.h"

#include "UiObject.h"

#include "Button.h"
#include "Slider.h"
#include "Meter.h"
#include "TextField.h"
#include "Radio.h"
#include "FileList.h"

class   GuiPanel
{
    //Handles overall control of the User interface.

    private:
        //VARIABLES:--------------------------------------------------
        int             myX;
        int             myY;
        int             myWidth;
        int             myHeight;
        string          myName;
        bool            hidden;
        int             buttonSpacing;

        string          fontString;
        int             fontSize;
        ofTrueTypeFont  myFont;

        bool            HoverOver;
        string          state; //UP,OVER,PUSHED,ACTIVATED, ETC...

    public:
        GuiPanel();                           //constructor
        ~GuiPanel();                          //destructor
        void            initialize(string name,int x,int y,int w, int h);          //initialize objects.

        //METHODS:--------------------------------------------------------

        void            update();
        void            draw();

        vector          <UiObject*> elements;               //buttons, sliders, radios, textboxes, are all elements.

        UiObject*       addElement(string name,int w, int h,int idNum);//to be removed.....
        UiObject*       addSpacer(int h); //just add some space..
        UiButton*       addButton(string name,int w, int h,string switchMode,bool textOnTop,int idNum, bool * pointValue);
        UiSlider*       addFloatSlider(string name,int w, int h, float startVal, float MaxVal, int idNum,float * pointValue);
        UiSlider*       addIntSlider(string name,int w, int h, float startVal, float MaxVal, int idNum, int * pointValue);
		UiMeter*        addIntMeter(string name,int w, int h, float startVal, float MaxVal, int idNum, int * pointValue);
        UiTextField*    addTextField(string name,int w, int h, string myDefaultText, int idNum, string * pointValue);
        UiRadio*        addRadio(string name, int h, int nColumns, int idNum, int * pointValue);
        void               addRadioElement(int idNum, string name);
        UiFileList*     addFileList(string name,int w, int h, string directory,int idNum, string * pointValue);
        void            bindKey(int idNum, int key); //associate a key press with a button or ui element.
        void            bindKey2(int idNum, int key); //associate a second key press for up/down cycling.

        bool            checkBoundaries(int x, int y);           //check to see if mouse is within boundaries of this panel.
        void            setLine (bool OnOff);
        int             calcSize();
        void            adjustHeight(int amt);
        void            adjustWidth(int amt);

        void            mouseMoved(int x, int y);
        void            mousePressed(int x, int y, int button);
        void            mouseDragged(int x, int y, int button);
        void            mouseReleased();

        //void            keyPressed(int key );
        //void            mousePressed(x, y, button);
        //void            mouseDragged(x, y, button);
        //void            mouseReleased(mouseX, mouseY, button);

};



#endif // GUIPANEL_H_INCLUDED
