#pragma once

#include "retronomicon/asset/music_asset.h"
#include <SDL2/SDL_audio.h>
#include <string>
#include <vector>

namespace retronomicon::sdl::asset {

    /**
     * @brief SDL-based music asset that ONLY decodes audio data (WAV/OGG)
     *        and does NOT interact with SDL_AudioDevice directly.
     *
     * The SDL audio backend will handle the playback and streaming.
     */
    class SDLMusicAsset final : public retronomicon::asset::MusicAsset {
    public:
        SDLMusicAsset(const std::string& path);
        SDLMusicAsset(const std::string& path, const std::string& name);

        ~SDLMusicAsset() override = default;

        /**
         * @brief Decode WAV or OGG into raw PCM data in memory.
         *
         * @param outPCM      Raw PCM bytes
         * @param outFmt      SDL_AudioFormat (AUDIO_S16LSB, etc.)
         * @param outFreq     Sampling rate (Hz)
         * @param outChannels Number of channels (1 or 2)
         */
        bool decode(std::vector<Uint8>& outPCM,
                    SDL_AudioFormat& outFmt,
                    int& outFreq,
                    int& outChannels);

        // Dummy overrides (SDL backend owns playback)
        bool load() override { return true; }
        void unload() override {}
        void play(bool loop = true) override {}
        void stop() override {}
        
        std::string to_string() const override;

    private:
        bool loadWavFile(const std::string& path,
                         std::vector<Uint8>& outPCM,
                         SDL_AudioFormat& outFmt,
                         int& outFreq,
                         int& outChannels);

        bool loadOggFile(const std::string& path,
                         std::vector<Uint8>& outPCM,
                         SDL_AudioFormat& outFmt,
                         int& outFreq,
                         int& outChannels);
    };

}
