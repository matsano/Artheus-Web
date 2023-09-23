# pragma once

#define ENABLE_DEBUG

namespace global{
    const int SMALL = 0;
    const int NORMAL = 1;
    const int BIG = 2;
    const int INIT_SCREEN_WIDTH = 640;
    const int INIT_SCREEN_HEIGHT = 480;
    const int FONT_SIZE = 32;
    const int FONT_SMALL_SIZE = 20;
    const int SCROLL_SPEED = 10;
    const int WINDOW_PADDING = 10;
    const int SIDE_SPACCING = 10;
    const int V_SPACING = 5;
    const int INIT_TEXT_BOX_HEIGHT = FONT_SIZE + 2 * V_SPACING;
    const int HEADER_HEIGHT = 1.7*FONT_SIZE + 2 * V_SPACING;
    const int DIE = 0;
    const int MAINMENU = 1;
    const int CHAT = 2;
    const int NEWRECEIVER = 3;
    const int MAKENEWCHAT = 4;
    const int LOGIN = 5;
    const int WIDTH_LIMIT = 492;
    const int MAX_CHARACTER = 15;
    const int LOGIN_WINDOW_WIDTH = 600;
    const int LOGIN_WINDOW_HEIGHT = 350;
    const int MAIN_WINDOW_WIDTH = 563;
    const int MAIN_WINDOW_HEIGHT = 844;
    const int NEW_RECEIVER_WINDOW_WIDTH = 600;
    const int NEW_RECEIVER_WINDOW_HEIGHT = 350;
    const std::string pathToFont = "source/fonts/arial.ttf";
    const std::string pathToImgs = "source/images/";
}