#include "MsgContainer.hpp"

MsgContainer::MsgContainer(std::function<int(const std::string&, unsigned, TTF_Font*, SDL_Renderer*)> _getHeightText, 
        TTF_Font* _fontNormal, TTF_Font* _fontSmall, SDL_Renderer* _renderer, int _widthMax) :
        totalHeight(0), getHeightText(_getHeightText),
        fontNormal(_fontNormal), fontSmall(_fontSmall), renderer(_renderer), widthMax(_widthMax) {}

void MsgContainer::updateWidthMax(int newWidthMax){
    widthMax = newWidthMax;
}

void MsgContainer::updateHeight(){
    totalHeight = 0;
    for(auto i : _container){
        totalHeight += getHeightText(i.text, widthMax, fontNormal, renderer) + global::V_SPACING;
        totalHeight += getHeightText(i.hour+":"+i.minute, widthMax, fontSmall, renderer);
    }
}

int MsgContainer::getTotalHeight(){
    updateHeight();
    return totalHeight;
}

int MsgContainer::append(Message msg){
    _container.push_back(msg);
    int aux = getHeightText(msg.text, widthMax, fontNormal, renderer);
    aux += getHeightText(msg.hour+":"+msg.minute, widthMax, fontSmall, renderer);
    aux += global::V_SPACING;
    totalHeight += aux;
    return aux;
}

std::vector<Message>::const_iterator MsgContainer::getReadBeginIterator(){
    return _container.cbegin();
}

std::vector<Message>::const_iterator MsgContainer::getReadEndIterator(){
    return _container.cend();
}

void MsgContainer::updateFontRenderer(TTF_Font* newFontNormal, TTF_Font* newFontSmall, SDL_Renderer* newRenderer){
    fontNormal = newFontNormal;
    fontSmall = newFontSmall;
    renderer = newRenderer;
}
