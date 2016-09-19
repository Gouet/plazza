//
// Created by veyrie_f on 4/20/16.
//

#include "ScrollBar.hpp"

ScrollBar::ScrollBar(WindowManagerSDL &win, Vector2<double> pos, Vector2<double> size, double minVal, double maxVal,
                     std::string const &background, std::string const &button) :
        UIComponent(win, pos, size),
        m_minVal(minVal),
        m_maxVal(maxVal),
        m_background(win.LoadImage(background)),
        m_button(win, button, pos, size)
{
    m_button.SetSize(Vector2<double>(m_size.x, m_size.x));
}

ScrollBar::~ScrollBar()
{

}

void ScrollBar::Draw()
{
    SDL_Rect bgPos;

    bgPos.x = m_pos.x;
    bgPos.y = m_pos.y;
    bgPos.w = m_size.x;
    bgPos.h = m_size.y;
    SDL_BlitScaled(m_background, NULL, m_window.GetSurface(), &bgPos);

    if (!m_isButtonSelected)
    {
        UpdateScrollBar(m_button.GetPosition().y + m_button.GetSize().y / 2.0);
    }

    m_button.Draw();
}

void ScrollBar::OnMouseButtonUp(SDL_MouseMotionEvent)
{
    m_isButtonSelected = false;
}

void ScrollBar::OnHover(SDL_MouseMotionEvent mouse)
{
    // if the button is being dragged, update pos and values
    if (m_isButtonSelected)
    {
        UpdateScrollBar(mouse.y);
    }
}

void ScrollBar::OnMouseButtonDown(SDL_MouseMotionEvent mouse)
{
    if (m_button.IsHovered(mouse))
    {
        m_isButtonSelected = true;
    }
}

/*
 * \brief Updates button position and slider value
 */
void ScrollBar::UpdateScrollBar(double yVal)
{
    Vector2<double> newPos = m_button.GetPosition();

    newPos.x = m_pos.x;
    newPos.y = WindowManagerSDL::Clamp(m_pos.y, m_pos.y + m_size.y - m_button.GetSize().y, yVal - m_button.GetSize().y / 2.0);
    m_value = (m_button.GetPosition().y - m_pos.y) / (m_size.y - m_button.GetSize().y) * (m_maxVal - m_minVal);
    m_button.SetPosition(newPos);
    ExecuteCallbacks();
}

double ScrollBar::GetValue() const
{
    return m_value;
}

void ScrollBar::SetMaxValue(double max)
{
    if (max < 0)
    {
        max = 0;
    }
    m_maxVal = max;
}
