#ifndef BUTTONCHAT_H
#define BUTTONCHAT_H

#include "Button.hpp"
#include "chat.hpp"

class ButtonChat : public Button{
    private:
        std::string userName;
        Chat *chat;

    public:
        ButtonChat(SDL_Window * windows, SDL_Renderer * renderer, int x, int y, std::string fontPath, std::string imagesPath, std::string name, Chat *theChat);
        ~ButtonChat();
        void draw();
        Chat *getChat();
        std::string getUserName();
        
};

#endif /* BUTTONCHAT_H */