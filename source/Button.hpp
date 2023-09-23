#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "common.hpp"
#include "globals.hpp"

class Button{
    protected:
        TTF_Font* gFontNormal;
        SDL_Window * win;
        SDL_Renderer * ren;
        SDL_Surface* buttonSurface;
        SDL_Texture* buttonTexture;
        SDL_Rect buttonRect;
        std::string pathImages;
        int buttonWidth;
        int buttonHeight;
    
    public:
        
        Button(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, std::string imagesPath);
        virtual ~Button();
        void draw();
        void setWindowRenderer(SDL_Window * newWindow, SDL_Renderer * newRen, SDL_Texture* newTexture);
        void setPosition(int x, int y);
        SDL_Rect getButtonRect();
        SDL_Texture* getTexture();
};

#endif /* BUTTON_H */