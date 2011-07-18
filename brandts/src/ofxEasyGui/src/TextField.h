#ifndef TEXTFIELD_H_INCLUDED
#define TEXTFIELD_H_INCLUDED

#include "ofMain.h"

#include "UiObject.h"

class   UiTextField: public UiObject
{
    //Handles overall control of the User interface.
    private:
        bool            inTextEnterMode;
        bool            replaceAllText;
        string          myDefaultText;
        string *        myPointValue;


    public:
        UiTextField();                           //constructor
        ~UiTextField();                          //destructor
        void             initialize(string name,int x,int y,int w, int h, string defaultText, int idNum, string * pointValue);

        //METHODS:--------------------------------------------------------


        void            draw();
        void            press();
        void            dePress();//releases the text focus whn pressed outside of area.
        void            keyPressed(int key);
        bool            keyPressOK(); //blocks other keys from activating while typing.
        //void            release();

       // float           getValue();

};




#endif // TEXTFIELD_H_INCLUDED
