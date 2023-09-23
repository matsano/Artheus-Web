#include "Socket.hpp"

Socket::Socket(std::string ip, unsigned port, unsigned _myID, int type, std::string userName, unsigned link)
    : myID(_myID), myType(type), userName(userName){

    // socket creation
    sf::Socket::Status status = socket.connect(ip, port);
    if (status != sf::Socket::Done){
        std::cerr << "Falha na connexão" << std::endl;
        validSocket = false;
    } else {
        validSocket = true;
    }

    if(validSocket){
        // Server handshake
        MessageNet authMsg = {MSG_CONX_REQ, myID, SERVER_ID, std::to_string(myType)+userName};
        toSend << authMsg;
        #ifdef ENABLE_DEBUG_SOCKET
        std::cerr << "Sending: " << authMsg << std::endl;
        #endif
        socket.send(toSend);
        socket.receive(toRecv);
        toRecv >> recvMsg;
        #ifdef ENABLE_DEBUG_SOCKET
        std::cerr << "Recieved: " << recvMsg << std::endl;
        #endif
        if (recvMsg.sender != 0 || recvMsg.msgType != MSG_CONX_REP || recvMsg.content.compare("Aproved")){
            std::cerr << "Connection refused by server" << std::endl;
            validSocket = false;
        }   

        // gets ID from server
        if(myID == NOIDYET){
            myID = recvMsg.dest;
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "got my id! : " << myID << std::endl;
            #endif
        }

        // Link to another client?
        if(link && myType == EMITTER){
            MessageNet linkMsg = {MSG_LINK_REQ, myID, SERVER_ID, std::to_string(link)};
            toSend.clear();
            toSend << linkMsg;
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "Sending: " << linkMsg << std::endl;
            #endif
            socket.send(toSend);
            //TODO: verifi
            socket.receive(toRecv);
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "Recieved: " << recvMsg << std::endl;
            #endif
        }
    } else {
        std::cerr << "Invalid socket, no operation will be made" << std::endl;
    }

    if(myType == RECIEVER)
        socket.setBlocking(false);
}

Socket::~Socket(){

}

int Socket::send(unsigned destID, std::string msg){

    if(!validSocket){
        std::cerr << "Invalid socket, no operation will be made" << std::endl;
        return -1;
    }
    if(myType != EMITTER){
        std::cerr << "Operation forbidden in this type os socket" << std::endl;
        return -1;
    }

    sendMsg.msgType = MSG_STD;
    sendMsg.sender = myID;
    
    sendMsg.dest = destID;
    sendMsg.content = msg;
    
    toSend.clear();
    toSend << sendMsg;
    #ifdef ENABLE_DEBUG_SOCKET
    std::cerr << "Sending: " << sendMsg << std::endl;
    #endif
    socket.send(toSend);

    socket.receive(toRecv);
    toRecv >> recvMsg;
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_OK || recvMsg.dest != myID){
        std::cerr << "Error: " << recvMsg.content << std::endl;
    }
    return 0;
}

MessageNet Socket::recv(){
    sf::Socket::Status status;

    if(!validSocket){
        std::cerr << "Invalid socket, no operation will be made" << std::endl;
        return MessageNet();
    }

    if(myType != RECIEVER){
        std::cerr << "Operation forbidden in this type os socket" << std::endl;
        return MessageNet();
    }
    
    socket.setBlocking(false);
    while(true){
        status = socket.receive(toRecv);
        // treat partial receive or disconect
        if(status ==sf::Socket::Done){
            toRecv >> recvMsg;
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "received full:" << recvMsg << std::endl;
            #endif
            return recvMsg;
        } else if(status ==sf::Socket::Partial){
        //TODO: treat partial
            toRecv >> recvMsg;
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "received partial:" << recvMsg << std::endl;
            #endif
            return recvMsg;
        } else if(status ==sf::Socket::Disconnected){
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "Disconected" << std::endl;
            #endif
            return MessageNet();
        } else if(status ==sf::Socket::Error){
            #ifdef ENABLE_DEBUG_SOCKET
            std::cerr << "Error" << std::endl;
            #endif
            return MessageNet();
        } else {
        }
    sf::sleep(sf::milliseconds(500));
    }

    
    
}

std::string Socket::getUsernameFromServer(int idWho){
    socket.setBlocking(true);
    if(!validSocket){
        std::cerr << "Invalid socket, no operation will be made" << std::endl;
        return std::string();
    }

    sendMsg.msgType = MSG_WHOID_REQ;
    sendMsg.sender = myID;
    
    sendMsg.dest = SERVER_ID;
    sendMsg.content = std::to_string(idWho);
    
    toSend.clear();
    toSend << sendMsg;
    #ifdef ENABLE_DEBUG_SOCKET
    std::cerr << "Sending: " << sendMsg << std::endl;
    #endif
    socket.send(toSend);

    toRecv.clear();
    socket.receive(toRecv);
    toRecv >> recvMsg;
    #ifdef ENABLE_DEBUG_SOCKET
    std::cerr << "Recieved: " << recvMsg << std::endl;
    #endif
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_WHOID_REP || recvMsg.dest != myID){
        std::cerr << "Error: " << recvMsg.content << std::endl;
    }
    if(myType == RECIEVER)
        socket.setBlocking(false);
    return recvMsg.content;
}

int Socket::getID(){
    return myID;
}

int Socket::getIDFromServer(std::string who){
    socket.setBlocking(true);
    if(!validSocket){
        std::cerr << "Invalid socket, no operation will be made" << std::endl;
        return 0;
    }

    sendMsg.msgType = MSG_WHOUSER_REQ;
    sendMsg.sender = myID;
    
    sendMsg.dest = SERVER_ID;
    sendMsg.content = who;
    
    toSend.clear();
    toSend << sendMsg;
    std::cerr << "Sending: " << sendMsg << std::endl;
    socket.send(toSend);

    toRecv.clear();
    socket.receive(toRecv);
    toRecv >> recvMsg;
    #ifdef ENABLE_DEBUG_SOCKET
    std::cerr << "Recieved: " << recvMsg << std::endl;
    #endif
    if (recvMsg.sender != 0 || recvMsg.msgType != MSG_WHOUSER_REP || recvMsg.dest != myID){
        std::cerr << "Error: " << recvMsg.content << std::endl;
    }
    if(myType == RECIEVER)
        socket.setBlocking(false);
    if (recvMsg.content.size())
        return stoi(recvMsg.content);
    else
        return 0;
}