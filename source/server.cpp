#include <iostream>
#include <SFML/Network.hpp>
#include <list>
#include "common.hpp"

//#define ENABLE_DEBUG_SERVER

std::string getHostname() {
    std::string result = "";
    FILE* pipe = popen("hostname -I", "r");
    if (!pipe) return result;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);
    // Remove trailing newline character if it exists
    if (!result.empty() && result[result.length()-1] == '\n') {
        result.erase(result.length()-2);
    }
    return result;
}

bool existsClientId(std::list<ClientData> &clients, int id){
    for (auto client: clients){
        if(client.id == id)
            return 1;
    }
    return 0;
}

ClientData* findClientById(std::list<ClientData> &clients, int id){
    // returns preferably EMITTER
    for (auto & client: clients)
        if(client.id == id && client.type == EMITTER)
            return &client;
    for (auto & client: clients)
        if(client.id == id)
            return &client;
    return NULL;
}

ClientData* findClientByUser(std::list<ClientData> &clients, std::string user){
    // returns preferably EMITTER
    for (auto & client: clients)
        if(client.userName == user && client.type == EMITTER)
            return &client;
    for (auto & client: clients)
        if(client.userName == user)
            return &client;
    return NULL;
}

int getFreeId(std::list<ClientData> &clients){
    for(int i = 1; i < NOIDYET; i++)
        if (!existsClientId(clients,i))
            return i;
    return -1;
}

