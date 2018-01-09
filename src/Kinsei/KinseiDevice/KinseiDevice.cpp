#include "KinseiDevice.h"
#include<iostream>

using namespace std;


KinseiDevice::KinseiDevice(){
    sock = new CActiveSocket();
    sock->Initialize();
    //sock->SetNonblocking();
    //sock->SetReceiveTimeout(100);

}

KinseiDevice::KinseiDevice(const char * ServAddress, uint16_t ServPort){
    sock = new CActiveSocket();
    sock->Initialize();
    //sock->SetNonblocking();
    //sock->SetReceiveTimeout(100);
    sock->Open(ServAddress,ServPort);

}

KinseiDevice::~KinseiDevice(){
    delete sock;
}

bool KinseiDevice::connect(const char *  ServAddress, uint16_t ServPort)
{
    return sock->Open(ServAddress,ServPort);

}




bool KinseiDevice::disconnect(){
    return sock->Close();
}


bool KinseiDevice::IsConnected(){
     if(sock == NULL || sock->IsSocketValid() == false) return false;
     else return true;
}

bool KinseiDevice::GetRoomSize(point* Max,vector<point>& Corners ){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;


        //Room max dimensions
        command=0x73;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each Receive()

        bytesReceived = sock->Receive(6,Buffer);
        if(Buffer[0] != 0x73){
            return 0;
        }
        if(bytesReceived == 6){
                    Max->x=( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/10;
                    Max->y=( ((unsigned int) Buffer[3] << 8) | (unsigned char)  Buffer[4] )/10;
        }
        else{
            return 0;
        }
        Corners.resize((unsigned int)Buffer[5]);
        bytesReceived = sock->Receive( Corners.size()*4, Buffer);
        if(bytesReceived == (int)Corners.size()*4){
                for(unsigned int j=0;j<Corners.size();j++){
                                    Corners[j].x=( ((unsigned int) Buffer[j*4] << 8) | (unsigned char)  Buffer[j*4+1] )/10;
                                    Corners[j].y=( ((unsigned int) Buffer[j*4+2] << 8) | (unsigned char)  Buffer[j*4+3] )/10;
                                }
        }
        else{
            return 0;
        }

        return 1;
}


bool KinseiDevice::GetZones(vector<zone>& Zones){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;


        command=0x78;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 ||  Buffer[0] != 0x78){
            return 0;
        }
        bytesReceived = sock->Receive(1, &Buffer[1]);
        if(bytesReceived == 1){

                    Zones.resize((unsigned int)Buffer[1]);
                    for(unsigned int i=0;i<Zones.size();i++){
                            bytesReceived = 0;              // Bytes read on each recv()
                            bytesReceived = sock->Receive(1, Buffer);
                            if(bytesReceived != 1){
                                return 0;
                            }
                            Zones[i].type=(unsigned int) Buffer[0];
                            if(Zones[i].type==0){
                                bytesReceived = 0;              // Bytes read on each recv()
                                bytesReceived = sock->Receive(6, Buffer);
                                if(bytesReceived != 6){
                                    return 0;
                                }
                                Zones[i].center.x=( ((unsigned int) Buffer[0] << 8) | (unsigned char)  Buffer[1] )/10;
                                Zones[i].center.y=( ((unsigned int) Buffer[2] << 8) | (unsigned char)  Buffer[3] )/10;
                                Zones[i].radius=( ((unsigned int) Buffer[4] << 8) | (unsigned char)  Buffer[5] )/10;
                            }
                            else if(Zones[i].type==1){
                                bytesReceived = 0;              // Bytes read on each recv()
                                bytesReceived = sock->Receive(1, Buffer);
                                if(bytesReceived != 1){
                                    return 0;
                                }
                                Zones[i].points.resize((unsigned int) Buffer[0]);

                                bytesReceived = 0;              // Bytes read on each recv()
                                bytesReceived = sock->Receive(Zones[i].points.size()*4, Buffer);
                                if(bytesReceived != (int)Zones[i].points.size()*4){
                                    return 0;
                                }

                                for(unsigned int j=0;j<Zones[i].points.size();j++){
                                    Zones[i].points[j].x=( ((unsigned int) Buffer[j*4] << 8) | (unsigned char)  Buffer[j*4+1] )/10;
                                    Zones[i].points[j].y=( ((unsigned int) Buffer[j*4+2] << 8) | (unsigned char)  Buffer[j*4+3] )/10;
                                }

                            }
                            else{

                                    return 0;
                                }

                    }
        }
        else {
            return 0;
        }

        return 1;
}

bool KinseiDevice::GetFusion(vector<fusion>& Fusion){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;

        //FUSION
        command=0x66;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 0x66){
            return 0;
        }
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2 ) return 0;
        if((unsigned int) Buffer[1] != 0) bytesReceived += sock->Receive(((unsigned int) Buffer[1])*7, &Buffer[2]);
        if(bytesReceived != (int)((unsigned int)Buffer[1])*7 + 2){
            return 0;
        }

        Fusion.resize((unsigned int)Buffer[1]);

        for(unsigned int k=0;k<(unsigned int)Buffer[1];k++){
                Fusion[k].centroid.x=(unsigned int)((float)( ((unsigned int) Buffer[k*7+4] << 8) | (unsigned char)  Buffer[k*7+5] )/10.0);
                Fusion[k].centroid.y=(unsigned int)((float)( ((unsigned int) Buffer[k*7+6] << 8) | (unsigned char)  Buffer[k*7+7] )/10.0);
                Fusion[k].val=(unsigned char)  Buffer[k*7+8];

        }

        return 1;
}




