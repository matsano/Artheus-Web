# pragma once

#include <SFML/Network.hpp>
#include <string>
#include <iostream>

#include "common.hpp"

//define ENABLE_DEBUG_SOCKET

class Socket {
    private:
        sf::Packet toSend, toRecv;
        MessageNet sendMsg, recvMsg;
        sf::TcpSocket socket;
        bool validSocket;
        unsigned myID;
        int myType;
        std::string userName;
    public:
        Socket(std::string ip, unsigned port, unsigned myID, int type, std::string userName, unsigned link = 0);
        ~Socket();
        int send(unsigned destID, std::string msg);
        MessageNet recv();
        std::string getUsernameFromServer(int);
        int getIDFromServer(std::string);
        int getID();
};