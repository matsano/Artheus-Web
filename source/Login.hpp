#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <SDL2/SDL_image.h>
#include "State.hpp"
#include "Button.hpp"
#include "globals.hpp"

class Login : public State {
    private:
        std::string pathImages;
        std::string inputText;
        bool confirmLogin;

    public:
        Login(std::string,std::string);
        ~Login();
        int execute();
};


#endif /* LOGIN_H */