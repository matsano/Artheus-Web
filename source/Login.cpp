#include "Login.hpp"

Login::Login(std::string fontPath, std::string imagesPath) : State(fontPath){
    pathImages = imagesPath;
}

Login::~Login(){

}

int Login::execute(){
    SDL_Window * windowAdd = SDL_CreateWindow("Login",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            global::LOGIN_WINDOW_WIDTH, //window_width
                            global::LOGIN_WINDOW_HEIGHT, //window height
                            SDL_WINDOW_SHOWN);

    SDL_Renderer * rendererAdd = SDL_CreateRenderer(windowAdd, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    inputText = "";
    bool flagRenderText = true;

    // BACKGROUND
    SDL_Rect backgroundRect = {0, 0, global::LOGIN_WINDOW_WIDTH, global::LOGIN_WINDOW_HEIGHT};

    // LOGIN NAME
    std::string loginWho = "Login:";

    // NAME TEXT
    SDL_Rect edgeRect = {55, 115, 500, 64};
    SDL_Rect nameTextRect = {59, 119, 492, 56};

    // ENTER BUTTON
    Button * enterButton = new Button(windowAdd, rendererAdd, 200, 250, global::pathToFont, std::string(pathImages+"enterButton.png").c_str());
    SDL_Rect enterButtonRect = enterButton->getButtonRect();

    while (running){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                confirmLogin = false;
                running = false;
                break;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Check if the button was pressed
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = {mouseX, mouseY};
                // Enter Button
                if (SDL_PointInRect(&mousePoint, &enterButtonRect)) {
                    if (inputText.size() != 0){
                        confirmLogin = true;
                        running = false;
                    }
                }
                break;
            } else if (event.type == SDL_KEYDOWN) {
                //Handle backspace
                if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ) {
                    //lop off character
                    inputText.pop_back();
                    flagRenderText = true;
                }
                //Handle enter
                else if( event.key.keysym.sym == SDLK_RETURN) {
                    if(inputText.size() != 0) {
                        confirmLogin = true;
                        running = false;
                        flagRenderText = true;
                    }
                }
                break;
            } else if (event.type == SDL_TEXTINPUT) {
                //Not copy or pasting
                if ( !( SDL_GetModState() & KMOD_CTRL && ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' || event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) ) ) {
                    if (inputText.size() < global::MAX_CHARACTER){
                        //Append character
                        inputText += event.text.text;
                        flagRenderText = true;
                    }
                }
                break;
            }
        }

        // NUM MAX CHARACTER
        std::string numCharacterText = "Number of characters: ";
        numCharacterText += std::to_string(inputText.size());
        numCharacterText += "/15";
        userName = inputText;
        
        if(flagRenderText){
            SDL_RenderClear(rendererAdd);

            // BACKGROUND
            SDL_SetRenderDrawColor(rendererAdd, 200, 200, 200, 255);
            SDL_RenderFillRect(rendererAdd, &backgroundRect);

            // TO WHO
            renderText(loginWho, 50, 70, fontNormal, global::WIDTH_LIMIT, rendererAdd);

            // CONFIRM BUTTON
            enterButton->draw();

            // NAME TEXT
            SDL_SetRenderDrawColor(rendererAdd, 0, 0, 0, 255);
            SDL_RenderFillRect(rendererAdd, &edgeRect);
            SDL_SetRenderDrawColor(rendererAdd, 128, 128, 128, 255);
            SDL_RenderFillRect(rendererAdd, &nameTextRect);

            // NUM MAX CHARACTER
            renderText(numCharacterText, 355, 180, fontSmall, global::WIDTH_LIMIT, rendererAdd);

            // INPUT TEXT
            renderText(inputText, 65, 140, fontNormal, global::WIDTH_LIMIT, rendererAdd);
            flagRenderText = false;

            SDL_RenderPresent(rendererAdd);
        }    
    }

    if (!confirmLogin){
        userName = "";
    }

    // Release the resources used by the SDL2 library
    SDL_DestroyRenderer(rendererAdd);
    SDL_DestroyWindow(windowAdd);
    SDL_Quit();

    if (!confirmLogin){
        userName = "";
        return global::DIE;
    } else{
        return global::MAINMENU;
    }
    
}
