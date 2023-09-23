cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(IN204_Messenger VERSION 1.0)


add_executable(Server source/server.cpp source/common.cpp)

add_executable(ArtheusWeb source/ArtheusWeb.cpp source/MainMenu.cpp source/Button.cpp source/State.cpp source/common.cpp source/Message.cpp source/MsgContainer.cpp source/NewReceiver.cpp source/chat.cpp source/ButtonChat.cpp source/Login.cpp source/Socket.cpp)

target_link_libraries(Server sfml-system sfml-graphics sfml-window sfml-network SDL2 SDL2_ttf)

target_link_libraries(ArtheusWeb SDL2 SDL2_ttf SDL2_image sfml-system sfml-network)
