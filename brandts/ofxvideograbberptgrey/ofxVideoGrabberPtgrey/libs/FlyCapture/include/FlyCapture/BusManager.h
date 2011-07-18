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
// $Id: BusManager.h,v 1.34 2009/04/24 15:51:32 soowei Exp $
//=============================================================================

#ifndef PGR_FC2_BUSMANAGER_H
#define PGR_FC2_BUSMANAGER_H

#include "FlyCapture2Platform.h"
#include "FlyCapture2Defs.h"

namespace FlyCapture2
{    
    class Error;
    class TopologyNode;

    /** 
     * Bus event callback function prototype. Defines the syntax of the
     * callback function that is passed into RegisterCallback() and
     * UnregisterCallback().
     */
    typedef void (*BusEventCallback)( void* pParameter );
    
    /** 
     * Handle that is returned when registering a callback. It is required
     * when unregistering the callback.
     */
    typedef void* CallbackHandle;    
    
    /**
     * The BusManager class provides the functionality for the user to get an 
     * PGRGuid for a desired camera or device easily. Once the
     * camera or device token is found, it can then be used to connect to the 
     * camera or device through the camera class or device class. In addition, 
     * the BusManager class provides the ability to be notified when a camera 
     * or device is added or removed or some event occurs on the PC.
     */ 
    class FLYCAPTURE2_API BusManager
    {
    public: 

        /**
         * Default constructor.
         */
        BusManager();

        /**
         * Default destructor.
         */
        virtual ~BusManager();

        /**
         * Gets the number of cameras attached to the PC.
         *
         * @param pNumCameras The number of cameras attached.
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetNumOfCameras( unsigned int* pNumCameras );
        
        /**
         * Gets the PGRGuid for a camera on the PC. It uniquely identifies
         * the camera specified by the index and is used to identify the camera
         * during a Camera::Connect() call.
         *
         * @param index Zero based index of camera.
         * @param pGuid Unique PGRGuid for the camera.
         *
         * @see GetCameraFromSerialNumber()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetCameraFromIndex( 
            unsigned int index, 
            PGRGuid*     pGuid );
        
        /**
         * Gets the PGRGuid for a camera on the PC. It uniquely identifies
         * the camera specified by the index and is used to identify the camera
         * during a Camera::Connect() call.
         *
         * @param serialNumber Serial number of camera.
         * @param pGuid Unique PGRGuid for the camera.
         *
         * @see GetCameraFromIndex()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetCameraFromSerialNumber( 
            unsigned int serialNumber, 
            PGRGuid*     pGuid );
        
        /**
         * Gets the serial number of the camera with the specified index.
         *
         * @param index Zero based index of desired camera.
         * @param pSerialNumber Serial number of camera.
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetCameraSerialNumberFromIndex( 
            unsigned int  index, 
            unsigned int* pSerialNumber );
        
        /**
         * Gets the topology information for the PC.
         *
         * @param pNode TopologyNode object that will contain the topology 
         *              information.
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error GetTopology(
            TopologyNode* pNode );

        /**
         * Register a callback function that will be called when the
         * specified callback event occurs.
         *
         * @param busEventCallback Pointer to function that will received
                                   the callback.
         * @param callbackType Type of callback to register for.
         * @param pParameter Callback parameter to be passed to callback.
         * @param pCallbackHandle Unique callback handle used for 
         *                        unregistering callback.
         *
         * @see UnregisterCallback()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error RegisterCallback( 
            BusEventCallback    busEventCallback, 
            BusCallbackType     callbackType,
            void*               pParameter,
            CallbackHandle*     pCallbackHandle );
        
        /**
         * Unregister a callback function.
         *
         * @param callbackHandle Unique callback handle.
         *
         * @see RegisterCallback()
         *
         * @return An Error indicating the success or failure of the function.
         */ 
        virtual Error UnregisterCallback( CallbackHandle callbackHandle );

    private:

        BusManager( const BusManager& );
        BusManager& operator=( const BusManager& );

        struct BusManagerData; // Forward declaration

        BusManagerData* m_pBusManagerData;


    };

}

#endif //PGR_FC2_BUSMANAGER_H
