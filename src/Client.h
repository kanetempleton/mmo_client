#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include "GUI.h"
#include "KProtocol.h"


class Client {
public:
    Client(GUI* g,KProtocol* k);
    ~Client();
    GUI* gui;
    KProtocol* kProtocol;
    bool connectToServer(const std::string& serverIP, int serverPort);
    bool sendData(const std::string& data);
    bool sendBytes(char* data, int dataSize);
     std::string receiveData();
      void setOnDataReceivedCallback(const std::function<void(const std::string&)>& callback);


private:
    int socket_fd;
    fd_set master_fds;
    
    bool setNonBlocking(int fd);
    std::function<void(const std::string&)> onDataReceivedCallback;
};

#endif // CLIENT_H
