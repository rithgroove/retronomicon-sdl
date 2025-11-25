#pragma once

#include "retronomicon/asset/music_asset.h"
#include <SDL2/SDL_audio.h>

#include <string>
#include <vector>

namespace retronomicon::sdl::asset {

    /**
     * @brief SDL-based music asset that ONLY decodes audio data (WAV/OGG)
     *        and does NOT interact with SDL_AudioDevice or SDL_mixer playback.
     *
     * The SDL audio backend will handle the actual playback, looping, and streaming.
     *
     * IMPORTANT:
     *   This header stays free of stb_vorbis includes. Implementations should also avoid
     *   stb symbols to prevent duplicate definition conflicts with SDL_mixer.
     */
    class SDLMusicAsset final : public retronomicon::asset::MusicAsset {
    public:
        SDLMusicAsset(const std::string& path);
        SDLMusicAsset(const std::string& path, const std::string& name);

        ~SDLMusicAsset() override = default;

        /**
         * @brief Decode WAV or OGG into raw PCM data.
         *
         * @param outPCM       Raw PCM bytes.
         * @param outFmt       SDL_AudioFormat (e.g., AUDIO_S16LSB).
         * @param outFreq      Sample rate in Hz.
         * @param outChannels  Number of channels (1 = mono, 2 = stereo).
         *
         * @return true on success, false otherwise.
         */
        bool decode(std::vector<Uint8>& outPCM,
                    SDL_AudioFormat& outFmt,
                    int& outFreq,
                    int& outChannels);

        // ---------------------------------------------------------------------
        // Dummy overrides — SDL audio backend owns playback/streaming.
        // ---------------------------------------------------------------------
        bool load() override { return true; }
        void unload() override {}
        void play(bool /*loop*/ = true) override {}
        void stop() override {}

        /**
         * @brief Helper debug string (path + name).
         */
        std::string to_string() const override;

    private:
        /**
         * @brief Decode a WAV file using SDL_LoadWAV.
         */
        bool loadWavFile(const std::string& path,
                         std::vector<Uint8>& outPCM,
                         SDL_AudioFormat& outFmt,
                         int& outFreq,
                         int& outChannels);

        /**
         * @brief Decode an OGG file.
         *
         * Implementation should use SDL_mixer's Mix_LoadWAV or
         * similar instead of stb_vorbis to avoid duplicate-symbol errors.
         */
        bool loadOggFile(const std::string& path,
                         std::vector<Uint8>& outPCM,
                         SDL_AudioFormat& outFmt,
                         int& outFreq,
                         int& outChannels);
    };

} // namespace retronomicon::sdl::asset
