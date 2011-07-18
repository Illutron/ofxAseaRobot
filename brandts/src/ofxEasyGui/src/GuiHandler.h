#ifndef GUIHANDLER_H_INCLUDED
#define GUIHANDLER_H_INCLUDED

//--------------------------------------------------------------
#include "ofMain.h"

#include "ofxDirList.h"
#include "ofxVectorMath.h"
#include "ofxXmlSettings.h"

#include "GuiPanel.h"

class   GuiHandler // : public ListenerClass //: public ofMouseListener, public ofKeyListener, public ofAppListener
{
    //Handles overall control of the User interface.

    private:
        //VARIABLES:--------------------------------------------------
        bool    isHidden;
        int     hideKey;
        bool    keysWhileHidden;
        bool    isTypingBlocked;


    public:
        GuiHandler();                           //constructor
        ~GuiHandler();                          //destructor
        void             initialize();          //initialize objects.

        //METHODS:--------------------------------------------------------

        bool            updateBool(int idNum);
        float           updateFloat(int idNum);
        string          updateString(int idNum);
        int             updateInt(int idNum);
        bool            listenForTrigger(int idNum);

        // bool            update(int idNum);
        void            draw(int x, int y);
        vector          <GuiPanel*> panelList;
        GuiPanel*       addPanel(string name,int x,int y,int w, int h);
        void            draw();


  	//void	keyPressed(int key);
	//void	mouseMoved(int x, int y);
//	void	mousePressed(int x, int y, int button);


        void            mouseMoved(ofMouseEventArgs& _args);
        void            mousePressed(ofMouseEventArgs& _args);
        void            mouseDragged(ofMouseEventArgs& _args);
        void            mouseReleased(ofMouseEventArgs& _args);
        void            keyPressed(ofKeyEventArgs& _args);
        void            keyReleased(ofKeyEventArgs& _args);

        void            hide();
        void            unHide();
        void            setHideKey(int key);

      //  void            setKeysWhileHidden(bool yesNo);
        bool             isSafeToType();
        void             disableKeys();
        void             enableKeys();
        bool             isGuiHidden();
        //void            mouseDragged(x, y, button);
        //void            mouseReleased(mouseX, mouseY, button);
	
		// XML SETTINGS
		void saveToXML(string file);
		void loadFromXML(string file);
		ofxXmlSettings					XML;

};



#endif // GUIHANDLER_H_INCLUDED
