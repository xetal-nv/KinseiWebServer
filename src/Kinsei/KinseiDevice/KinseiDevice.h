/**
 * \file KinseiDevice.h
 * This file contains the KinseiDevice handling class
 *
 * \author Marco Camilli
 * \copyright Property Xetal NV
 *
 * \license MIT
 * \email marco@xetal.eu
 */

#ifndef _KINSEIDEVICE_H
#define _KINSEIDEVICE_H

#include "../KinseiCommon/clsocket/ActiveSocket.h"
#include <vector>
#include <string>


using namespace std;

/**
 * Simple structure to define a point in the cartesian plane
 */
struct point{
    float x;
    float y;
};

/**
 * Defines a zone
 */
struct zone{
    unsigned int type; ///1 polygonal zone, 0 circular zone (see MOCA server sources)
    vector<point> points; ///Used by the polygonal zone, array of vertexes
    float radius; ///Used by circular zone, radius of the circle
    point center; ///Used by circular zone, circle center
};

/**
 * Defines a fusion pixel
 */
struct fusion{
    point centroid; /// Fusion pixel position
    int val; /// Fusion pixel relative estimated value
};



/**
 * Defines a sensor
 */
struct sensor{
    bool online; /// 1 if online, 0 otherwise
    point position; /// Position of the sensor
};

/**
 * Define ThermalMap Pixel
 */
struct thermalmap_str{
    unsigned int X; /// Ncolumns
    unsigned int Y; ///Nrows
    unsigned int size_cm;
    vector<vector<float>> tmap;
};


/**
 * Defines the KinseiDevice class
 *
 * It handles the TCP connection with the device
 *
 */
class KinseiDevice{

public:

        /**
         * Empty Constructor
         * In order to connect it needs to know IP and PORT
         */
        KinseiDevice();

        /**
         * Initialize the TCP connection
         *
         * @param ServAddress Ip address of the device
         * @param ServPort Port number where the device is listening
         *
         */
        KinseiDevice(const char *  ServAddress, uint16_t ServPort);

        /**
         * Destructor
         *
         * It closes the TCP connection
         */
        ~KinseiDevice();


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
         * Check if the TCP connection is up
         */
        bool IsConnected();

        /**
         * It gets the room sizes form the device configuration
         *
         * @param MaxSize Pointer to a point variable, the function fills it with the bounding box size of the room
         * @param Corners Vector of point passed by reference, the function fill it with the room vertexes
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetRoomSize(point* MaxSize, vector<point>& Corners );


        /**
         * It gets the room entry and exit zones
         *
         * @param Zones Vector of zone passed by reference, the function fill it with the zones vertexes
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetZones(vector<zone>& Zones);

        /**
         * It gets the current fusion status
         *
         * @param Fusion Vector of fusion passed by reference, the function fill it with the fusion status
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetFusion(vector<fusion>& Fusion);


        /**
         * It gets the current Tracking Positions
         *
         * @param dpositions Vector of point passed by reference, the function fill it with the current tracking positions
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetPositions(vector<point>& dpositions);

        /**
         * It gets the current number of people
         * The number of people is not post processed and it is not always an integer number
         *
         * @param np The function fill it with the estimated raw number of people.
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetNPersons(float * np);

        /**
         * It gets the current number of people
         * The number of people is post processed by the device itself using a smith trigger
         *
         * @param np The function fill it with the estimate number of people.
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetNPersonsInt(unsigned int * np);

        /**
         * It gets the current device status
         * It report the device status connection with its own sensors not the connection status between client and device
         *
         * @param status 1 if online, 0 otherwise
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetOnlineStatus(bool* status);

        /**
         * It gets the sensor data and current status
         *
         * @param sen Vector ofsensor passed by reference, the function fill it with the sensors information
         *
         * @return bool if successful, 0 otherwise
         */
        int GetSensorsStatus(vector<sensor>& sen);



        /**
         * It gets the the thermal map
         *
         * @param thermalmap_str passed by reference, the function fill it with the thermalmap information
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetThermalMap(thermalmap_str& thermalmap);



        /**
         *  Firmware > 12/2017
         *
         * It gets the the thermistors temperatures
         *
         * @param vector<float> passed by reference, the function fill it with the thermistors values
         *
         * @return bool if successful, 0 otherwise
         */
        bool GetThermistors(vector<float>& thermistors);


private:

        CActiveSocket *sock; ///Class that handles the TCP basic communication

};


#endif
