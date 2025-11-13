#include "retronomicon/input/sdl_raw_input.h"

namespace retronomicon::input {

    SDLRawInput::SDLRawInput(SDL_Window* window)
        : m_window(window), m_mouseX(0), m_mouseY(0), m_mouseButtons(0) {
    }

    void SDLRawInput::poll() {
        m_events.clear();

        SDL_Event event;
        m_mouseButtons = 0;

        // Poll all SDL events
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

                default:
                    break;
            }
        }

        // Mouse state
        Uint32 sdlButtons = SDL_GetMouseState(&m_mouseX, &m_mouseY);

        if (sdlButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
            m_mouseButtons |= 1 << 0;
        if (sdlButtons & SDL_BUTTON(SDL_BUTTON_RIGHT))
            m_mouseButtons |= 1 << 1;
        if (sdlButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
            m_mouseButtons |= 1 << 2;
    }

    void SDLRawInput::clear() {
        m_events.clear();
    }

    const std::vector<std::string>& SDLRawInput::getEvents() const {
        return m_events;
    }

    bool SDLRawInput::isKeyPressed(int keyCode) const {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[SDL_GetScancodeFromKey(keyCode)] != 0;
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

} // namespace retronomicon::input
