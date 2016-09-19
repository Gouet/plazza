//
// Created by veyrie_f on 4/19/16.
//

#ifndef CPP_PLAZZA_UICOMPONENT_HPP
#define CPP_PLAZZA_UICOMPONENT_HPP


#include <functional>
#include <memory>
#include "Vector2.hpp"
#include "WindowManagerSDL.hpp"
#include "Delegate.hpp"

/*
 * \brief Base class for every component of the user interface
 */
class UIComponent
{
public:
    UIComponent(WindowManagerSDL &window, Vector2<double> pos, Vector2<double> size) :
            m_window(window),
            m_pos(pos),
            m_size(size),
            m_posOriginal(m_pos),
            m_sizeOriginal(m_size)
    {}
    virtual ~UIComponent()
    {}

public:
    virtual void SetPosition(Vector2<double> pos)
    {
        m_pos = pos;
    }

    void SetSize(Vector2<double> size)
    {
        m_size = size;
    }

    Vector2<double> const& GetPosition() const
    {
        return m_pos;
    }

    // Raw pos of the ui component
    Vector2<double> const& GetPositionOriginal() const
    {
        return m_posOriginal;
    }

    // Raw size of the ui component
    Vector2<double> const& GetSizeOriginal() const
    {
        return m_sizeOriginal;
    }

    Vector2<double> const& GetSize() const
    {
        return m_size;
    }

    virtual void Draw() = 0;
    virtual void OnMouseButtonUp(SDL_MouseMotionEvent mouse) = 0;
    virtual void OnMouseButtonDown(SDL_MouseMotionEvent mouse) = 0;
    virtual void OnHover(SDL_MouseMotionEvent mouse) = 0;
    virtual void AddComponent(UIComponent *, std::string const&) {}

    virtual bool IsHovered(SDL_MouseMotionEvent mouse) const
    {
        return (mouse.x >= m_pos.x && mouse.x <= m_pos.x + m_size.x
                && mouse.y >= m_pos.y && mouse.y <= m_size.y + m_pos.y);
    }

    // Adds callback to the ui component
    template <class T>
    void AddCallback(std::function<T> f)
    {
        m_delegate.emplace_back(new Delegate<T>(f));
    }

    void SetDestruction(bool destroy)
    {
        m_destroy = destroy;
    }

    // Garbage destruction system
    bool GetDestruction() const
    {
        return m_destroy;
    }

protected:
    void ExecuteCallbacks() const
    {
        std::vector<std::unique_ptr<IDelegate> >::const_iterator it;

        for (it = m_delegate.begin(); it != m_delegate.end(); it++)
        {
            (*it).get()->operator()();
        }
    }

protected:
    WindowManagerSDL &m_window;
    Vector2<double> m_pos;
    Vector2<double> m_size;
    Vector2<double> m_posOriginal;
    Vector2<double> m_sizeOriginal;

    std::vector<std::unique_ptr<IDelegate> > m_delegate;
    bool m_destroy = false;
};

#endif //CPP_PLAZZA_UICOMPONENT_HPP
