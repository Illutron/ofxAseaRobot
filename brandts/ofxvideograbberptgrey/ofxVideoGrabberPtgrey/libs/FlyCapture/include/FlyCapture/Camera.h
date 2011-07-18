//=============================================================================
// Copyright © 2008 Point Grey Research, Inc. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with Point Grey Research, Inc. (PGR).
// 
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

//=============================================================================
// $Id: Camera.h,v 1.81 2009/06/10 21:50:36 soowei Exp $
//=============================================================================

#ifndef PGR_FC2_CAMERA_H_
#define PGR_FC2_CAMERA_H_

#include "FlyCapture2Platform.h"
#include "FlyCapture2Defs.h"

namespace FlyCapture2
{    
    class Error;
    class Image;

    /** 
     * Image event callback function prototype. Defines the syntax of the
     * image callback function that is passed into StartCapture().
     */
    typedef void (*ImageEventCallback)( class Image* pImage,void* pCallbackData );

    /**
     * The Camera object represents a physical camera. The object must first
     * be connected to using Connect() before any other operations can proceed.
     *
     * @nosubgrouping
     */
    class FLYCAPTURE2_API Camera
    {
    public:

        /**
         * Default constructor.
         */
        Camera();

        /**
         * Default destructor.
         */
        virtual ~Camera();

        /** 
         * @name Connection and Image Retrieval
         *
         * These functions deal with connections and image retrieval from
         * the camera.
         */
        /*@{*/ 

        /**
         * Connects the camera object to the camera specified by the GUID.
         * If the guid is omitted or set to NULL, the connection will be made 
         * to the first camera detected on the PC (i.e. index = 0).
         *
         * @param pGuid The unique identifier for a specific camera on the PC.
         *
         * @see BusManager::GetCameraFromIndex()
         * @see BusManager::GetCameraFromSerialNumber()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error Connect( PGRGuid* pGuid = NULL );        

        /**
         * Disconnects the camera object from the camera. This allows another
         * physical camera to be connected to the camera object.
         *
         * @see Connect()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error Disconnect();

        /**
         * Checks if the camera object is currently connected to a physical
         * camera.
         *
         * @see Connect()
         * @see Disconnect()
         *
         * @return Whether the camera object is connected to a physical camera.
         */ 
        virtual bool IsConnected();
        
