#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

// ----------------------
// ---- GENERAL PART ----
// ----------------------

namespace debugging
{
#ifdef ENABLE_DEBUG
    constexpr bool debug = true;
#else
    constexpr bool debug = false;
#endif

    template <typename... Args>
    void print(const char* file, int line, Args... args) {
        (std::clog << "[" << file << ":" << line << "] "
                  << ... << args) << std::endl;
    }
}

#define DEBUG(...)                                              \
    do {                                                        \
        if (debugging::debug)                                   \
            debugging::print(__FILE__, __LINE__, __VA_ARGS__);  \
    } while (0)

// ----------------------
// ---- NETWORK PART ----
// ----------------------

#define SERVER_ID 0

#define MSG_STD      0
#define MSG_CONX_REQ 1
#define MSG_CONX_REP 2
#define MSG_LINK_REQ 3
#define MSG_LINK_REP 4
#define MSG_ERROR    5
#define MSG_OK       6
#define MSG_WHOID_REQ  7
#define MSG_WHOID_REP  8
#define MSG_WHOUSER_REQ  9
#define MSG_WHOUSER_REP  10

#define EMITTER 0
#define RECIEVER 1

#define NOIDYET 65565

#define SERVERPORT 53001

struct ClientData {
    sf::TcpSocket* socket;
    unsigned id;
    bool type;
    unsigned link;
    std::string userName;
};

struct MessageNet
{
    sf::Uint32 msgType;
    sf::Uint32 sender;
    sf::Uint32 dest;
    std::string content;
};

inline sf::Packet& operator <<(sf::Packet& packet, const MessageNet& message)
{
    return packet << message.msgType << message.sender << message.dest << message.content;
}

inline sf::Packet& operator >>(sf::Packet& packet, MessageNet& message)
{
    return packet >> message.msgType >> message.sender >> message.dest >> message.content;
}

inline std::ostream& operator << (std::ostream& aStream, const MessageNet& message)
{
    return aStream << "[" << message.msgType << "] FROM:" << message.sender << " TO:" << message.dest << " CONTENT: " << message.content;
}

struct AuthMessageNet {
    sf::Uint16 msgType;
    sf::Uint32 sender;
    sf::Uint32 dest;
    std::string content;
    sf::Uint32 authentication;
};

inline sf::Packet& operator <<(sf::Packet& packet, const AuthMessageNet& message)
{
    return packet << message.msgType << message.sender << message.dest << message.content << message.authentication;
}

inline sf::Packet& operator >>(sf::Packet& packet, AuthMessageNet& message)
{
    return packet >> message.msgType >> message.sender >> message.dest >> message.content >> message.authentication;
}

inline std::ostream& operator << (std::ostream& aStream, const AuthMessageNet& message)
{
    return aStream << "[" << message.msgType << "]FROM:" << message.sender << " TO:" << message.dest <<
         " CONTENT: " << message.content << " AUTH: " << message.authentication;
}

// -----------------------
// ---- GRAPHICS PART ----
// -----------------------

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void logSDLError(const std::string&);

int renderText(const std::string&, int, int, TTF_Font*, unsigned, SDL_Renderer*);

int getHeightText(const std::string&, unsigned, TTF_Font*, SDL_Renderer*);

int getWidthText(const std::string&, unsigned, TTF_Font*, SDL_Renderer*);

void getSizeText(const std::string&, unsigned, TTF_Font*, SDL_Renderer*, int&, int&);