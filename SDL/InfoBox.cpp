//
// Created by veyrie_f on 4/21/16.
//

#include "InfoBox.hpp"

InfoBox::InfoBox(WindowManagerSDL &win, Vector2<double> pos, Vector2<double> size, std::string const& sprite, UIComponent *toDisp) :
        UIComponent(win, pos, size),
        m_sprite(win.LoadImage(sprite)),
        m_component(toDisp)
{
    //m_component->SetSize(Vector2<double>(0, 0));
    m_component->SetPosition(Vector2<double>(win.WIDTH, win.HEIGHT));
}

InfoBox::~InfoBox()
{
    SDL_FreeSurface(m_sprite);
}

void InfoBox::Draw()
{
    SDL_Rect bgPos;

    bgPos.x = m_pos.x;
    bgPos.y = m_pos.y;
    bgPos.w = m_size.x;
    bgPos.h = m_size.y;
    SDL_BlitScaled(m_sprite, NULL, m_window.GetSurface(), &bgPos);
    m_component->Draw();
}

void InfoBox::OnMouseButtonUp(SDL_MouseMotionEvent)
{

}

void InfoBox::OnMouseButtonDown(SDL_MouseMotionEvent)
{

}

void InfoBox::OnHover(SDL_MouseMotionEvent mouse)
{
    if (IsHovered(mouse))
    {
        m_component->SetPosition(m_component->GetPositionOriginal());
    }
    else
    {
        m_component->SetPosition(Vector2<double>(m_window.WIDTH, m_window.HEIGHT));
    }
}
