#include "common.hpp"

// ----------------------
// ---- NETWORK PART ----
// ----------------------

// NONE

// -----------------------
// ---- GRAPHICS PART ----
// -----------------------

void logSDLError(const std::string& msg) {
    std::cerr << msg << " error: " << SDL_GetError() << std::endl;
}

int renderText(const std::string& text, int x, int y, TTF_Font* font, unsigned widthMax, SDL_Renderer* renderer) {
    SDL_Surface* surface = NULL;
    surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), SDL_Color({ 0, 0, 0 }), widthMax);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width = 0;
    int height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SDL_Rect destRect = { x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    return height;
}

int getHeightText(const std::string& text, unsigned widthMax, TTF_Font* font, SDL_Renderer* renderer) {
    int width = 0;
    int height = 0;
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), SDL_Color({ 0, 0, 0 }), widthMax);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return height;
}

int getWidthText(const std::string& text, unsigned widthMax, TTF_Font* font, SDL_Renderer* renderer) {
    int width = 0;
    int height = 0;
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), SDL_Color({ 0, 0, 0 }), widthMax);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    return width;
}

void getSizeText(const std::string& text, unsigned widthMax, TTF_Font* font, SDL_Renderer* renderer, int& height, int& width) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), SDL_Color({ 0, 0, 0 }), widthMax);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}