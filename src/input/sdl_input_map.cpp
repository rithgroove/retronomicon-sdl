#include "retronomicon/input/sdl_input_map.h"

namespace retronomicon::input {

    void SdlInputMap::updateFromRawInput(const SDLRawInput& raw, InputState& state) const {
        // Action bindings
        for (const auto& [key, action] : getActionBindings()) {
            if (raw.isKeyPressed(key)) {
                state.setAction(action, true);
            } else {
                state.setAction(action, false);
            }
        }

        // Axis bindings
        for (const auto& [axis, bindings] : getAxisBindings()) {
            float value = 0.0f;
            for (const auto& [key, weight] : bindings) {
                if (raw.isKeyPressed(key)) {
                    value += weight;
                }
            }
            state.setAxis(axis, value);
        }
    }

} // namespace retronomicon::input