        /**
         * Starts isochronous image capture. It will use either the current
         * video mode or the most recently set video mode of the camera.
         * The optional callback function parameter is called on completion of 
         * image transfer. Alternatively, the callback parameter can
         * be set to NULL and RetrieveBuffer() can be called as a blocking
         * call to get the image data.
         *
         * @param callbackFn A function to be called when a new image is
         *                   received.
         * @param pCallbackData A pointer to data that can be passed to
         *                      to the callback function.
         *
         * @see RetrieveBuffer()
         * @see StartSyncCapture()
         * @see StopCapture()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error StartCapture( 
            ImageEventCallback callbackFn = NULL, 
            void* pCallbackData = NULL );
        
        /**
         * Starts isochronous image capture on multiple cameras. On each frame, 
         * the time stamps across the cameras are aligned which means the frames 
         * are synchronized.
         *
         * @param numCameras Number of Camera objects in the ppCameras array.
         * @param ppCameras Array of pointers to Camera objects containing the
         *                  cameras to be started and synchronized.
         * @param pCallbackFns Array of callback functions for each camera.
         *
         * @see RetrieveBuffer()
         * @see StartCapture()
         * @see StopCapture()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        static Error StartSyncCapture( 
            int numCameras,
            const Camera **ppCameras, 
            const ImageEventCallback *pCallbackFns = NULL);
        
        /**
         * Retrieves the the next image object containing the next image.
         * If the grab mode has not been set, or has been set to DROP_FRAMES
         * the default behavior is to requeue images for DMA if they have
         * not been retrieved by the time the next image transfer completes.
         * If BUFFER_FRAMES is specified, the next image in the sequence will 
         * be retrieved.  Note that for the BUFFER_FRAMES case, if retrieval 
         * does not keep up with the DMA process, images will be lost.  
         * The default behavior is to perform DROP_FRAMES image retrieval.   
         *
         * @param pImage Pointer to Image object to store image data.
         *
         * @see StartCapture()
         * @see StopCapture()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error RetrieveBuffer( Image* pImage );

        /**
         * Stops isochronous image transfer and cleans up all associated
         * resources.
         *
         * @see StartCapture()
         * @see RetrieveBuffer()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error StopCapture();

        /**
         * Retrieves the next image event containing the next part of the image.
         *
         * @param pImage Pointer to Image object to store image data.
         * @param eventNumber The event number to wait for.
         *
         * @see StartCapture()
         * @see RetrieveBuffer()
         * @see StopCapture()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error WaitForBufferEvent( Image* pImage, unsigned int eventNumber );

        /**
         * Specify user allocated buffers to use as image data buffers.
         *
         * @param pMemBuffers Array of memory buffers to be written to.
         * @param size The size of each buffer (in bytes).
         * @param numBuffers Number of buffers in the array.
         *
         * @see StartCapture()
         * @see RetrieveBuffer()
         * @see StopCapture()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetUserBuffers(
            unsigned char* const    pMemBuffers,
            int                     size,
            int                     numBuffers );

        /**
         * Get the configuration associated with the camera object.
         *
         * @param pConfig Pointer to the configuration structure to be filled.
         *
         * @see SetConfiguration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetConfiguration( FC2Config* pConfig );

        /**
         * Set the configuration associated with the camera object.
         *
         * @param pConfig Pointer to the configuration structure to be used.
         *
         * @see GetConfiguration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetConfiguration( const FC2Config* pConfig );

        /*@}*/

        /** 
         * @name Information and Properties
         *
         * These functions deal with information and properties can be
         * retrieved from the camera.
         */
        /*@{*/ 

        /**
         * Retrieves information from the camera such as serial number, model
         * name and other camera information.
         *
         * @param pCameraInfo Pointer to the camera information structure
         *                    to be filled.
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetCameraInfo( CameraInfo* pCameraInfo );
        
        /**
         * Retrieves information about the specified camera property. The 
         * property type must be specified in the PropertyInfo structure 
         * passed into the function in order for the function to succeed.
         *
         * @param pPropInfo Pointer to the PropertyInfo structure to be filled.
         *
         * @see GetProperty()
         * @see SetProperty()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetPropertyInfo( PropertyInfo* pPropInfo );

        /**
         * Reads the settings for the specified property from the camera. The 
         * property type must be specified in the Property structure passed
         * into the function in order for the function to succeed. If auto
         * is on, the integer and abs values returned may not be consistent
         * with each other.
         *
         * @param pProp Pointer to the Property structure to be filled.
         *
         * @see GetPropertyInfo()
         * @see SetProperty()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetProperty( Property* pProp );
        
        /**
         * Writes the settings for the specified property to the camera. The 
         * property type must be specified in the Property structure passed
         * into the function in order for the function to succeed.
         * The absControl flag controls whether the absolute or integer value
         * is written to the camera.
         *
         * @param pProp Pointer to the Property structure to be used.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see GetPropertyInfo()
         * @see GetProperty()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetProperty( 
            const Property* pProp, 
            bool            broadcast = false );

        /*@}*/

        /** 
         * @name General Purpose Input / Output
         *
         * These functions deal with general GPIO pin control on the camera.
         */
        /*@{*/ 

        /**
         * Get the GPIO pin direction for the specified pin. This is not a
         * required call when using the trigger or strobe functions as
         * the pin direction is set automatically internally.
         *
         * @param pin Pin to get the direction for.
         * @param pDirection Direction of the pin. 0 for input, 1 for output.
         *
         * @see SetGPIOPinDirection()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetGPIOPinDirection( unsigned int pin, unsigned int* pDirection);
        
        /**
         * Set the GPIO pin direction for the specified pin. This is useful if
         * there is a need to set the pin into an input pin (i.e. to read the
         * voltage) off the pin without setting it as a trigger source. This 
         * is not a required call when using the trigger or strobe functions as
         * the pin direction is set automatically internally.
         *
         * @param pin Pin to get the direction for.
         * @param direction Direction of the pin. 0 for input, 1 for output.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see GetGPIOPinDirection()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetGPIOPinDirection( unsigned int pin, unsigned int direction, bool broadcast = false );    

        /*@}*/       

