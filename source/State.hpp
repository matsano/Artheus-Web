#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "globals.hpp"
#include "common.hpp"

using namespace std;

class State{
    public:
        State(std::string fontPath);
        virtual ~State();
        int execute();
        std::string getReceiverName();
        void setReceiverName(std::string newName);
        std::string getUserName();
        void setUserName(std::string newName);

    protected:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        int TEXT_BOX_HEIGHT;
        int MAX_MSG_WIDTH;
        SDL_Window* Window;
        SDL_Renderer* renderer;
        TTF_Font* fontNormal;
        TTF_Font* fontSmall;
        TTF_Font* fontBig;
        SDL_Color textColor;
        std::string receiverName;
        std::string userName;
};


#endif /* STATE_H */