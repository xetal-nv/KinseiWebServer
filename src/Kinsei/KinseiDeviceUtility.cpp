#include "KinseiDeviceUtility.h"

#define NCONFIGS 2


using namespace std;


#ifdef WIN32
//#include <mmsystem.h>
#include <windows.h>
#else
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#endif

#include<json/writer.h>
#include <sstream>
#include <iostream>


void KinseiDeviceUtility::SleepMs(int ms) {
    #ifdef WIN32
    Sleep(ms);
    #else
    usleep(ms*1000); //convert to microseconds
    #endif
    return;
}


KinseiDeviceUtility::KinseiDeviceUtility(const char* Address, uint16_t Port){
    kinseidata_updated = true;
    pthread_mutex_init (&sem_mem, NULL);
    pthread_mutex_init (&sem_device, NULL);
    Connect(Address, Port);
}





KinseiDeviceUtility::KinseiDeviceUtility(){
    kinseidata_updated = true;
    pthread_mutex_init (&sem_mem, NULL);
    pthread_mutex_init (&sem_device, NULL);
}


KinseiDeviceUtility::~KinseiDeviceUtility(){
    Disconnect();
}

bool KinseiDeviceUtility::Connect(const char* Address, uint16_t Port){
        bool ret=true;
        pthread_mutex_lock (&sem_device);
        ret=device.connect(Address,Port);
        pthread_mutex_unlock (&sem_device);
        return ret;

}

bool KinseiDeviceUtility::IsConnected(){
        bool ret=true;
        pthread_mutex_lock (&sem_device);
        ret=device.IsConnected();
        pthread_mutex_unlock (&sem_device);
        return ret;

}

bool KinseiDeviceUtility::Start(){
    bool ret=true;

    pthread_mutex_lock (&sem_device);

    ret&=device.GetRoomSize(&kinseiData_int.room_size,kinseiData_int.room_corners);
    ret&=device.GetZones(kinseiData_int.zones);

    pthread_mutex_unlock (&sem_device);


    if(ret){
        pthread_mutex_lock (&sem_mem);
        kinseiData=kinseiData_int;
        pthread_mutex_unlock (&sem_mem);
    }

    return ret;

}

bool KinseiDeviceUtility::Update(){

    bool ret;

    pthread_mutex_lock (&sem_device);

    ret=device.GetOnlineStatus(&kinseiData_int.online);

    if(kinseiData_int.online==1)
        ret&= device.GetFusion(kinseiData_int.fusiondata) && device.GetPositions(kinseiData_int.dposition) && device.GetNPersons(&kinseiData_int.npersons) &&  device.GetNPersonsInt(&kinseiData_int.np) &&  device.GetThermalMap(kinseiData_int.thermalmap) &&  device.GetThermistors(kinseiData_int.thermistors);//
    else ret&=device.GetSensorsStatus(kinseiData_int.sensordata);

    pthread_mutex_unlock (&sem_device);

    pthread_mutex_lock (&sem_mem);
    if(ret){

        kinseiData=kinseiData_int;
        kinseidata_updated = true;

    }else kinseidata_updated = false;

    pthread_mutex_unlock (&sem_mem);

    return ret;
}

bool KinseiDeviceUtility::Disconnect(){

    pthread_mutex_lock (&sem_device);

    device.disconnect();

    pthread_mutex_unlock (&sem_device);

    return true;

}

void KinseiDeviceUtility::trd_KinseiDeviceUtility(void *ptr)
{

    KinseiDeviceUtility* kinseiUtility =(KinseiDeviceUtility *) ptr ;

	if(kinseiUtility->Start()){

        while(kinseiUtility->Update()){
             SleepMs(200);

        }

	}
	kinseiUtility->Disconnect();

}



bool KinseiDeviceUtility::StartThread(){

    if(pthread_create(&thread1, NULL,trd_KinseiDeviceUtilityFunc, this)) return false;
    else return true;

}

