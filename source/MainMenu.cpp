#include "MainMenu.hpp"

MainMenu::MainMenu(std::vector<ButtonChat*> *_chats, std::string fontPath, std::string imagesPath) : chats(_chats), State(fontPath) {
    pathImages = imagesPath;
    nbChats = chats->size();
}

MainMenu::~MainMenu() {}

int MainMenu::execute(Socket* outbound){

    unsigned lastRender = SDL_GetTicks();
    nbChats = chats->size();
    int nextState = global::MAINMENU;
    bool flagRender = true;

    SDL_Window * window = SDL_CreateWindow("Artheus Web",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            global::MAIN_WINDOW_WIDTH, //window_width
                            global::MAIN_WINDOW_HEIGHT, //window height
                            SDL_WINDOW_SHOWN);

    SDL_Renderer * ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    bool running = true;

    // HEADER
    SDL_Texture * textureHeader = IMG_LoadTexture(ren, std::string(pathImages+"header.png").c_str());
    SDL_Rect cabecalhoRect = {0, 0, 373, 70};
    Button * buttonHeader = new Button(window, ren, 373, 0, global::pathToFont, std::string(pathImages+"newChat.png"));
    SDL_Rect buttonHeaderRect = buttonHeader->getButtonRect();

    // CHATS (BUTTONS)
    if (receiverName != ""){
        int rep = outbound->getIDFromServer(receiverName);
        // client exists
        if(rep != 0){
            addNewChat(window, ren);
            nbChats = chats->size();
            updateChats(window, ren, chats->back()->getTexture(), 0, 70+nbChats*92);
        }else
            std::cerr << "Client does not Exist" << std::endl;
        receiverName = "";
    } else{
        SDL_Surface * newButtonSurface = IMG_Load(std::string(pathImages+"profileChat.png").c_str());
        SDL_Texture * newButtonTexture = SDL_CreateTextureFromSurface(ren, newButtonSurface);
        updateChats(window, ren, newButtonTexture, 0, 70+nbChats*92);
    }

    // BLANK SPACE (WITHOUT CHATS)
    SDL_Rect conversasRect = {0, 0, 563, 844};

    while (running && nextState == global::MAINMENU){
        nbChats = chats->size();
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    nextState = global::DIE;
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // Check if the button was pressed
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    SDL_Point mousePoint = {mouseX, mouseY};
                    for (auto i : *chats){
                        SDL_Rect chatRect = i->getButtonRect();
                        if (SDL_PointInRect(&mousePoint, &chatRect)) {
                            chatSelected = i->getChat();
                            nextState = global::CHAT;
                            running = false;
                        }
                    }
                    if (SDL_PointInRect(&mousePoint, &buttonHeaderRect)) {
                        nextState = global::NEWRECEIVER;
                        running = false;
                    }
                    break;
            }

        }

        // render if need be or depased 100ms threshold
        if (flagRender || lastRender+100 < SDL_GetTicks()){
            // update chat references
            nbChats = chats->size();
            if (nbChats){
                SDL_Surface * newButtonSurface = IMG_Load(std::string(pathImages+"profileChat.png").c_str());
                SDL_Texture * newButtonTexture = SDL_CreateTextureFromSurface(ren, newButtonSurface);
                updateChats(window, ren, newButtonTexture, 0, 70);
            }
            // BLANK SPACE (WITHOUT CHATS)
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            SDL_RenderFillRect(ren, &conversasRect);

            // HEADER
            SDL_RenderCopy(ren, textureHeader, NULL, &cabecalhoRect);
            buttonHeader->draw();
            renderText(userName, 90, 20, fontNormal, global::WIDTH_LIMIT, ren);

            // BUTTONS
            for (auto i : *chats){
                i->draw();
            }

            SDL_RenderPresent(ren);
            flagRender = false;
            lastRender = SDL_GetTicks();
        }
    }
    // Release the resources used by the SDL2 library
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return nextState;

}

void MainMenu::addNewChat(SDL_Window * window, SDL_Renderer * ren){
    // look for existing chat
    bool flagFound = false;
    for(auto i: *chats)
        if(i->getUserName()==receiverName){
            flagFound = true;
            break;
        }
    
    // did not find it, adding
    if(!flagFound){
        Chat * newChat = new Chat(std::string(global::pathToFont), receiverName);
        ButtonChat *newBottonChat = new ButtonChat(window, ren, 0, 70+nbChats*92, global::pathToFont, std::string(pathImages+"profileChat.png"), receiverName, newChat);
        chats->push_back(newBottonChat);
    }
}

void MainMenu::updateChats(SDL_Window * window, SDL_Renderer * ren, SDL_Texture* buttonTexture, int x, int yOffset){
    int n = 0;
    for (auto i : *chats){
        i->setWindowRenderer(window, ren, buttonTexture);
        i->setPosition(x,yOffset+n*92);
        n++;
    }
}

Chat* MainMenu::getChatSelected(){
    return chatSelected;
}
