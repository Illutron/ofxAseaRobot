/*
 *  of1394VideoGrabber.h
 *  openFrameworks
 *
 *  Created by Zach Gage on 8/8/08.
 *  Copyright 2008 stfj. All rights reserved.
 *
 */
#ifndef _OF_1394_VIDEO_GRABBER
#define _OF_1394_VIDEO_GRABBER

#include "ofMain.h"
#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include <iostream>
#include "ofxThread.h"


#include <dc1394/dc1394.h>
//#include <dc1394/format7.h>
#include <dc1394/vendor/avt.h>


class of1394VideoGrabber : public ofxThread{
	
	public :
	
	of1394VideoGrabber();
	~of1394VideoGrabber();
	
	void 			listDevices();
	bool 			isFrameNew();
	void			grabFrame();
	int 			initGrabber();
	void			videoSettings();
	unsigned char 	* getPixels();
	void 			setVerbose(bool bTalkToMe);
	void			setDeviceID(int _deviceID);
	
	//five helper functions from Andreas Muller (http://www.nanikawa.com/)
	void			print_format( uint32_t format );					//prints the format of the supplied uint reference to the struct
	uint32_t		get_num_pixels(uint32_t format );					//Returns the number of pixels in the image based upon the format
	void			print_color_coding(uint32_t color_id );				//Prints the type of color encoding
	void			print_mode_info(dc1394video_mode_t mode );			//Prints various information about the mode the camera is in
	dc1394error_t	getBayerTile();	//trys to set Bayer Tile struct to value from Camera if applicable
	void setWhiteBalance(unsigned int a, unsigned int b);
	void setTemperature(unsigned int a);
	
	//Jonas functions
	float getExposure();
	void setExposure(float);
	void setAutoExposure(bool);
	float getShutter();
	void setShutter(float);
	void setAutoShutter(bool);
	float getFramerate();
	uint32_t getWidth();
	uint32_t getHeight();

	unsigned int			width, height;
	ofTexture 				tex;
	bool dying;
	bool failedToInit;
	dc1394camera_t *camera;
    dc1394featureset_t features;
    dc1394framerates_t framerates;
    dc1394video_modes_t video_modes;
    dc1394framerate_t framerate;
    dc1394video_mode_t video_mode;
    dc1394color_coding_t coding;
	dc1394color_filter_t bayer_pattern; //added by JGL for bayer pattern stuff, not required for now
	unsigned int frameCt, oldFrameCt;
    dc1394video_frame_t *frame;
    dc1394_t * d;
    dc1394camera_list_t * list;
	dc1394error_t err;
	void setGamma(float e);
	
	void threadedFunction(){
		while( isThreadRunning())
		{
			if(lock())
			{
				
				if(isFrameNew())
				{
					
					
					dc1394_capture_dequeue( camera, DC1394_CAPTURE_POLICY_WAIT, &frame ) ;
//int t=ofGetElapsedTimeMillis();
					memcpy( pixels2, frame->image, width * height) ; //3 channels, RGB
					dc1394_bayer_decoding_8bit(pixels2, pixels, width, height, DC1394_COLOR_FILTER_RGGB, DC1394_BAYER_METHOD_BILINEAR);
					dc1394_capture_enqueue( camera, frame ) ;
				//	cout<<ofGetElapsedTimeMillis()-t<<endl;
				}
				
				unlock();
				
				ofSleepMillis(5);
			}
			
		}
	}
	
protected:
	bool					bChooseDevice;
	int						deviceID;
	bool					bUseTexture;
	bool 					bVerbose;
	bool 					bGrabberInited;
	unsigned char * 		pixels;
	unsigned char * 		pixels2;

	bool 					bIsFrameNew;
};




#endif

