//
// Created by veyrie_f on 4/19/16.
//

#ifndef CPP_PLAZZA_SDL_HPP
#define CPP_PLAZZA_SDL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <map>
#include <vector>
#include "Vector2.hpp"
#include "../ExploreDirectory.hpp"
#include "../NamedPipe.hpp"

class Panel;
class UIComponent;

/*
 * \brief This class represents a complete window manager rendered in SDL
 */
class WindowManagerSDL
{
public:
    const int WIDTH = 1200;
    const int HEIGHT = 800;

public:
    WindowManagerSDL();
    ~WindowManagerSDL();

public:
    void Loop(NamedPipe *commandLine, NamedPipe *output);
    SDL_Surface *GetSurface() const;
    SDL_Surface *LoadImage(std::string const &path);

    // Very handy tool for UI
    static double Clamp(double min, double max, double value)
    {
        return std::max(min, std::min(value, max));
    }

private:
    void DrawUI() const;
    void UpdateFiles(SDL_MouseMotionEvent mouse);
    void InstantiateUI();
    void CreateDelegates();
    void StartScrapper();

public:
    // Exceptions
    class SdlException : public std::runtime_error
    {
    public:
        SdlException (std::string const& msg) : std::runtime_error(msg)
        { }
    };

private:
    SDL_Window *m_screen;
    SDL_Renderer *m_renderer;
    SDL_Event m_event;
    bool m_isRunning = true;

    // explorer file choosing
    enum FileType { E_IP_ADDRESS, E_EMAIL_ADDRESS, E_PHONE_NUMBER };
    FileType m_fileType = E_IP_ADDRESS;
    std::string m_fileString[3] = {"Ip Address", "Email Address", "Phone number"};

    Panel *m_mainPanel;
    // explorer handling
    ExploreDirectory m_explorer;
    size_t m_fileIndex = 0;
    size_t m_selectedIndex = 0;
    size_t m_outputIndex = 0;
    std::vector<UIComponent *> m_textFiles;
    std::vector<std::pair<UIComponent*, FileType> > m_selectedFiles;

    //std::vector<UIComponent *> m_selectedFiles;
    std::vector<UIComponent *> m_outputFiles;

    // Pipes
    NamedPipe *m_commandLine = nullptr;
    NamedPipe *m_output = nullptr;
    std::vector<std::string> m_stringOutput;
};


#endif //CPP_PLAZZA_SDL_HPP
