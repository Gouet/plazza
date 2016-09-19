//
// Created by veyrie_f on 4/20/16.
//

#include "Panel.hpp"

Panel::Panel(WindowManagerSDL &win, std::string const& sprite, Vector2<double> pos, Vector2<double> size) :
        UIComponent(win, pos, size),
        m_sprite(win.LoadImage(sprite)),
        m_originalSize(size)
{
}

Panel::~Panel()
{
    std::map<std::string, UIComponent*>::iterator it;

    for (it = m_components.begin(); it != m_components.end(); it++)
    {
        delete it->second;
    }
    SDL_FreeSurface(m_sprite);
}

void Panel::AddComponent(UIComponent *component, std::string const &componentName)
{
    m_components[componentName] = component;
}

void Panel::Draw()
{
    SDL_Rect bgPos;
    std::map<std::string, UIComponent*>::iterator it;

    bgPos.x = m_pos.x;
    bgPos.y = m_pos.y;
    bgPos.w = m_size.x;
    bgPos.h = m_size.y;
    SDL_BlitScaled(m_sprite, NULL, m_window.GetSurface(), &bgPos);

    for (it = m_components.begin(); it != m_components.end(); it++)
    {
        // Change children pos and size according to their parent
        if ((*it).second != nullptr)
        {
            /* Should resize
            (*it).second->SetPosition(Vector2<double>((*it).second->GetPositionOriginal()) + m_pos);
            Vector2<double> newSize = (*it).second->GetSize();
            newSize.x *= m_size.x / m_originalSize.x;
            newSize.y *= m_size.y / m_originalSize.y;
            (*it).second->SetSize(newSize);*/
            (*it).second->Draw();
        }
    }
}

void Panel::OnMouseButtonUp(SDL_MouseMotionEvent mouse)
{
    std::map<std::string, UIComponent*>::iterator it;

    for (it = m_components.begin(); it != m_components.end(); it++)
    {
        (*it).second->OnMouseButtonUp(mouse);
    }
}

void Panel::OnMouseButtonDown(SDL_MouseMotionEvent mouse)
{
    std::map<std::string, UIComponent*>::iterator it;

    for (it = m_components.begin(); it != m_components.end(); it++)
    {
        (*it).second->OnMouseButtonDown(mouse);
    }
}

void Panel::OnHover(SDL_MouseMotionEvent mouse)
{
    std::map<std::string, UIComponent*>::iterator it;

    for (it = m_components.begin(); it != m_components.end(); it++)
    {
        (*it).second->OnHover(mouse);
    }
}

UIComponent *Panel::operator[](std::string const &component) const
{
    if (m_components.find(component) == m_components.end())
    {
        return nullptr;
    }

    return m_components[component];
}

void Panel::RemoveComponent(std::string const &componentName)
{
    if (m_components.find(componentName) != m_components.end())
    {
        delete m_components[componentName];
        m_components.erase(componentName);
    }
}
