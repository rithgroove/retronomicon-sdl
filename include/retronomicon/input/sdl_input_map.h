#pragma once

#include "retronomicon/input/input_map.h"
#include "retronomicon/input/sdl_raw_input.h"
#include "retronomicon/input/input_state.h"
#include <SDL2/SDL.h>

namespace retronomicon::input {

    class SdlInputMap : public InputMap {
    public:
        using InputMap::InputMap; // inherit constructors

        /**
         * @brief Update InputState using current RawInput (SDL)
         */
        void updateFromRawInput(const SDLRawInput& raw, InputState& state) const;
    };

} // namespace retronomicon::input
