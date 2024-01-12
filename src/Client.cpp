#include "Client.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Client::Client(GUI* g) : gui(g), socket_fd(-1) {
    FD_ZERO(&master_fds);
}

Client::~Client() {
    if (socket_fd != -1) {
        close(socket_fd);
    }
}

bool Client::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return false;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        return false;
    }

    return true;
}

bool Client::connectToServer(const std::string& serverIP, int serverPort) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        return false;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(socket_fd);
        return false;
    }

    if (connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        if (errno != EINPROGRESS) {
            perror("connect");
            close(socket_fd);
            return false;
        }
    }

    if (!setNonBlocking(socket_fd)) {
        close(socket_fd);
        return false;
    }

    FD_SET(socket_fd, &master_fds);

    return true;
}

bool Client::sendData(const std::string& data) {
    ssize_t totalSent = 0;

    while (totalSent < data.size()) {
        ssize_t sent = send(socket_fd, data.c_str() + totalSent, data.size() - totalSent, 0);

        if (sent == -1) {
            perror("send");
            return false;
        }

        totalSent += sent;
    }

    printf("Sent: %s\n", data.c_str());

    return true;
}


void Client::setOnDataReceivedCallback(const std::function<void(const std::string&)>& callback) {
    onDataReceivedCallback = callback;
}

std::string Client::receiveData() {
    std::string receivedData;

    while (true) {
        fd_set read_fds = master_fds;

        // Use a non-blocking call to check for data
        int result = select(socket_fd + 1, &read_fds, nullptr, nullptr, nullptr);

        if (result == -1) {
            perror("select");
            return receivedData;  // Return what has been received so far
        }

        if (FD_ISSET(socket_fd, &read_fds)) {
            char buffer[1024];
            ssize_t bytesRead = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead == -1) {
                perror("recv");
                return receivedData;  // Return what has been received so far
            }

            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                std::cout << "Received data: " << buffer << std::endl;
                gui->serverResponse = buffer;

                // Append the received data to the string
                receivedData += buffer;

                // Call the callback with the received data
                if (onDataReceivedCallback) {
                    onDataReceivedCallback(receivedData);
                }
            } else {
                std::cout << "Connection closed by the server." << std::endl;
                close(socket_fd);
                break;  // Exit the loop when the connection is closed
            }
        }
    }

    return receivedData;
}