        /** 
         * @name Trigger
         *
         * These functions deal with trigger control on the camera.
         */
        /*@{*/ 
        
        /**
         * Retrieve trigger information from the camera.
         *
         * @param pTriggerModeInfo Structure to receive trigger information.
         *
         * @see GetTriggerMode()
         * @see SetTriggerMode()
         * @see GetTriggerDelayInfo()
         * @see GetTriggerDelay()
         * @see SetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetTriggerModeInfo( TriggerModeInfo* pTriggerModeInfo );
        
        /**
         * Retrieve current trigger settings from the camera.
         *
         * @param pTriggerMode Structure to receive trigger mode settings.
         *
         * @see GetTriggerModeInfo()
         * @see SetTriggerMode()
         * @see GetTriggerDelayInfo()
         * @see GetTriggerDelay()
         * @see SetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetTriggerMode( TriggerMode* pTriggerMode );
        
        /**
         * Set the specified trigger settings to the camera.
         *
         * @param pTriggerMode Structure providing trigger mode settings.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see GetTriggerModeInfo()
         * @see GetTriggerMode()
         * @see GetTriggerDelayInfo()
         * @see GetTriggerDelay()
         * @see SetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetTriggerMode( 
            const TriggerMode*  pTriggerMode,
            bool                broadcast = false );

        /**
         * Fire the software trigger according to the DCAM specifications.
         *
         * @param broadcast Whether the action should be broadcast.
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error FireSoftwareTrigger( bool broadcast = false );
        
        /**
         * Retrieve trigger delay information from the camera.
         *
         * @param pTriggerDelayInfo Structure to receive trigger delay information.
         *
         * @see GetTriggerModeInfo()
         * @see GetTriggerMode()
         * @see SetTriggerMode()
         * @see GetTriggerDelay()
         * @see SetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetTriggerDelayInfo( TriggerDelayInfo* pTriggerDelayInfo );
        
        /**
         * Retrieve current trigger delay settings from the camera.
         *
         * @param pTriggerDelay Structure to receive trigger delay settings.
         * 
         * @see GetTriggerModeInfo()
         * @see GetTriggerMode()
         * @see SetTriggerMode()         
         * @see GetTriggerDelayInfo()
         * @see SetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetTriggerDelay( TriggerDelay* pTriggerDelay );
        
        /**
         * Set the specified trigger delay settings to the camera.
         *
         * @param pTriggerDelay Structure providing trigger delay settings.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see GetTriggerModeInfo()
         * @see GetTriggerMode()
         * @see SetTriggerMode()         
         * @see GetTriggerDelayInfo()
         * @see GetTriggerDelay()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetTriggerDelay( 
            const TriggerDelay* pTriggerDelay, 
            bool                broadcast = false );

        /*@}*/       

        /** 
         * @name Strobe
         *
         * These functions deal with strobe control on the camera.
         */
        /*@{*/ 

        /**
         * Retrieve strobe information from the camera.
         *
         * @param pStrobeInfo Structure to receive strobe information.
         *
         * @see GetStrobe()
         * @see SetStrobe()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetStrobeInfo( StrobeInfo* pStrobeInfo );
        
        /**
        * Retrieve current strobe settings from the camera. The strobe pin
        * must be specified in the structure before being passed in to
        * the function.
        *
        * @param pStrobeControl Structure to receive strobe settings.
        * 
        * @see GetStrobeInfo()
        * @see SetStrobe()
        *
        * @return An Error indicating the success or failure of the function.
        */ 
        virtual Error GetStrobe( StrobeControl* pStrobeControl );
        
