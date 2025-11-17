#pragma once

#include <SDL2/SDL.h>
#include "retronomicon/input/key.h"

namespace retronomicon::sdl::input {

    inline SDL_Keycode toSDLKey(retronomicon::input::Key key) {
        using retronomicon::input::Key;

        switch (key) {

            // --- Alphabet ---
            case Key::A: return SDLK_a;
            case Key::B: return SDLK_b;
            case Key::C: return SDLK_c;
            case Key::D: return SDLK_d;
            case Key::E: return SDLK_e;
            case Key::F: return SDLK_f;
            case Key::G: return SDLK_g;
            case Key::H: return SDLK_h;
            case Key::I: return SDLK_i;
            case Key::J: return SDLK_j;
            case Key::K: return SDLK_k;
            case Key::L: return SDLK_l;
            case Key::M: return SDLK_m;
            case Key::N: return SDLK_n;
            case Key::O: return SDLK_o;
            case Key::P: return SDLK_p;
            case Key::Q: return SDLK_q;
            case Key::R: return SDLK_r;
            case Key::S: return SDLK_s;
            case Key::T: return SDLK_t;
            case Key::U: return SDLK_u;
            case Key::V: return SDLK_v;
            case Key::W: return SDLK_w;
            case Key::X: return SDLK_x;
            case Key::Y: return SDLK_y;
            case Key::Z: return SDLK_z;

            // --- Digits ---
            case Key::Num0: return SDLK_0;
            case Key::Num1: return SDLK_1;
            case Key::Num2: return SDLK_2;
            case Key::Num3: return SDLK_3;
            case Key::Num4: return SDLK_4;
            case Key::Num5: return SDLK_5;
            case Key::Num6: return SDLK_6;
            case Key::Num7: return SDLK_7;
            case Key::Num8: return SDLK_8;
            case Key::Num9: return SDLK_9;

            // --- Function keys ---
            case Key::F1:  return SDLK_F1;
            case Key::F2:  return SDLK_F2;
            case Key::F3:  return SDLK_F3;
            case Key::F4:  return SDLK_F4;
            case Key::F5:  return SDLK_F5;
            case Key::F6:  return SDLK_F6;
            case Key::F7:  return SDLK_F7;
            case Key::F8:  return SDLK_F8;
            case Key::F9:  return SDLK_F9;
            case Key::F10: return SDLK_F10;
            case Key::F11: return SDLK_F11;
            case Key::F12: return SDLK_F12;

            // --- Modifiers ---
            case Key::LeftShift:   return SDLK_LSHIFT;
            case Key::RightShift:  return SDLK_RSHIFT;
            case Key::LeftControl: return SDLK_LCTRL;
            case Key::RightControl:return SDLK_RCTRL;
            case Key::LeftAlt:     return SDLK_LALT;
            case Key::RightAlt:    return SDLK_RALT;
            case Key::LeftSuper:   return SDLK_LGUI;
            case Key::RightSuper:  return SDLK_RGUI;

            // --- Arrow keys ---
            case Key::Up:    return SDLK_UP;
            case Key::Down:  return SDLK_DOWN;
            case Key::Left:  return SDLK_LEFT;
            case Key::Right: return SDLK_RIGHT;

            // --- Special keys ---
            case Key::Space:     return SDLK_SPACE;
            case Key::Enter:     return SDLK_RETURN;
            case Key::Escape:    return SDLK_ESCAPE;
            case Key::Tab:       return SDLK_TAB;
            case Key::Backspace: return SDLK_BACKSPACE;
            case Key::DeleteKey: return SDLK_DELETE;
            case Key::Insert:    return SDLK_INSERT;
            case Key::Home:      return SDLK_HOME;
            case Key::End:       return SDLK_END;
            case Key::PageUp:    return SDLK_PAGEUP;
            case Key::PageDown:  return SDLK_PAGEDOWN;

            // --- Punctuation ---
            case Key::Minus:        return SDLK_MINUS;
            case Key::Plus:         return SDLK_EQUALS;
            case Key::Comma:        return SDLK_COMMA;
            case Key::Period:       return SDLK_PERIOD;
            case Key::Slash:        return SDLK_SLASH;
            case Key::Semicolon:    return SDLK_SEMICOLON;
            case Key::Apostrophe:   return SDLK_QUOTE;
            case Key::LeftBracket:  return SDLK_LEFTBRACKET;
            case Key::RightBracket: return SDLK_RIGHTBRACKET;
            case Key::Backslash:    return SDLK_BACKSLASH;
            case Key::Grave:        return SDLK_BACKQUOTE;

            // --- Numpad ---
            case Key::Numpad0:        return SDLK_KP_0;
            case Key::Numpad1:        return SDLK_KP_1;
            case Key::Numpad2:        return SDLK_KP_2;
            case Key::Numpad3:        return SDLK_KP_3;
            case Key::Numpad4:        return SDLK_KP_4;
            case Key::Numpad5:        return SDLK_KP_5;
            case Key::Numpad6:        return SDLK_KP_6;
            case Key::Numpad7:        return SDLK_KP_7;
            case Key::Numpad8:        return SDLK_KP_8;
            case Key::Numpad9:        return SDLK_KP_9;
            case Key::NumpadAdd:      return SDLK_KP_PLUS;
            case Key::NumpadSubtract: return SDLK_KP_MINUS;
            case Key::NumpadMultiply: return SDLK_KP_MULTIPLY;
            case Key::NumpadDivide:   return SDLK_KP_DIVIDE;
            case Key::NumpadEnter:    return SDLK_KP_ENTER;
            case Key::NumpadDecimal:  return SDLK_KP_DECIMAL;

            // --- Mouse buttons ---
            case Key::MouseLeft:   return SDL_BUTTON_LEFT;
            case Key::MouseRight:  return SDL_BUTTON_RIGHT;
            case Key::MouseMiddle: return SDL_BUTTON_MIDDLE;

            // --- Unknown ---
            default: return SDLK_UNKNOWN;
        }
    }

} // namespace retronomicon::sdl::input
