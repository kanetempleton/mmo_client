#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <thread>
#include <atomic>
#include "Client.h"

class Console {
public:
    Console(Client& client);
    ~Console();

    void startListening();
    void receiveMessage(const std::string& message);

private:
    std::thread inputThread;
    std::atomic<bool> stopThread;
    Client& client;

    void processCommand(const std::string& command);

    static void inputLoop(Console* console);
};

#endif // CONSOLE_H

