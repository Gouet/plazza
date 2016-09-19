//
// Created by veyrie_f on 4/19/16.
//

#include "WindowManagerSDL.hpp"
#include "TextBox.hpp"
#include "Button.hpp"
#include "ScrollBar.hpp"
#include "Panel.hpp"
#include "InfoBox.hpp"

// TODO: handle different commands
WindowManagerSDL::WindowManagerSDL()
{
    m_screen = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        throw SdlException("Failed to init video");
    if ((m_screen = SDL_CreateWindow("Plazza", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0)) == NULL)
        throw SdlException("Failed to create window");
    if (TTF_Init() == -1)
        throw SdlException("Failed to init TTF");
    if ((m_renderer = SDL_CreateRenderer(m_screen, -1, SDL_RENDERER_ACCELERATED)) == NULL)
    {
        throw SdlException("Failed to init rendering");
    }
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    InstantiateUI();
    CreateDelegates();
}

WindowManagerSDL::~WindowManagerSDL()
{
    delete m_mainPanel;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_screen);
    SDL_Quit();
}

void WindowManagerSDL::Loop(NamedPipe *commandLine, NamedPipe *output)
{
    m_event = {0};

    m_commandLine = commandLine;
    m_output = output;
    while (m_isRunning)
    {
        // Clear screen
        SDL_FillRect(SDL_GetWindowSurface(m_screen), NULL, 0);

        UpdateFiles(m_event.motion);

        DrawUI();

        SDL_UpdateWindowSurface(m_screen);

        if (m_isRunning)
        {
            SDL_WaitEvent(&m_event);
        }
        UpdateFiles(m_event.motion);

        switch (m_event.type)
        {
            case SDL_QUIT:
                m_isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (m_event.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_isRunning = false;
                }

                break;

            // if the mouse moves, checks hover for all the components
            case SDL_MOUSEMOTION:
                m_mainPanel->OnHover(m_event.motion);
                for (std::vector<std::pair<UIComponent*, FileType> >::const_iterator it = m_selectedFiles.begin(); it != m_selectedFiles.end(); ++it)
                {
                    (*it).first->OnHover(m_event.motion);
                }
                break;

            // if the user clicks, check on each component
            case SDL_MOUSEBUTTONUP:
                m_mainPanel->OnMouseButtonUp(m_event.motion);
                for (std::vector<std::pair<UIComponent*, FileType> >::const_iterator it = m_selectedFiles.begin(); it != m_selectedFiles.end(); ++it)
                {
                    (*it).first->OnMouseButtonUp(m_event.motion);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                m_mainPanel->OnMouseButtonDown(m_event.motion);
                for (std::vector<std::pair<UIComponent*, FileType> >::const_iterator it = m_selectedFiles.begin(); it != m_selectedFiles.end(); ++it)
                {
                    (*it).first->OnMouseButtonDown(m_event.motion);
                }
                break;
            default:
                break;
        }

        // Garbage is being collected
        std::vector<std::pair<UIComponent*, FileType> >::iterator it = m_selectedFiles.begin();
        while (it != m_selectedFiles.end())
        {
            if ((*it).first->GetDestruction())
            {
                delete (*it).first;
                it = m_selectedFiles.erase(it);
                if ((*m_mainPanel)["ScrollBar2"] != nullptr)
                {
                    dynamic_cast<ScrollBar*>((*m_mainPanel)["ScrollBar2"])->SetMaxValue(m_selectedFiles.size() - 9);
                }
            }
            else
            {
                ++it;
            }
        }
    }
}

SDL_Surface *WindowManagerSDL::LoadImage(std::string const &path)
{
    SDL_Surface *s_opti;
    SDL_Surface *s = IMG_Load(path.c_str());

    if (s == nullptr)
    {
        return nullptr;
    }

    s_opti = SDL_ConvertSurface(s, s->format, 0);
    //SDL_SetSurfaceAlphaMod(s_opti, SDL_ALPHA_TRANSPARENT);
    SDL_FreeSurface(s);
    return s_opti;
}

void WindowManagerSDL::DrawUI() const
{
    double i = 0;
    double yPos = 75;

    // Draws the main panel
    m_mainPanel->Draw();

    // Draws the right panel content
    for (std::vector<std::pair<UIComponent*, FileType> >::const_iterator it = m_selectedFiles.begin(); it != m_selectedFiles.end(); ++it)
    {
        if ((*it).second == m_fileType)
        {
            if (i >= m_selectedIndex && i <= m_selectedIndex + 8)
            {
                (*it).first->SetPosition(Vector2<double>(WIDTH * 0.5 + 65, yPos += 30));
                (*it).first->Draw();
            }
            ++i;
        }
    }

    // Draws the bottom panel content
    i = 0;
    yPos = 15;
    for (std::vector<UIComponent*>::const_iterator it = m_outputFiles.begin(); it != m_outputFiles.end(); ++it)
    {
        if (i >= m_outputIndex && i <= m_outputIndex + 8)
        {
            (*it)->SetPosition(Vector2<double>(50, HEIGHT / 2.0 + yPos));
            (*it)->Draw();
            yPos += 40;
        }
        ++i;
    }
}

SDL_Surface *WindowManagerSDL::GetSurface() const
{
    return SDL_GetWindowSurface(m_screen);
}

void WindowManagerSDL::UpdateFiles(SDL_MouseMotionEvent mouse)
{
    size_t i = 0;
    double yPos = 75;
    std::list<dirent>::const_iterator dirs;
    UIComponent *newComponent;

    m_explorer.openDirectory();

    // Make sure we remove all the text box from the display list !!!!
    for (i = 0; i < 1000; ++i)
    {
        (*m_mainPanel).RemoveComponent("TextFile" + std::to_string(i));
    }
    m_textFiles.clear();

    i = 0;
    if ((*m_mainPanel)["ScrollBar1"] != nullptr && dynamic_cast<ScrollBar*>((*m_mainPanel)["ScrollBar1"]) != nullptr)
    {
        dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar1"])->SetMaxValue(static_cast<double>(m_explorer.getFiles().size()) - 10);
    }
    for (dirs = m_explorer.getFiles().begin(); dirs != m_explorer.getFiles().end(); dirs++)
    {
        if (i >= m_fileIndex && i <= m_fileIndex + 8)
        {
            newComponent = new TextBox(*this, (*dirs).d_name, "SDL/police.ttf", 20, Vector2<double>(40, yPos += 30),
                                       Vector2<double>(1, 1), ((*dirs).d_type == 4) ? SDL_Color{155, 0, 155, 255} : SDL_Color{125, 125, 125, 255}), "TextFile" + std::to_string(i);

            if ((*dirs).d_type == 4)
            {
                // We use the lambda capturing by value and not by reference because int i is destroyed out of scope
                dynamic_cast<TextBox *>(newComponent)->AddCallback<void()>([=]()
                                                                           {
                                                                               if ((*m_mainPanel)["TextFile" + std::to_string(i)] != nullptr)
                                                                               {
                                                                                   TextBox *text = dynamic_cast<TextBox *>((*m_mainPanel)["TextFile" + std::to_string(i)]);
                                                                                   m_explorer += text->GetText();
                                                                               }
                                                                           });
            }
            else
            {
                // Catch by reference because we modify the vector
                dynamic_cast<TextBox *>(newComponent)->AddCallback<void()>([=]()
                                                                           {
                                                                               UIComponent *textBox = new TextBox(*this, dynamic_cast<TextBox *>(newComponent)->GetText(), "SDL/police.ttf", 20, Vector2<double>(WIDTH / 2.0 + 65, 105), Vector2<double>(1, 1), SDL_Color{125, 125, 125, 255});
                                                                               dynamic_cast<TextBox*>(textBox)->AddCallback<void()>([=]()
                                                                                                                                    {
                                                                                                                                        std::vector<std::pair<UIComponent*, FileType>>::iterator it = m_selectedFiles.begin();
                                                                                                                                        for (; it != m_selectedFiles.end(); it++)
                                                                                                                                        {
                                                                                                                                            if ((*it).first == textBox)
                                                                                                                                            {
                                                                                                                                                (*it).first->SetDestruction(true);
                                                                                                                                                break;
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    });
                                                                               m_selectedFiles.push_back(std::pair<UIComponent*, FileType>(textBox, m_fileType));
                                                                               // Update scroll bar
                                                                               if ((*m_mainPanel)["ScrollBar2"] != nullptr)
                                                                               {
                                                                                   dynamic_cast<ScrollBar*>((*m_mainPanel)["ScrollBar2"])->SetMaxValue(
                                                                                           static_cast<double>(m_selectedFiles.size()) - 10);
                                                                               }
                                                                           });
            }
            newComponent->OnHover(mouse);
            m_mainPanel->AddComponent(newComponent, "TextFile" + std::to_string(i));
            m_textFiles.push_back(newComponent);
        }
        ++i;
    }
}

// All the UI elements are created here
void WindowManagerSDL::InstantiateUI()
{
    // Main panel
    m_mainPanel = new Panel(*this, "", Vector2<double>(0, 0), Vector2<double>(WIDTH, HEIGHT));

    // Left panel
    m_mainPanel->AddComponent(new Panel(*this, "SDL/Sprites/BlackWindow.bmp", Vector2<double>(0, 0),
                                        Vector2<double>(WIDTH / 2.0 - 25, HEIGHT / 2.0)), "PanelLeft");
    m_mainPanel->AddComponent(new TextBox(*this, "Files", "SDL/police.ttf", 50, Vector2<double>(250, 25),
                                          Vector2<double>(1, 1), SDL_Color{125, 125, 125, 255}), "PanelLeftTitle");
    m_mainPanel->AddComponent(new ScrollBar(*this, Vector2<double>(WIDTH / 2.0 - 80, 100), Vector2<double>(30, 280), 0,
                                            1, "SDL/Sprites/ScrollBarBlack.bmp", "SDL/Sprites/ButtonScrollBlack.bmp"),
                              "ScrollBar1");

    // Right panel
    m_mainPanel->AddComponent(new Panel(*this, "SDL/Sprites/BlackWindow.bmp", Vector2<double>(WIDTH / 2.0 + 25, 0),
                                        Vector2<double>(WIDTH / 2.0, HEIGHT / 2.0)), "PanelRight");
    m_mainPanel->AddComponent(new TextBox(*this, "IP Address", "SDL/police.ttf", 50, Vector2<double>(WIDTH * 0.65, 25),
                                          Vector2<double>(1, 1), SDL_Color{125, 125, 125, 255}), "PanelRightTitle");
    m_mainPanel->AddComponent(new ScrollBar(*this, Vector2<double>(WIDTH - 50, 100), Vector2<double>(30, 280), 0, 1,
                                            "SDL/Sprites/ScrollBarBlack.bmp", "SDL/Sprites/ButtonScrollBlack.bmp"),
                              "ScrollBar2");
    m_mainPanel->AddComponent(new Button(*this, "SDL/Sprites/ArrowLeft.bmp", Vector2<double>(WIDTH * 0.5 + 60, 32),
                                         Vector2<double>(50, 50)), "ZButtonPrev");
    m_mainPanel->AddComponent(new Button(*this, "SDL/Sprites/ArrowRight.bmp", Vector2<double>(WIDTH - 85, 32),
                                         Vector2<double>(50, 50)), "ZButtonNext");

    // Bottom panel
    m_mainPanel->AddComponent(new Panel(*this, "SDL/Sprites/WhiteWindow.bmp", Vector2<double>(0, HEIGHT / 2.0),
                                        Vector2<double>(WIDTH, HEIGHT / 2.0)), "PanelBottom");
    m_mainPanel->AddComponent(new ScrollBar(*this, Vector2<double>(WIDTH - 70, HEIGHT / 2.0 + 20),
                                            Vector2<double>(30, HEIGHT / 2.0 - 40), 0, 1,
                                            "SDL/Sprites/ScrollBarWhite.bmp", "SDL/Sprites/ButtonScrollWhite.bmp"),
                              "ScrollBar3");

    // Middle button
    m_mainPanel->AddComponent(new Button(*this, "SDL/Sprites/ButtonOk.bmp", Vector2<double>(WIDTH / 2.0 - 25, 180),
                                         Vector2<double>(50, 50)), "ButtonOk");

    // Pop up threads
    UIComponent *popUp = new Panel(*this, "SDL/Sprites/WhiteWindow.bmp", Vector2<double>(10, HEIGHT - 400),
                                   Vector2<double>(WIDTH * 0.5, HEIGHT * 0.4));
    //popUp->AddComponent(new TextBox(*this, "Threads", "SDL/police.ttf", 50, Vector2<double>(200, HEIGHT / 2.0 + 10),
    // Vector2<double>(1, 1), SDL_Color{125, 125, 125, 255}), "PopUpThreadTitle");
    m_mainPanel->AddComponent(new InfoBox(*this, Vector2<double>(0, HEIGHT - 50), Vector2<double>(50, 50),
                                          "SDL/Sprites/ButtonAdd.bmp", popUp), "PopUpThread");
}

void WindowManagerSDL::CreateDelegates()
{

    (*m_mainPanel)["ScrollBar1"]->AddCallback<void()>([&]()
                                                      {
                                                          if ((*m_mainPanel)["ScrollBar1"] != nullptr && dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar1"]) != nullptr)
                                                          {
                                                              m_fileIndex = dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar1"])->GetValue();
                                                          }
                                                      });

    (*m_mainPanel)["ScrollBar2"]->AddCallback<void()>([&]()
                                                      {
                                                          if ((*m_mainPanel)["ScrollBar2"] != nullptr && dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar2"]) != nullptr)
                                                          {
                                                              m_selectedIndex = dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar2"])->GetValue();
                                                          }
                                                      });
    (*m_mainPanel)["ScrollBar3"]->AddCallback<void()>([&]()
                                                      {
                                                          if ((*m_mainPanel)["ScrollBar3"] != nullptr && dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar3"]) != nullptr)
                                                          {
                                                              m_outputIndex = dynamic_cast<ScrollBar *>((*m_mainPanel)["ScrollBar3"])->GetValue();
                                                          }
                                                      });
    (*m_mainPanel)["ZButtonPrev"]->AddCallback<void()>([&]()
                                                       {
                                                           m_fileType = static_cast<FileType>((m_fileType + 1) % 3);
                                                           dynamic_cast<TextBox*>((*m_mainPanel)["PanelRightTitle"])->SetText(m_fileString[m_fileType]);
                                                       });
    (*m_mainPanel)["ZButtonNext"]->AddCallback<void()>([&]()
                                                       {
                                                           m_fileType = static_cast<FileType>(static_cast<int>(m_fileType) - 1);
                                                           if (m_fileType < E_IP_ADDRESS)
                                                           {
                                                               m_fileType = E_PHONE_NUMBER;
                                                           }
                                                           dynamic_cast<TextBox*>((*m_mainPanel)["PanelRightTitle"])->SetText(m_fileString[m_fileType]);
                                                       });

    (*m_mainPanel)["ButtonOk"]->AddCallback<void()>([&]()
                                                    {
                                                        StartScrapper();
                                                    });
}

// Starts the plazza command
void WindowManagerSDL::StartScrapper()
{
    std::string str = "";
    for (std::vector<std::pair<UIComponent*, FileType>>::iterator it = m_selectedFiles.begin(); it != m_selectedFiles.end(); ++it)
    {
        str += dynamic_cast<TextBox*>((*it).first)->GetText() + (((*it).second == E_IP_ADDRESS) ? " IP_ADDRESS;"
                                                                : ((*it).second == E_PHONE_NUMBER) ? " PHONE_NUMBER;"
                                                                : " EMAIL_ADDRESS;");
    }

    if (str.empty())
    {
        return;
    }

    (*m_commandLine) << str;

    std::string out;
    bool hastoQuit = false;
    size_t posEOF;
    while (!hastoQuit)
    {
        (*m_output) >> out;

        if (out.empty())
            break;
        if ((posEOF = out.find(EOF)) != std::string::npos)
        {
            hastoQuit = true;
            out.erase(posEOF);
        }
        size_t pos = 0;
        while ((pos = out.find('\n')) != std::string::npos)
        {
            std::string order;

            order = out.substr(0, pos);
            out.erase(0, pos + 1);
            m_stringOutput.push_back(order);
        }
    }

    size_t i;
    for (i = 0; i < 10000; ++i)
    {
        m_mainPanel->RemoveComponent("TextOutput" + std::to_string(i));
    }
    double yPos = 15;
    for (auto it : m_stringOutput)
    {
        m_outputFiles.push_back(new TextBox(*this, it, "SDL/police.ttf", 50, Vector2<double>(50, HEIGHT / 2.0 + yPos),
                                Vector2<double>(1, 1), SDL_Color{185, 185, 185, 255}));
        if ((*m_mainPanel)["ScrollBar3"] != nullptr)
        {
            dynamic_cast<ScrollBar*>((*m_mainPanel)["ScrollBar3"])->SetMaxValue(static_cast<double>(m_stringOutput.size()) - 8);
        }
        yPos += 45;
    }
    m_stringOutput.empty();
}
