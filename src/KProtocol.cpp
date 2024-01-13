#include "KProtocol.h"
#include <cstdio>

const char KProtocol::version[4] = {1,1,1,1};

KProtocol::KProtocol() {

}
KProtocol::~KProtocol() {

}

void KProtocol::processPacket(char bytes[]) {
    /*for (int i = 0; i < 1024; i++) {
        printf("%d\t%c\t//%d\n", i, bytes[i], static_cast<unsigned char>(bytes[i]));
    }*/
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
    // Convert the message to a char array
    const char* msgData = msg.c_str();
    int msgLength = msg.length();

    // Create a new char array and copy the message data
    char* msgCopy = new char[msgLength + 1];  // +1 for the null terminator
    std::copy(msgData, msgData + msgLength + 1, msgCopy);

   // Construct the packet with the copied message data
    char* packet = constructPacket(1, msgLength, msgCopy);
    printf("sendMessagePacket: %d\n", msgLength);


    // Free the memory allocated for msgCopy
    delete[] msgCopy;

    return packet;
}


//constructPacket(dat[]): dat[]={payload}
char* KProtocol::constructPacket(int packetid, int payloadLength, char dat[]) {
    char* ret = new char[1024];
    int l = 0;

    // Construct packet version (4 bytes)
    ret[0] = version[0];
    ret[1] = version[1];
    ret[2] = version[2];
    ret[3] = version[3];
    l = 4;

    // Client-key (16 bytes)
    for (int i = 0; i < 16; i++) {
        ret[l + i] = 0; // 0 for now
    }
    l += 16;

    // Packet-id (2 bytes)
    ret[l] = (packetid >> 8) & 0xFF;
    ret[l + 1] = packetid & 0xFF;
    l += 2;

    // Payload-length (2 bytes)
    ret[l] = (payloadLength >> 8) & 0xFF; // High byte
    ret[l + 1] = payloadLength & 0xFF;    // Low byte
    l += 2;

    // Copy payload data, ensuring no buffer overrun
    for (int i = 0; i < payloadLength && l < 1024; i++, l++) {
        ret[l] = dat[i];
    }

    // Fill the remaining space with zeros
    for (; l < 1024; l++) {
        ret[l] = 0;
    }
    printf("constructPacket: l=%d\n",l);

    return ret;
}
