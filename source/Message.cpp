#include "Message.hpp"

Message::Message():text(std::string("")),sender(std::string("")),hour(std::string("")),minute(std::string("")){}

Message::Message(std::string text, std::string sender, std::string hour, std::string minute):text(text),sender(sender),hour(hour),minute(minute){}

Message::Message(std::string text, std::string sender):text(text),sender(sender){}
