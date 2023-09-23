#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>  
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Message.hpp"
#include "MsgContainer.hpp"
#include "common.hpp"
#include "State.hpp"
#include "Button.hpp"
#include "Socket.hpp"

// WARNING, depends heavely on constants defined in globals.hpp
#include "globals.hpp"

class Chat : public State {
    private:
        MsgContainer* msgContainer;
        std::string whoImTalkingTo;

    public:
        Chat(std::string fontPath, std::string receiver);
        ~Chat();
        void addMessage(Message);
        int execute(Socket*, std::map<std::string,int>*);
    private:
        void renderMessages(int);
        void scrollMessages(int&, int, int amount=0);
};