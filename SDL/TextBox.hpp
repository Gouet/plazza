//
// Created by veyrie_f on 4/19/16.
//

#ifndef CPP_PLAZZA_TEXTBOX_HPP
#define CPP_PLAZZA_TEXTBOX_HPP

#include <SDL2/SDL_pixels.h>
#include "UIComponent.hpp"

class TextBox : public UIComponent
{
public:
    TextBox(WindowManagerSDL &win);
    TextBox(WindowManagerSDL &win, std::string const& text, std::string const& font, int fontSize,
            Vector2<double> pos, Vector2<double> size, SDL_Color color);
    virtual ~TextBox();

public:
    void Draw();
    void OnMouseButtonUp(SDL_MouseMotionEvent mouse);
    void OnMouseButtonDown(SDL_MouseMotionEvent mouse);
    void OnHover(SDL_MouseMotionEvent mouse);
    void SetText(std::string const& text);
    void SetColor(SDL_Color color);
    std::string const& GetText() const;

private:
    std::string m_text = "";
    std::string m_fontName = "";
    int m_fontSize = 20;
    TTF_Font *m_font = nullptr;
    SDL_Color m_color;
    SDL_Color m_originalColor;
};


#endif //CPP_PLAZZA_TEXTBOX_HPP
