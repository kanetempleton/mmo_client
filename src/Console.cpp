#include "Console.h"

Console::Console(Client& client) : stopThread(false), client(client) {
    inputThread = std::thread(&Console::inputLoop, this);
}

Console::~Console() {
    stopThread = true;
    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void Console::startListening() {
    while (!stopThread) {
        // Your main thread logic can go here
        // For example, you can perform other tasks while waiting for user input
    }
}

void Console::processCommand(const std::string& command) {
    if (command.compare(0, 7, "message") == 0) {
        // Extract the message from the command
        std::string message = command.substr(8);

        // Send the message to the server
        client.sendData(message);

    } else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void Console::inputLoop(Console* console) {
    while (!console->stopThread) {
        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        if (!command.empty()) {
            console->processCommand(command);
        }
    }
}

void Console::receiveMessage(const std::string& message) {
    // Print the received message
    std::cout << "Received message: " << message << std::endl;
}