bool KinseiDevice::GetPositions(vector<point>& dpositions){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;


        command='d';
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 'd') return 0;
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2 ) return 0;
        if((unsigned int) Buffer[1] != 0) bytesReceived += sock->Receive(((unsigned int) Buffer[1])*6, &Buffer[2] );
        if(bytesReceived != (int)((unsigned int)Buffer[1])*6 + 2) return 0;

        dpositions.resize((unsigned int)Buffer[1]);

        for(unsigned int k=0;k<(unsigned int)Buffer[1];k++){

            if((unsigned int)( ((unsigned int) Buffer[k*6+2] << 8) | (unsigned char)  Buffer[k*6+3] ) == 65535 ){
                dpositions[k].x=-1;
                dpositions[k].y=-1;
            }else{
                dpositions[k].x=(unsigned int)((float)( ((unsigned int) Buffer[k*6+4] << 8) | (unsigned char)  Buffer[k*6+5] )/10.0);
                dpositions[k].y=(unsigned int)((float)( ((unsigned int) Buffer[k*6+6] << 8) | (unsigned char)  Buffer[k*6+7] )/10.0);
            }

        }


        return 1;
}

bool KinseiDevice::GetNPersons(float * np){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;
        Buffer[0]=0x65;

        command='n';
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 'n') return 0;
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2) return 0;
        *np= ((unsigned int)Buffer[1])/10.0;

        return 1;

}


bool KinseiDevice::GetNPersonsInt(unsigned int * np){

        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;

        command='m';
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1,Buffer);
        if(bytesReceived != 1 || (Buffer[0] != 'm' && Buffer[0] != 'n')) return 0;
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2) return 0;
        *np = (unsigned int)Buffer[1];

        return 1;

}


bool KinseiDevice::GetOnlineStatus(bool* status){

        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;

        command='t';
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 't') return 0;
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2) return 0;
        if((unsigned int)Buffer[1] == 1) *status=1;
        else  *status=false;

        return 1;
}

bool KinseiDevice::GetSensorsStatus(vector<sensor>& sen){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock==NULL) return 0;


        command='z';
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 'z') return 0;
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2) return 0;
        if((unsigned int) Buffer[1] != 0) bytesReceived += sock->Receive(((unsigned int) Buffer[1])*5, &Buffer[2]);
        if(bytesReceived != (int)(((unsigned int) Buffer[1])*5 + 2)) return 0;
        sen.resize((unsigned int) Buffer[1]);
        for(unsigned int k=0;k<(unsigned int)Buffer[1];k++){
            sen[k].online=(bool)Buffer[2+k*5];
            sen[k].position.x=(unsigned int)((float)( ((unsigned int) Buffer[k*5+3] << 8) | (unsigned char)  Buffer[k*5+4] )/10.0);
            sen[k].position.y=(unsigned int)((float)( ((unsigned int) Buffer[k*5+5] << 8) | (unsigned char)  Buffer[k*5+6] )/10.0);
        }
        return 1;
}


bool KinseiDevice::GetThermalMap(thermalmap_str& thermalmap){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;

        //Thermal map structure
        command=0x1A;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 0x1A) return 0;

        bytesReceived += sock->Receive(3, &Buffer[1]);
        if(bytesReceived != 3 + 1) return 0;



        //cout<<(unsigned int)Buffer[1]<<" "<<(unsigned int)Buffer[2]<<" "<<(unsigned int)Buffer[3]<<" "<<endl;
        if(thermalmap.X != (unsigned int)Buffer[1] || thermalmap.Y != (unsigned int)Buffer[2]){
            thermalmap.X=(unsigned int)Buffer[1];
            thermalmap.Y=(unsigned int)Buffer[2];
            thermalmap.size_cm=(unsigned int)Buffer[3];
            thermalmap.tmap.resize(thermalmap.Y);
            for(unsigned int i=0;i<thermalmap.Y;i++) thermalmap.tmap[i].resize(thermalmap.X);
        }


        //thermalmap values
        command=0x1B;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 0x1B){
            return 0;
        }
        bytesReceived += sock->Receive(2, &Buffer[1]);
        if(bytesReceived != 2+1) return 0;
        unsigned int Npixels=(((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] );
        if(Npixels != 0) bytesReceived = sock->Receive(Npixels*2, Buffer);
        if(bytesReceived != (int)Npixels*2){
            return 0;
        }

        unsigned int k=0;
        for(unsigned int i=0;i<thermalmap.Y;i++)
            for(unsigned int j=0;j<thermalmap.X;j++){
               thermalmap.tmap[i][j]= ((float)( ((unsigned int) Buffer[k++] << 8) | (unsigned char)  Buffer[k++] )/10.0);
            }

        return 1;
}


bool KinseiDevice::GetThermistors(vector<float>& thermistors){
        char command;
        int bytesReceived;              // Bytes read on each recv()
        unsigned char Buffer[10000];    // Buffer

        if(sock == NULL || sock->IsSocketValid() == false) return 0;

        Buffer[0]=0x65;

        //thermalmap values
        command=0x1C;
        sock->Send((const uint8*)&command, 1);
        bytesReceived = 0;              // Bytes read on each recv()
        bytesReceived = sock->Receive(1, Buffer);
        if(bytesReceived != 1 || Buffer[0] != 0x1C){
            return 0;
        }
        bytesReceived += sock->Receive(1, &Buffer[1]);
        if(bytesReceived != 2) return 0;
        thermistors.resize((unsigned int) Buffer[1]);
        if(thermistors.size() != 0) bytesReceived = sock->Receive(thermistors.size()*2, Buffer);
        if(bytesReceived != (int)thermistors.size()*2){
            return 0;
        }

        unsigned int k=0;
        for(unsigned int i=0;i<thermistors.size();i++)
               thermistors[i]= ((float)( ((unsigned int) Buffer[k++] << 8) | (unsigned char)  Buffer[k++] )/10.0);


        return 1;
}
