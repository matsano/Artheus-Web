#ifndef MAINMENU_H
#define MAINMENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "MainMenu.hpp"
#include "Button.hpp"
#include "chat.hpp"
#include "globals.hpp"
#include "State.hpp"
#include "ButtonChat.hpp"

class MainMenu : public State{
    private:
        bool buttonNewChat;
        int nbChats;
        std::vector<ButtonChat*> *chats;
        std::string pathImages;
        Chat * chatSelected;
    
    public:
        MainMenu(std::vector<ButtonChat*> *_chats,std::string fontPath, std::string imagesPath);
        virtual ~MainMenu();
        void addNewChat(SDL_Window * window, SDL_Renderer * ren);
        void updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture, int x, int y);
        int execute(Socket* outbound);
        Chat * getChatSelected();
};

#endif /* MAINMENU_H */