        /**
         * Set current strobe settings to the camera. The strobe pin
         * must be specified in the structure before being passed in to
         * the function.
         *
         * @param pStrobeControl Structure providing strobe settings.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see GetStrobeInfo()
         * @see GetStrobe()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetStrobe( 
            const StrobeControl* pStrobeControl,
            bool                 broadcast = false );

        /*@}*/       

        /** 
         * @name DCAM Formats
         *
         * These functions deal with DCAM video mode and frame rate 
         * on the camera.
         */
        /*@{*/ 

        /**
         * Query the camera to determine if the specified video mode and 
         * frame rate is supported.
         *
         * @param videoMode Video mode to check.
         * @param frameRate Frame rate to check.
         * @param pSupported Whether the video mode and frame rate is
         *                   supported.
         *
         * @see GetVideoModeAndFrameRate()
         * @see SetVideoModeAndFrameRate()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetVideoModeAndFrameRateInfo(
            VideoMode videoMode,
            FrameRate frameRate,
            bool*     pSupported);
        
        /**
         * Get the current video mode and frame rate from the camera. If
         * the camera is in Format7, the video mode will be VIDEOMODE_FORMAT7
         * and the frame rate will be FRAMERATE_FORMAT7.
         *
         * @param pVideoMode Current video mode.
         * @param pFrameRate Current frame rate.
         *
         * @see GetVideoModeAndFrameRateInfo()
         * @see SetVideoModeAndFrameRate()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetVideoModeAndFrameRate(
            VideoMode* pVideoMode,
            FrameRate* pFrameRate );

        /**
         * Set the specified video mode and frame rate to the camera. It is
         * not possible to set the camera to VIDEOMODE_FORMAT7 or 
         * FRAMERATE_FORMAT7. Use the Format7 functions to set the camera
         * into Format7.
         *
         * @param videoMode Video mode to set to camera.
         * @param frameRate Frame rate to set to camera.
         *
         * @see GetVideoModeAndFrameRateInfo()
         * @see GetVideoModeAndFrameRate()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetVideoModeAndFrameRate(
            VideoMode videoMode,
            FrameRate frameRate );

        /*@}*/       

        /** 
         * @name Format7
         *
         * These functions deal with Format7 custom image control on the camera.
         */
        /*@{*/ 
        
        /**
         * Retrieve the availability of Format7 custom image mode and the
         * camera capabilities for the specified Format7 mode. The mode must
         * be specified in the Format7Info structure in order for the
         * function to succeed.
         *
         * @param pInfo Structure to be filled with the capabilities of the
         *              specified mode and the current state in the specified
         *              mode.
         * @param pSupported Whether the specified mode is supported.
         *
         * @see ValidateFormat7Settings()
         * @see GetFormat7Configuration()
         * @see SetFormat7Configuration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetFormat7Info( 
            Format7Info*   pInfo,
            bool*          pSupported );
        
        /**
         * Validates Format7ImageSettings structure and returns valid packet
         * size information if the image settings are valid. The current
         * image settings are cached while validation is taking place. The
         * cached settings are restored when validation is complete.
         *
         * @param pImageSettings Structure containing the image settings.
         * @param pSettingsAreValid Whether the settings are valid.
         * @param pPacketInfo Packet size information that can be used to
         *                    determine a valid packet size.
         *
         * @see GetFormat7Info()
         * @see GetFormat7Configuration()
         * @see SetFormat7Configuration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error ValidateFormat7Settings( 
            const Format7ImageSettings*   pImageSettings,
            bool*                   pSettingsAreValid,
            Format7PacketInfo*      pPacketInfo );
        
        /**
         * Get the current Format7 configuration from the camera.
         *
         * @param pImageSettings Current image settings.
         * @param pPacketSize Current packet size.
         * @param pPercentage Current packet size as a percentage.
         *
         * @see GetFormat7Info()
         * @see ValidateFormat7Settings()
         * @see SetFormat7Configuration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetFormat7Configuration( 
            Format7ImageSettings* pImageSettings,
            unsigned int*         pPacketSize,
            float*                pPercentage );
        
        /**
         * Set the current Format7 configuration to the camera.
         *
         * @param pImageSettings Image settings to be written to the camera.
         * @param packetSize Packet size to be written to the camera.
         *
         * @see GetFormat7Info()
         * @see ValidateFormat7Settings()
         * @see GetFormat7Configuration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetFormat7Configuration( 
            const Format7ImageSettings* pImageSettings,
            unsigned int                packetSize );

        /**
         * Set the current Format7 configuration to the camera.
         *
         * @param pImageSettings Image settings to be written to the camera.
         * @param percentSpeed Percentage of packet size to be written to
         *                     the camera.
         *
         * @see GetFormat7Info()
         * @see ValidateFormat7Settings()
         * @see GetFormat7Configuration()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetFormat7Configuration(
            const Format7ImageSettings* pImageSettings,
            float                       percentSpeed );

        /*@}*/       

