#include "retronomicon/input/sdl_raw_input.h"
#include "retronomicon/input/sdl_key.h"
#include <SDL2/SDL.h>
#include <iostream>
namespace retronomicon::sdl::input {

    SDLRawInput::SDLRawInput(SDL_Window* window)
        : m_window(window), m_mouseX(0), m_mouseY(0), m_mouseButtons(0) {
    }

    void SDLRawInput::poll() {
        m_events.clear();
        m_mouseButtons = 0;

        SDL_Event event;
        std::cout<<"poll called"<<std::endl;
        // Process all SDL events (same idea as GLFW polling)
        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_QUIT:
                    m_events.push_back("QUIT");
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        m_events.push_back("ESCAPE_PRESSED");
                    }
                    break;
            }
        }

        // --- Mouse position & buttons (same as GLFWRawInput::poll) ---
        Uint32 state = SDL_GetMouseState(&m_mouseX, &m_mouseY);

        if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
            m_mouseButtons |= (1 << 0);

        if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
            m_mouseButtons |= (1 << 1);

        if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
            m_mouseButtons |= (1 << 2);
    }

    void SDLRawInput::clear() {
        m_events.clear();
    }

    const std::vector<std::string>& SDLRawInput::getEvents() const {
        return m_events;
    }

    bool SDLRawInput::isKeyPressed(Key key) const {

        SDL_Keycode sdlKey = toSDLKey(key);
        if (sdlKey == SDLK_UNKNOWN)
            return false;

        // Handle mouse buttons first (just like your GLFW version)
        if (key == Key::MouseLeft)
            return (m_mouseButtons & (1 << 0)) != 0;

        if (key == Key::MouseRight)
            return (m_mouseButtons & (1 << 1)) != 0;

        if (key == Key::MouseMiddle)
            return (m_mouseButtons & (1 << 2)) != 0;

        // --- Keyboard state ---
        const Uint8* kstate = SDL_GetKeyboardState(nullptr);
        SDL_Scancode sc = SDL_GetScancodeFromKey(sdlKey);

        return kstate[sc] != 0;
    }

    int SDLRawInput::getMouseX() const {
        return m_mouseX;
    }

    int SDLRawInput::getMouseY() const {
        return m_mouseY;
    }

    unsigned int SDLRawInput::getMouseButtons() const {
        return m_mouseButtons;
    }

} // namespace retronomicon::sdl::input
