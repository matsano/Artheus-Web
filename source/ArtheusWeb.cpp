#include <iostream>
#include <thread>
#include <map>
#include "MainMenu.hpp"
#include "chat.hpp"
#include "NewReceiver.hpp"
#include "ButtonChat.hpp"
#include "Login.hpp"
#include "globals.hpp"
#include "Socket.hpp"

//#define DEBUG_MAIN

using namespace std;

std::vector<ButtonChat*> chats;
bool globalRun;

Socket* outbound;

std::map<int,std::string> idToUsername;
std::map<std::string,int> usernameToId;

void recvThread(std::vector<ButtonChat*> *chats, std::string ip, std::string username){
    Socket inbound(ip, SERVERPORT, NOIDYET, RECIEVER, username);
    // link sockets
    outbound = new Socket(ip, SERVERPORT, NOIDYET, EMITTER, username, inbound.getID());
    std::cout << "socket created in thread" << std::endl;
    MessageNet recvMsg;
    while(globalRun){
        recvMsg = inbound.recv();

        // failed receiving
        if (recvMsg.dest == 0)
            globalRun = false;

        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        std::ostringstream hour;
        hour << std::setw(2) << std::setfill('0') << timeinfo->tm_hour;
        std::ostringstream min;
        min << std::setw(2) << std::setfill('0') << timeinfo->tm_min;

        bool foundChat = false;
        // look for chat with specified name
        for (auto i : *chats){
            // chat already exists and add msg
            if(i->getUserName() == idToUsername[recvMsg.sender]) {
                i->getChat()->addMessage(Message(recvMsg.content, i->getUserName(),hour.str(),min.str()));
                foundChat = true;
                #ifdef DEBUG_MAIN
                std::cout << "Adding to existing chat:" << i->getUserName() << "\" number " << recvMsg.sender << std::endl;
                #endif
                break;
            }
        }
        // chat doenst exist yet and needs to create it
        if(foundChat == false){
            // needs name of chatter, is gonna demand the server
            std::string name = inbound.getUsernameFromServer(recvMsg.sender);
            // if name is valid
            if(name != std::string()){
                // it could de that it has the chat but its just not mapped correctly so needs to search for name of chat
                // and if that's the case, add reference to so
                bool foundChat = false;
                for (auto i : *chats){
                    // chat already exists and add msg
                    if(i->getUserName() == name) {
                        i->getChat()->addMessage(Message(recvMsg.content, i->getUserName(),hour.str(),min.str()));
                        foundChat = true;
                        #ifdef DEBUG_MAIN
                        std::cout << "Adding to existing chat:" << i->getUserName() << "\" number " << recvMsg.sender << std::endl;
                        #endif
                        idToUsername[recvMsg.sender] = name;
                        break;
                    }
                }
                if(foundChat == false){
                    // now that has it, create client
                    Chat * newChat = new Chat(std::string(global::pathToFont), name);
                    ButtonChat *newButtonChat = new ButtonChat(NULL, NULL, 0, 0, global::pathToFont, std::string(global::pathToImgs+"profileChat.png"), name, newChat);
                    chats->push_back(newButtonChat);
                    chats->back()->getChat()->addMessage(Message(recvMsg.content, name,hour.str(),min.str()));
                    #ifdef DEBUG_MAIN
                    std::cout << "Adding to new chat:" << chats->back()->getUserName() << std::endl;
                    #endif
                    idToUsername[recvMsg.sender] = name;
                    usernameToId[name] = recvMsg.sender;
                }
            }else{
                std::cerr << "Got bad reponse from server" << std::endl;
            }
        }
    }
    delete outbound;
}

int main(int argc, char** argv){
    globalRun = true;
    std::thread *threadRecv;

    MainMenu mainMenu = MainMenu(&chats, global::pathToFont, global::pathToImgs);
    NewReceiver newReceiver = NewReceiver(global::pathToFont, global::pathToImgs);
    Login login = Login(global::pathToFont, global::pathToImgs);
    Chat *chat = NULL;
    
    int currentState = global::LOGIN;
    
    while (globalRun) {
        switch (currentState)
        {
        case global::MAINMENU:
            currentState = mainMenu.execute(outbound);
            break;
        case global::CHAT:
            chat = mainMenu.getChatSelected();
            currentState = chat->execute(outbound, &usernameToId);
            break;
        case global::NEWRECEIVER:
            currentState = newReceiver.execute();
            mainMenu.setReceiverName(newReceiver.getReceiverName());
            break;
        case global::LOGIN:
            currentState = login.execute();
            // initialization
            if(currentState == global::MAINMENU)
                threadRecv = new std::thread(recvThread, &chats, std::string(argv[1]),login.getUserName());
            mainMenu.setUserName(login.getUserName());
            break;
        default:
            globalRun = false;
            break;
        }
    }
    
    if(!threadRecv){
        threadRecv->join();
        delete threadRecv;
    }
    return 0;
}