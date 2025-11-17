#pragma once

#include "retronomicon/input/raw_input.h"
#include "retronomicon/input/key.h"
#include "sdl_key.h"   // toSDLKey()
#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace retronomicon::sdl::input {
    using namespace retronomicon::input;

    class SDLRawInput : public RawInput {
    public:
        explicit SDLRawInput(SDL_Window* window);
        ~SDLRawInput() override = default;

        void poll() override;
        void clear() override;

        const std::vector<std::string>& getEvents() const override;

        bool isKeyPressed(Key key) const override;

        int getMouseX() const override;
        int getMouseY() const override;
        unsigned int getMouseButtons() const override;

    private:
        SDL_Window* m_window;
        std::vector<std::string> m_events;

        mutable int m_mouseX;
        mutable int m_mouseY;
        mutable unsigned int m_mouseButtons;
    };

} // namespace retronomicon::input
