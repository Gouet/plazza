//
// Created by veyrie_f on 4/20/16.
//

#include "Button.hpp"

Button::Button(WindowManagerSDL &win, std::string const &sprite, Vector2<double> pos, Vector2<double> size) :
        UIComponent(win, pos, size),
        m_spritePath(sprite)
{
    m_sprite = m_window.LoadImage(sprite);
}

Button::~Button()
{
    SDL_FreeSurface(m_sprite);
}

void Button::Draw()
{
    SDL_Rect bgPos;

    bgPos.x = m_pos.x;
    bgPos.y = m_pos.y;
    bgPos.w = m_size.x;
    bgPos.h = m_size.y;
    SDL_BlitScaled(m_sprite, NULL, m_window.GetSurface(), &bgPos);
}

void Button::OnMouseButtonUp(SDL_MouseMotionEvent)
{

}

void Button::OnHover(SDL_MouseMotionEvent mouse)
{
    if (IsHovered(mouse))
    {
        SDL_SetSurfaceColorMod(m_sprite, 125, 125, 125);
    }
    else
    {
        SDL_SetSurfaceColorMod(m_sprite, 255, 255, 255);
    }
}

void Button::OnMouseButtonDown(SDL_MouseMotionEvent mouse)
{
    if (IsHovered(mouse))
    {
        ExecuteCallbacks();
    }
}
