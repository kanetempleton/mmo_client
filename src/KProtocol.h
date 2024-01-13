#pragma once

#include <string>

class KProtocol {
    public:
        KProtocol();
        ~KProtocol();
        static const char version[4];
        void processPacket(char bytes[]);
        char* sendMessagePacket(const std::string& msg);
    private:
        char* constructPacket(int packetid, int payloadLength, char dat[]);
};