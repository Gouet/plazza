//
// Created by veyrie_f on 4/19/16.
//

#include "TextBox.hpp"

TextBox::TextBox(WindowManagerSDL &win) :
        UIComponent(win, Vector2<double>(0, 0), Vector2<double>(0, 0))
{

}

TextBox::TextBox(WindowManagerSDL &win, std::string const &text, std::string const& font, int fontSize, Vector2<double> pos, Vector2<double> size, SDL_Color color) :
        UIComponent(win, pos, size),
        m_text(text),
        m_fontName(font),
        m_fontSize(fontSize),
        m_color(color),
        m_originalColor(color)
{
    // in this particular case the size is the word length by its font size
    m_size.y = fontSize;
    m_size.x = (fontSize / 2.3) * text.size();
}

TextBox::~TextBox()
{
    TTF_CloseFont(m_font);
}

void TextBox::Draw()
{
    SDL_Rect rect;
    SDL_Surface *text;

    rect.x = m_pos.x;
    rect.y = m_pos.y;

    if (m_font == nullptr)
    {
        m_font = TTF_OpenFont(m_fontName.c_str(), m_fontSize);
    }

    if (m_font == nullptr)
    {
        throw WindowManagerSDL::SdlException("Failed to open font " + m_fontName);
    }

    text = TTF_RenderText_Blended(m_font, m_text.c_str(), m_color);
    SDL_BlitSurface(text, NULL, m_window.GetSurface(), &rect);
    free(text);
}

void TextBox::OnMouseButtonUp(SDL_MouseMotionEvent mouse)
{
    if (IsHovered(mouse))
    {
        ExecuteCallbacks();
    }
}

void TextBox::OnHover(SDL_MouseMotionEvent mouse)
{
    if (IsHovered(mouse))
    {
        m_color.r = m_originalColor.r / 2.0;
        m_color.g = m_originalColor.g / 2.0;
        m_color.b = m_originalColor.b / 2.0;
    }
    else
    {
        m_color = m_originalColor;
    }
}

void TextBox::OnMouseButtonDown(SDL_MouseMotionEvent)
{

}

void TextBox::SetText(std::string const &text)
{
    m_text = text;
}

std::string const &TextBox::GetText() const
{
    return m_text;
}

void TextBox::SetColor(SDL_Color color)
{
    m_color = color;
}
