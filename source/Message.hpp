#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>

class Message {
    public:
    std::string text;
    std::string sender;
    std::string hour;
    std::string minute;

    Message();
    Message(std::string text, std::string sender, std::string hour, std::string minute);
    Message(std::string text, std::string sender);

};

#endif /* MESSAGE_H */