bool KinseiDeviceUtility::IsThreadRunning(){

    if(pthread_tryjoin_np( thread1, NULL ) == 0) return false;
    else return true;

}

bool KinseiDeviceUtility::StopThread(){

    return Disconnect();

}

bool KinseiDeviceUtility::GetJsonString(std::string &jsonstring){
    Json::Value my_json;

    pthread_mutex_lock (&sem_mem);

    //if(kinseidata_updated==false) return false;

    my_json["room_size"]["x"]=kinseiData.room_size.x;
    my_json["room_size"]["y"]=kinseiData.room_size.y;

    for(unsigned int i=0;i<kinseiData.room_corners.size();i++){
        my_json["room_corners"][i]["x"]=kinseiData.room_corners[i].x;
        my_json["room_corners"][i]["y"]=kinseiData.room_corners[i].y;
    }

    for(unsigned int i=0;i<kinseiData.zones.size();i++){
        my_json["zones"][i]["type"]=kinseiData.zones[i].type;
        if(kinseiData.zones[i].type == 0){
            my_json["zones"][i]["radius"]=kinseiData.zones[i].radius;
            my_json["zones"][i]["center"]["x"]=kinseiData.zones[i].center.x;
            my_json["zones"][i]["center"]["y"]=kinseiData.zones[i].center.y;
        }
        else{
            for(unsigned int j=0;j<kinseiData.zones[i].points.size();j++){
                my_json["zones"][i]["corners"][j]["x"]=kinseiData.zones[i].points[j].x;
                my_json["zones"][i]["corners"][j]["y"]=kinseiData.zones[i].points[j].y;
            }
        }
    }

    my_json["online"]=kinseiData.online;

    if(kinseiData.online){

        for(unsigned int i=0;i<kinseiData.dposition.size();i++){
            my_json["position"][i]["x"]=kinseiData.dposition[i].x;
            my_json["position"][i]["y"]=kinseiData.dposition[i].y;
        }

        for(unsigned int i=0;i<kinseiData.fusiondata.size();i++){
            my_json["fusion"][i]["x"]=kinseiData.fusiondata[i].centroid.x;
            my_json["fusion"][i]["y"]=kinseiData.fusiondata[i].centroid.y;
            my_json["fusion"][i]["val"]=kinseiData.fusiondata[i].val;
        }

        my_json["npersons_float"]=kinseiData.npersons;
        my_json["npersons_int"]=kinseiData.np;

        for(unsigned int i=0;i<kinseiData.thermistors.size();i++){
            my_json["thermistors"][i]=kinseiData.thermistors[i];
        }

        my_json["thermalmap"]["columns"]=kinseiData.thermalmap.X;
        my_json["thermalmap"]["rows"]=kinseiData.thermalmap.Y;
        my_json["thermalmap"]["pixel_cm"]=kinseiData.thermalmap.size_cm;
        if(kinseiData.thermalmap.tmap.size()>0)
            for(unsigned int i=0;i<kinseiData.thermalmap.tmap[0].size();i++)
                for(unsigned int j=0;j<kinseiData.thermalmap.tmap.size();j++)
                    my_json["thermalmap"]["map"][i][j]=kinseiData.thermalmap.tmap[i][j];


    }
    else for(unsigned int i=0;i<kinseiData.sensordata.size();i++){
            my_json["sensorstatus"][i]["x"]=kinseiData.sensordata[i].position.x;
            my_json["sensorstatus"][i]["y"]=kinseiData.sensordata[i].position.y;
            my_json["sensorstatus"][i]["online"]=kinseiData.sensordata[i].online;
        }





    pthread_mutex_unlock (&sem_mem);


    Json::StreamWriterBuilder wbuilder;
    wbuilder.settings_["precision"] = 5;
    jsonstring = Json::writeString(wbuilder, my_json);


    //stringstream buffer;
    //buffer<<my_json;
    //cout<<my_json;
    //jsonstring.assign(buffer.str());


    return true;
}





