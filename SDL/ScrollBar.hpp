//
// Created by veyrie_f on 4/20/16.
//

#ifndef CPP_PLAZZA_SCROLLBAR_HPP
#define CPP_PLAZZA_SCROLLBAR_HPP


#include "UIComponent.hpp"
#include "Button.hpp"

/**
 * \brief Scroll Bar ui element for SDL
 */
class ScrollBar : public UIComponent
{
public:
    ScrollBar(WindowManagerSDL &win, Vector2<double> pos, Vector2<double> size,
              double minVal, double maxVal, std::string const& background, std::string const& button);
    virtual ~ScrollBar();

public:
    void Draw();
    void OnMouseButtonUp(SDL_MouseMotionEvent mouse);
    void OnMouseButtonDown(SDL_MouseMotionEvent mouse);
    void OnHover(SDL_MouseMotionEvent mouse);
    double GetValue() const;
    void SetMaxValue(double max);

private:
    void UpdateScrollBar(double yVal);

private:
    double m_minVal = 0;
    double m_maxVal = 1;
    double m_value = 0;
    SDL_Surface *m_background;
    Button m_button;
    bool m_isButtonSelected = false;
};


#endif //CPP_PLAZZA_SCROLLBAR_HPP
