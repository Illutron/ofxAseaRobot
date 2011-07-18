#ifndef SPACER_H_INCLUDED
#define SPACER_H_INCLUDED

#include "ofMain.h"
#include "UiObject.h"

class   UiButton: public UiObject
{
    //Handles overall control of the User interface.
    private:
        string      myMode;  //eihter "TRIGGER" or "SWITCH"
        bool        textOverButton;
        bool        fireTrigger;
        bool *      myPointValue;


    public:
        UiButton();                           //constructor
        ~UiButton();                          //destructor
        void             initialize(string name,int x,int y,int w, int h, string bmode,bool textOnTop,int idNum, bool * pointValue);

        //METHODS:--------------------------------------------------------


        void            draw();
        void            press();
        void            release();
        void            keyPressed(int key);
        void            keyReleased(int key);
        bool            getValueBool();
        bool            listenForTrigger();

};

#endif // SPACER_H_INCLUDED
