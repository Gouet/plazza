//
// Created by veyrie_f on 4/20/16.
//

#ifndef CPP_PLAZZA_PANEL_HPP
#define CPP_PLAZZA_PANEL_HPP

#include "UIComponent.hpp"

/*
 * \brief A panel is an element that is composed by an image and a list of ui components
 * All the positions of the children are relative to their parent, thus the panel
 */
class Panel : public UIComponent
{
public:
    Panel(WindowManagerSDL &win, std::string const& sprite, Vector2<double> pos, Vector2<double> size);
    virtual ~Panel();

public:
    void Draw();
    void OnMouseButtonUp(SDL_MouseMotionEvent mouse);
    void OnMouseButtonDown(SDL_MouseMotionEvent mouse);
    void OnHover(SDL_MouseMotionEvent mouse);
    void AddComponent(UIComponent * component, std::string const& componentName);
    void RemoveComponent(std::string const& componentName);

public:
    UIComponent *operator[](std::string const& component) const;

private:
    mutable std::map<std::string, UIComponent *> m_components;
    SDL_Surface *m_sprite;
    Vector2<double> m_originalSize;
};


#endif //CPP_PLAZZA_PANEL_HPP
