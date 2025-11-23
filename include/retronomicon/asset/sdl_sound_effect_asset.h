#pragma once

#include "retronomicon/asset/sound_effect_asset.h"
#include <string>
#include <vector>
#include <SDL2/SDL_audio.h>

namespace retronomicon::sdl::asset {

    /**
     * @brief SDL decoder-only sound effect asset.
     *
     * Decodes WAV/OGG → PCM (int16 or uint8).
     * SDL backend will upload PCM to SDL-Audio or SDL_mixer.
     */
    class SDLSoundEffectAsset final : public retronomicon::asset::SoundEffectAsset {
    public:
        SDLSoundEffectAsset(const std::string& path);
        SDLSoundEffectAsset(std::string path, std::string name);

        ~SDLSoundEffectAsset() override = default;

        /**
         * @brief Decode WAV or OGG audio data into PCM memory.
         *
         * @param outData  Raw PCM bytes.
         * @param outFmt   SDL_AudioFormat (AUDIO_S16LSB, AUDIO_U8, etc.)
         * @param outFreq  Sample rate (Hz)
         * @param outChannels Number of channels (1 or 2)
         */
        bool decode(std::vector<Uint8>& outData,
                    SDL_AudioFormat& outFmt,
                    int& outFreq,
                    int& outChannels);

        // Dummy overrides (assets no longer own audio)
        bool load() override { return true; }
        void unload() override {}
        void play(bool loop = false) override {}
        void stop() override {}

    private:
        bool loadWavFile(const std::string& path,
                         std::vector<Uint8>& data,
                         SDL_AudioFormat& fmt,
                         int& freq,
                         int& channels);

        bool loadOggFile(const std::string& path,
                         std::vector<Uint8>& data,
                         SDL_AudioFormat& fmt,
                         int& freq,
                         int& channels);
    };

} // namespace retronomicon::sdl::asset
