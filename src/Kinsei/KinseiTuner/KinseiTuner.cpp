#include "KinseiTuner.h"
#include<iostream>

using namespace std;


KinseiTuner::KinseiTuner(){
    sock = new CActiveSocket();
    sock->Initialize();
    //sock->SetNonblocking();
    //sock->SetReceiveTimeout(100);

}

KinseiTuner::KinseiTuner(const char * ServAddress, uint16_t ServPort){
    sock = new CActiveSocket();
    sock->Initialize();
    //sock->SetNonblocking();
    //sock->SetReceiveTimeout(100);
    sock->Open(ServAddress,ServPort);

}

KinseiTuner::~KinseiTuner(){
    delete sock;
}

bool KinseiTuner::connect(const char *  ServAddress, uint16_t ServPort)
{
    return sock->Open(ServAddress,ServPort);

}


bool KinseiTuner::disconnect(){
    return sock->Close();
}


bool KinseiTuner::ResetBG(){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x00;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x00) return 0;
    else return 1;
}

bool KinseiTuner::ResetOffset(){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0xfd;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0xfd) return 0;
    else return 1;
}

bool KinseiTuner::GetBG_Alpha(float* alpha){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x01;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x01) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *alpha= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SetBG_Alpha(float* alpha){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x02;
    Buffer[1]=(unsigned char) ((unsigned int)((*alpha)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*alpha)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x02) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *alpha= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::GetBG_Threshold(float* TH){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x03;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x03) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SetBG_Threshold(float* TH){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x04;
    Buffer[1]=(unsigned char) ((unsigned int)((*TH)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*TH)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x04) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::GetBG_Temperature_Threshold(float* TH){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x05;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x05) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SetBG_Temperature_Threshold(float* TH){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x06;
    Buffer[1]=(unsigned char) ((unsigned int)((*TH)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*TH)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x06) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::GetFusion_BG_Threshold(float* TH){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x07;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x07) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}


bool KinseiTuner::SetFusion_BG_Threshold(float* TH){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x08;
    Buffer[1]=(unsigned char) ((unsigned int)((*TH)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*TH)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x08) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *TH= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::GetFusion_Consensum_Factor(float* CF){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x09;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x09) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *CF= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SetFusion_Consensum_Factor(float* CF){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x0A;
    Buffer[1]=(unsigned char) ((unsigned int)((*CF)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*CF)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);


    if(bytesReceived != 1 || Buffer[0] != 0x0A) return 0;


    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *CF= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::GetFusion_Threshold(float* FT){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0x0B;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x0B) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *FT= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SetFusion_Threshold(float* FT){
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;


    Buffer[0]=0x0C;
    Buffer[1]=(unsigned char) ((unsigned int)((*FT)*100) >> 8);
    Buffer[2]=(unsigned char) ((unsigned int)((*FT)*100) & 0xff);

    sock->Send((const uint8*)Buffer, 3);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0x0C) return 0;

    bytesReceived += sock->Receive(2, &Buffer[1]);
    if(bytesReceived != 2 + 1) return 0;

    *FT= (float)( ((unsigned int) Buffer[1] << 8) | (unsigned char)  Buffer[2] )/100.0;
    return 1;
}

bool KinseiTuner::SaveOverride(){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0xfe;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0xfe) return 0;
    else return 1;
}

bool KinseiTuner::DeleteOverride(){
    char command;
    int bytesReceived;              // Bytes read on each recv()
    unsigned char Buffer[10];    // Buffer

    if(sock == NULL || sock->IsSocketValid() == false) return 0;

    Buffer[0]=0x65;

    command=0xff;
    sock->Send((const uint8*)&command, 1);
    bytesReceived = 0;              // Bytes read on each recv()
    bytesReceived = sock->Receive(1, Buffer);
    if(bytesReceived != 1 || Buffer[0] != 0xff) return 0;
    else return 1;
}


