#ifndef MSGCONTAINER_H
#define MSGCONTAINER_H

#include <iostream>
#include <vector>
#include <functional>
#include <iterator>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Message.hpp"
#include "globals.hpp"

// needs some information regarding the current screen to calculate height
class MsgContainer {
    private:
        int totalHeight;
        TTF_Font* fontNormal;
        TTF_Font* fontSmall;
        SDL_Renderer* renderer;
        int widthMax;
        std::function <int(const std::string&, unsigned, TTF_Font*, SDL_Renderer*)> getHeightText;
        void updateHeight();
        std::vector<Message> _container;
    public:
        std::vector<Message>::const_iterator getReadBeginIterator();
        std::vector<Message>::const_iterator getReadEndIterator();
        int getTotalHeight();
        MsgContainer(std::function<int(const std::string&, unsigned, TTF_Font*, SDL_Renderer*)>, TTF_Font*, TTF_Font*, SDL_Renderer*, int);
        int append(Message);
        void updateWidthMax(int);
        void updateFontRenderer(TTF_Font* newFontNormal, TTF_Font* newFontSmall, SDL_Renderer* newRenderer);
};

#endif /* MSGCONTAINER_H */