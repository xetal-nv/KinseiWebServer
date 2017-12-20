/**
 * \file KinseiTuner.h
 * This file contains the KinseiTuner handling class
 *
 * \author Marco Camilli
 * \copyright Property Xetal NV
 */

#ifndef _KINSEITUNER_H
#define _KINSEITUNER_H

#include "../KinseiCommon/clsocket/ActiveSocket.h"
#include <vector>
#include <string>


using namespace std;


/**
 * Defines the KinseiTuner class
 *
 * It handles the TCP connection with the device
 *
 */
class KinseiTuner{

public:

        /**
         * Empty Constructor
         * In order to connect it needs to know IP and PORT
         */
        KinseiTuner();

        /**
         * Initialize the TCP connection
         *
         * @param ServAddress Ip address of the device
         * @param ServPort Port number where the device is listening
         *
         */
        KinseiTuner(const char *  ServAddress, uint16_t ServPort);

        /**
         * Destructor
         *
         * It closes the TCP connection
         */
        ~KinseiTuner();


        /**
         * Initializes the TCP connection closing the previous one if needed
         *
         * @param ServAddress Ip address of the device
         * @param ServPort Port number where the device is listening
         *
         */
        bool connect(const char *  ServAddress, uint16_t ServPort);

        /**
         * Closes the TCP connection
         */
        bool disconnect();

        /**
         * Reset Background
         *
         * @return bool if successful, 0 otherwise
         */
        bool ResetBG();

        /**
         * Reset Sensors Offset
         *
         * @return bool if successful, 0 otherwise
         */
        bool ResetOffset();


        /**
         * It gets the BG alpha
         *
         * @param float background alpha
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetBG_Alpha(float* alpha);

        /**
         * It sets the BG alpha
         *
         * @param float background alpha
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetBG_Alpha(float* alpha);


        /**
         * It gets the BG threshold
         *
         * @param float background threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetBG_Threshold(float* TH);

        /**
         * It sets the BG threshold
         *
         * @param float background threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetBG_Threshold(float* TH);

        /**
         * It gets the BG temperature threshold
         *
         * @param float background temperature threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetBG_Temperature_Threshold(float* TH);

        /**
         * It sets the BG temperature threshold
         *
         * @param float background temperature threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetBG_Temperature_Threshold(float* TH);


        /**
         * It gets the fusion BG threshold
         *
         * @param float fusion BG threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetFusion_BG_Threshold(float* TH);

        /**
         * It sets the fusion BG threshold
         *
         * @param float fusion BG threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetFusion_BG_Threshold(float* TH);

        /**
         * It gets the fusion consensum factor
         *
         * @param float fusion consensum factor
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetFusion_Consensum_Factor(float* CF);

        /**
         * It sets the fusion consensum factor
         *
         * @param float fusion consensum factor
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetFusion_Consensum_Factor(float* CF);

        /**
         * It gets the fusion threshold
         *
         * @param float  fusion threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetFusion_Threshold(float* FT);

        /**
         * It sets the fusion threshold
         *
         * @param float fusion threshold
         *
         * @return bool if successful, 0 otherwise
         */
        bool SetFusion_Threshold(float* FT);

        /**
         * Save override values
         *
         * @return bool if successful, 0 otherwise
         */
        bool SaveOverride();

        /**
         * Delete override
         *
         * @return bool if successful, 0 otherwise
         */
        bool DeleteOverride();



private:

        CActiveSocket *sock; ///Class that handles the TCP basic communication

};


#endif
