//
// Created by veyrie_f on 4/20/16.
//

#ifndef CPP_PLAZZA_BUTTON_HPP
#define CPP_PLAZZA_BUTTON_HPP


#include "UIComponent.hpp"

class Button : public UIComponent
{
public:
    Button(WindowManagerSDL &win, std::string const& sprite, Vector2<double> pos, Vector2<double> size);
    virtual ~Button();

public:
    void Draw();
    void OnMouseButtonUp(SDL_MouseMotionEvent mouse);
    void OnMouseButtonDown(SDL_MouseMotionEvent mouse);
    void OnHover(SDL_MouseMotionEvent mouse);
    //void AddCallback();

private:
    std::string m_spritePath;
    SDL_Surface *m_sprite;
};


#endif //CPP_PLAZZA_BUTTON_HPP