        /** 
         * @name Look Up Table
         *
         * These functions deal with Look Up Table control on the camera.
         */
        /*@{*/ 
        
        /**
         * Query if LUT support is available on the camera.
         *
         * @param pData The LUT structure to be filled.
         *
         * @see EnableLUT()
         * @see GetLUTChannel()
         * @see SetLUTChannel()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetLUTInfo( LUTData* pData );

        /**
         * Query the read/write status of a single LUT bank.
         *
         * @param bank The bank to query.
         * @param pReadSupported Whether reading from the bank is supported.
         * @param pWriteSupported Whether writing to the bank is supported.
         *
         * @return An Error indicating the success or failure of the function.
         */
        virtual Error GetLUTBankInfo(
            unsigned int bank,
            bool* pReadSupported,
            bool* pWriteSupported );

        /**
         * Get the LUT bank that is currently being used. For cameras with
         * PGR LUT, the active bank is always 0.
         *
         * @param pActiveBank The currently active bank.
         *
         * @return An Error indicating the success or failure of the function.
         */
        virtual Error GetActiveLUTBank( unsigned int* pActiveBank );

        /**
         * Set the LUT bank that will be used.
         *
         * @param activeBank The bank to be set as active.
         *
         * @return An Error indicating the success or failure of the function.
         */
        virtual Error SetActiveLUTBank( unsigned int activeBank );

        /**
         * Enable or disable LUT functionality on the camera.
         *
         * @param on Whether to enable or disable LUT.
         *
         * @see GetLUTInfo()
         * @see GetLUTChannel()
         * @see SetLUTChannel()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error EnableLUT( bool on );
        
        /**
         * Get the LUT channel settings from the camera.
         *
         * @param bank Bank to retrieve.
         * @param channel Channel to retrieve.
         * @param sizeEntries Number of entries in LUT table to read.
         * @param pEntries Array to store LUT entries.
         *
         * @see GetLUTInfo()
         * @see EnableLUT()
         * @see SetLUTChannel()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetLUTChannel(
            unsigned int  bank,
            unsigned int  channel,
            unsigned int  sizeEntries,
            unsigned int* pEntries );
        
        /**
         * Set the LUT channel settings to the camera.
         *
         * @param bank Bank to set.
         * @param channel Channel to set.
         * @param sizeEntries Number of entries in LUT table to write.
         * @param pEntries Array containing LUT entries to write.
         *
         * @see GetLUTInfo()
         * @see EnableLUT()
         * @see GetLUTChannel()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetLUTChannel(
            unsigned int        bank,
            unsigned int        channel,
            unsigned int        sizeEntries,
            const unsigned int* pEntries );

        /*@}*/       

        /** 
         * @name Memory Channels
         *
         * These functions deal with memory channel control on the camera.
         */
        /*@{*/ 
        
