/**
 * \file KinseiDeviceUtility.h
 * This file contains the functions and the strctures used
 *
 * \author Marco Camilli
 * \copyright Property Xetal NV
 */

#ifndef _KINSEIDEVICEUTILITY_H
#define _KINSEIDEVICEUTILITY_H

#include <vector>
#include "../Kinsei/KinseiDevice/KinseiDevice.h"  // For Socket and SocketException

struct kinseidatastr{

    point room_size; ///max dimensions of the room
    vector<point> room_corners; ///Room vertexes
    vector<zone> zones; ///entry and exit zones
    vector<point> dposition; ///Position of the persons
    vector<fusion> fusiondata;
    float npersons; ///Number of persons
    unsigned int np; ///Number of persons after thresholding
    bool online; ///1 if the system is online
    vector<sensor> sensordata;
    thermalmap_str thermalmap;
    vector<float> thermistors;

};

/**
 * Defines the KinseiDeviceUtility class
 *
 * It handles the KinseiDevice
 *
 */
class KinseiDeviceUtility {

public:

    KinseiDeviceUtility();

    KinseiDeviceUtility(const char* Address, uint16_t Port);

    ~KinseiDeviceUtility();

    bool Connect(const char* Address, uint16_t Port);

    bool IsConnected();

    bool Start();

    bool Update();

    bool Disconnect();

    bool GetJsonString(std::string &jsonstring);

    bool StartThread();

    bool IsThreadRunning();

    bool StopThread();


protected:
   virtual void trd_KinseiDeviceUtility(void *ptr);


private:

    KinseiDevice device;

    pthread_t thread1;
    pthread_mutex_t sem_mem,sem_device;

    kinseidatastr kinseiData_int,kinseiData;

    bool kinseidata_updated = true;



    void SleepMs(int ms);

   static void * trd_KinseiDeviceUtilityFunc(void * ptr) {
       ((KinseiDeviceUtility *)ptr)->trd_KinseiDeviceUtility( ptr);
       return NULL;
   }

};


















#endif
