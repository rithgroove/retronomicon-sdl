#include "retronomicon/input/sdl_input_map.h"

namespace retronomicon::sdl::input {

    using retronomicon::input::Key;
    using retronomicon::input::InputState;

    void SDLInputMap::updateFromRawInput(const SDLRawInput& raw,
                                         InputState& state) const
    {
        // --- Update actions ---
        for (const auto& [key, actionName] : getActionBindings()) {
            bool pressed = raw.isKeyPressed(key);
            state.setAction(actionName, pressed);
        }

        // --- Update axes ---
        for (const auto& [axisName, bindings] : getAxisBindings()) {
            float value = 0.f;

            for (const auto& [key, weight] : bindings) {
                if (raw.isKeyPressed(key)) {
                    value += weight;
                }
            }

            state.setAxis(axisName, value);
        }
    }

} // namespace retronomicon::input
