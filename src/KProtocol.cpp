#include "KProtocol.h"
#include <cstdio>

const char KProtocol::version[4] = {1,1,0,1};

KProtocol::KProtocol() {

}
KProtocol::~KProtocol() {

}

void KProtocol::processPacket(char bytes[]) {
    for (int i = 0; i < 1024; i++) {
        printf("%d\t%c\t//%d\n", i, bytes[i], static_cast<unsigned char>(bytes[i]));
    }
    char packetVersion[4] = {bytes[0],bytes[1],bytes[2],bytes[3]};
    char* packetKey = new char[16];
    for (int i=0;i<16;i++) {
        packetKey[i]=bytes[4+i];
    }
    //int packetid = // binary representation of bytes[20] || bytes[21]
    int packetid = (static_cast<unsigned char>(bytes[20]) << 8) | static_cast<unsigned char>(bytes[21]);
    int payloadLength = (static_cast<unsigned char>(bytes[22]) << 8) | static_cast<unsigned char>(bytes[23]);
    char* packetPayload = new char[payloadLength];
    for (int i = 0; i < payloadLength; i++) {
        packetPayload[i] = bytes[24 + i];
    }
    printf("packet processed: %d len=%d\npayload:%s\n",packetid,payloadLength,packetPayload);

    delete[] packetKey;
    delete[] packetPayload;
}

char* KProtocol::sendMessagePacket(const std::string& msg) {
    return constructPacket(1,msg.length(),strdup(msg.c_str()));
}

//constructPacket(dat[]): dat[]={payload}
char* KProtocol::constructPacket(int packetid, int payloadLength, char dat[]) {
    char* ret = new char[1024];
    int l = 0;
    for (int i=0; i<1024; i++) {
        ret[i]=0;
    }
    // construct packet version (4 bytes)
    ret[0]=version[0];
    ret[1]=version[1];
    ret[2]=version[2];
    ret[3]=version[3];
    l = 4;

    // client-key (16 bytes)
    for (int i=0; i<16; i++) {
        ret[l+i]=0; //0 for now
    }
    l += 16;

    // packet-id (2 bytes)
    ret[l] = (packetid >> 8) & 0xFF;
    ret[l + 1] = packetid & 0xFF;  
    l+=2;

    // payload-length (2 bytes)
    ret[l] = (payloadLength >> 8) & 0xFF; // High byte
    ret[l + 1] = payloadLength & 0xFF;    // Low byte
    l+=2;


    for (int i=0; i<1024 && i<payloadLength; i++) {
        ret[l+i] = dat[i];
    }
    return ret;
}
