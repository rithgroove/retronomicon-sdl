#pragma once

#include "retronomicon/input/input_map.h"
#include "retronomicon/input/input_state.h"
#include "retronomicon/input/sdl_raw_input.h"

namespace retronomicon::sdl::input {

    class SDLInputMap : public InputMap {
    public:
        using InputMap::InputMap; // inherit constructors

        /**
         * @brief Update InputState using current RawInput (SDL backend)
         */
        void updateFromRawInput(const SDLRawInput& raw, InputState& state) const;
    };

} // namespace retronomicon::input