int main(){
    sf::TcpListener listener;

    if (listener.listen(SERVERPORT) != sf::Socket::Done)
    {
        std::cerr << "Failed in binding to port" << std::endl;
        return -1;
    }

    std::cout << "My IP is " << getHostname() << std::endl;

    std::list<ClientData> clients;
    sf::SocketSelector selector;

    MessageNet toRead, toSend;

    // to be notified in case of new connection
    selector.add(listener);

    while (1) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                // pending connection
                sf::TcpSocket* sock = new sf::TcpSocket;
                if (listener.accept(*sock) == sf::Socket::Done)
                {
                    ClientData aux = {NULL, 0, 0, 0, std::string()};
                    aux.socket = sock;
                    // Add the new client to the clients list
                    clients.push_back(aux);
                    // Add the new client to the selector so that we will
                    // be notified when he sends something
                    selector.add(*sock);
                }
                else
                {
                    // Error, we won't get a new connection, delete the socket
                    delete sock;
                }
            }
            else
            {
                // The listener socket is not ready, test all other sockets (the clients)
                for (auto client = clients.begin(); client != clients.end(); client++)
                {
                    //sf::TcpSocket* client = it;
                    if (selector.isReady(*((*client).socket)))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        
                        unsigned status = ((*client).socket)->receive(packet);
                        if (status == sf::Socket::Done)
                        {
                            if(packet >> toRead){  
                                #ifdef ENABLE_DEBUG_SERVER
                                std::cerr << "SUCESS: " << toRead << std::endl;
                                #endif
                                // Demanding connexion
                                if(toRead.msgType == MSG_CONX_REQ){

                                    if(existsClientId(clients,toRead.sender)){
                                        // client claiming to be someone else
                                        #ifdef ENABLE_DEBUG_SERVER
                                        std::cerr << "Disconnecting from repeated id client: " << ((*client).socket)->getRemoteAddress() << std::endl;
                                        #endif
                                        selector.remove(*((*client).socket));
                                        delete (*client).socket;
                                        client = clients.erase(client);
                                        client--;
                                    }else{
                                        // first connection of someone and is demanding an ID
                                        if(toRead.sender == NOIDYET){
                                            (*client).id = getFreeId(clients);
                                        }else{
                                            (*client).id = toRead.sender;
                                        }

                                        int aux = 0;
                                        (*client).type = std::stoi(std::to_string(toRead.content[0]));
                                        toRead.content.erase(0,1);
                                        (*client).userName = toRead.content;
                                    

                                        toSend.msgType = MSG_CONX_REP;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Aproved");
                                        packet.clear();
                                        packet << toSend;

                                        ((*client).socket)->send(packet);
                                        #ifdef ENABLE_DEBUG_SERVER
                                        std::cerr << "Admited " << (*client).id << " as " << (((*client).type)? "reciever" : "emitter")  << "| AKA:" << (*client).userName << std::endl;
                                        #endif
                                    }
                                }else if(toRead.msgType == MSG_STD){
                                    ClientData *clientData = findClientById(clients,toRead.dest);
                                    if(clientData == NULL){
                                        // destination not registered, return error to sender
                                        toSend.msgType = MSG_ERROR;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Destination not registered in database");
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                        #ifdef ENABLE_DEBUG_SERVER
                                        std::cerr << "Client " << toRead.dest << " not registered in database" << std::endl;
                                        #endif
                                    }else if(((*clientData).type == EMITTER) && (*clientData).link == 0){
                                        // destination not adequate to recv data, return error to sender
                                        toSend.msgType = MSG_ERROR;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Destination is a emitter and is not linked to any receiver");
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                        #ifdef ENABLE_DEBUG_SERVER
                                        std::cerr << "Client " << toRead.dest << " is not a reciever nor is linked to one" << std::endl;
                                        #endif
                                    }else{
                                        // redirecting through link or normal relay
                                        if ((*clientData).link != 0)
                                            clientData = findClientById(clients,(*clientData).link);
                                        (*clientData).socket->send(packet);
                                        #ifdef ENABLE_DEBUG_SERVER
                                        std::cerr << "Relaying msg to " << toRead.dest << std::endl;
                                        #endif
                                        // send ok to send
                                        toSend.msgType = MSG_OK;
                                        toSend.sender = SERVER_ID;
                                        toSend.dest = (*client).id;
                                        toSend.content = std::string("Done");
                                        packet.clear();
                                        packet << toSend;
                                        ((*client).socket)->send(packet);
                                    }
                                // try to link to another client
                                }else if(toRead.msgType == MSG_LINK_REQ){
                                    (*client).link = std::stoi(toRead.content);
                                    #ifdef ENABLE_DEBUG_SERVER
                                    std::cerr << "Linked " << (*client).id << " and " << (*client).link << std::endl;
                                    #endif
                                    toSend.msgType = MSG_OK;
                                    toSend.sender = SERVER_ID;
                                    toSend.dest = (*client).id;
                                    toSend.content = std::string();
                                    packet.clear();
                                    packet << toSend;
                                    ((*client).socket)->send(packet);
                                // name of who has this ID
                                }else if(toRead.msgType == MSG_WHOID_REQ){
                                    toSend.msgType = MSG_WHOID_REP;
                                    toSend.sender = SERVER_ID;
                                    toSend.dest = (*client).id;
                                    ClientData* who = findClientById(clients, stoi(toRead.content));
                                    if (who == NULL)
                                        toSend.content = std::string("NONE");
                                    else
                                        toSend.content = who->userName;
                                    
                                    packet.clear();
                                    packet << toSend;
                                    #ifdef ENABLE_DEBUG_SERVER
                                    std::cerr << "Awnsering request to name" << std::endl;
                                    std::cerr << toSend << std::endl;
                                    #endif
                                    ((*client).socket)->send(packet);
                                // ID of who has this name, in case of multiple, privileges EMITTER
                                }else if(toRead.msgType == MSG_WHOUSER_REQ){
                                    toSend.msgType = MSG_WHOUSER_REP;
                                    toSend.sender = SERVER_ID;
                                    toSend.dest = (*client).id;
                                    ClientData* who = findClientByUser(clients, toRead.content);
                                    if (who == NULL)
                                        toSend.content = std::string();
                                    else
                                        toSend.content = std::to_string(who->id);
                                    
                                    packet.clear();
                                    packet << toSend;
                                    #ifdef ENABLE_DEBUG_SERVER
                                    std::cerr << "Awnsering request to ID" << std::endl;
                                    std::cerr << toSend << std::endl;
                                    #endif
                                    ((*client).socket)->send(packet);
                                }
                            }
                            else{
                                std::cerr << "ERROR EXTRACTING" << std::endl;
                                break;
                            }
                        }
                        else if (status == sf::Socket::Disconnected)
                        {
                            std::cerr << "Disconnecting from " << ((*client).socket)->getRemoteAddress() << std::endl;
                            selector.remove(*((*client).socket));
                            delete (*client).socket;
                            client = clients.erase(client);
                            client--;   // since erase returns next element and loop also does this, needs to go back 1
                        }
                    }
                }
            } // if socket is ready
        }// if selector.wait
    }// while
}

    