//
// Created by veyrie_f on 4/21/16.
//

#ifndef CPP_PLAZZA_INFOBOX_HPP
#define CPP_PLAZZA_INFOBOX_HPP


#include "UIComponent.hpp"

/*
 * \brief Infobox component for UI. Shows pop up on hover
 */
class InfoBox : public UIComponent
{
public:
    InfoBox(WindowManagerSDL &win, Vector2<double> pos, Vector2<double> size, std::string const& sprite, UIComponent *toDisp);
    virtual ~InfoBox();

public:
    void Draw();
    void OnMouseButtonUp(SDL_MouseMotionEvent mouse);
    void OnMouseButtonDown(SDL_MouseMotionEvent mouse);
    void OnHover(SDL_MouseMotionEvent mouse);

private:
    SDL_Surface *m_sprite;
    UIComponent *m_component;
};


#endif //CPP_PLAZZA_INFOBOX_HPP