        /**
         * Retrieve the current memory channel from the camera.
         *
         * @param pCurrentChannel Current memory channel.
         *
         * @see SaveToMemoryChannel()
         * @see RestoreFromMemoryChannel()
         * @see GetMemoryChannelInfo()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetMemoryChannel( unsigned int* pCurrentChannel );
        
        /**
         * Save the current settings to the specfied current memory channel.
         *
         * @param channel Memory channel to save to.
         *
         * @see GetMemoryChannel()
         * @see RestoreFromMemoryChannel()
         * @see GetMemoryChannelInfo()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SaveToMemoryChannel( unsigned int channel );
        
        /**
         * Restore the specfied current memory channel.
         *
         * @param channel Memory channel to restore from.
         *
         * @see GetMemoryChannel()
         * @see SaveToMemoryChannel()
         * @see GetMemoryChannelInfo()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error RestoreFromMemoryChannel( unsigned int channel );
        
        /**
         * Query the camera for memory channel support. If the number of 
         * channels is 0, then memory channel support is not available.
         *
         * @param pNumChannels Number of memory channels supported.
         *
         * @see GetMemoryChannel()
         * @see SaveToMemoryChannel()
         * @see RestoreFromMemoryChannel()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetMemoryChannelInfo( unsigned int* pNumChannels );

        /*@}*/       

        /** 
         * @name Embedded Image Information
         *
         * These functions deal with embedded image information control 
         * on the camera.
         */
        /*@{*/ 

        /**
         * Get the current status of the embedded image information register,
         * as well as the availability of each embedded property.
         *
         * @param pInfo Structure to be filled.
         *
         * @see SetEmbeddedImageInfo()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetEmbeddedImageInfo( EmbeddedImageInfo* pInfo );
        
        /**
         * Sets the on/off values of the embedded image information structure
         * to the camera.
         *
         * @param pInfo Structure to be used.
         *
         * @see GetEmbeddedImageInfo()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error SetEmbeddedImageInfo( EmbeddedImageInfo* pInfo );

        /*@}*/       

        /** 
         * @name Register Operation
         *
         * These functions deal with register operation on the camera.
         */
        /*@{*/ 
        
        /**
         * Write to the specified register on the camera.
         *
         * @param address DCAM address to be written to.
         * @param value The value to be written.
         * @param broadcast Whether the action should be broadcast.
         *
         * @see ReadRegister()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error WriteRegister( 
            unsigned int address, 
            unsigned int value,
            bool broadcast=false);
        
        /**
         * Read the specified register from the camera.
         *
         * @param address DCAM address to be read from.
         * @param pValue The value that is read.
         *
         * @see WriteRegister()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error ReadRegister( 
            unsigned int  address, 
            unsigned int* pValue );
        
        /**
         * Write to the specified register block on the camera.
         *
         * @param addressHigh Top 16 bits of the 48 bit absolute address to
         *                    write to.
         * @param addressLow Bottom 32 bits of the 48 bits absolute address to
         *                   write to.
         * @param pBuffer Array containing data to be written.
         * @param length Size of array, in quadlets.
         *
         * @see ReadRegisterBlock()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error WriteRegisterBlock(
            unsigned short       addressHigh,
            unsigned int         addressLow,
            const unsigned int*  pBuffer,
            unsigned int         length );
        
        /**
         * Read from the specified register block on the camera.
         *
         * @param addressHigh Top 16 bits of the 48 bit absolute address to
         *                    read from.
         * @param addressLow Bottom 32 bits of the 48 bits absolute address to
         *                   read from.
         * @param pBuffer Array to store read data.
         * @param length Size of array, in quadlets.
         *
         * @see WriteRegisterBlock()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error ReadRegisterBlock(
            unsigned short addressHigh,
            unsigned int   addressLow,
            unsigned int*  pBuffer,
            unsigned int   length );      

        /**
         * Returns a text representation of the register value.
         *
         * @param registerVal The register value to query.
         *
         * @return The text representation of the register.
         */ 
        static const char* GetRegisterString( unsigned int registerVal);

        /*@}*/       

    protected:

    private:   

        Camera( const Camera& );
        Camera& operator=( const Camera& );

        struct CameraData; // Forward declaration

        CameraData* m_pCameraData;

    };
}

#endif // PGR_FC2_CAMERA_H_
