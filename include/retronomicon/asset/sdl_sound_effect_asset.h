#pragma once

#include "retronomicon/asset/sound_effect_asset.h"
#include <SDL2/SDL_audio.h>

#include <string>
#include <vector>

namespace retronomicon::sdl::asset {

    /**
     * @brief SDL decoder-only sound effect asset.
     *
     * This class converts WAV/OGG files into raw PCM memory.
     * The SDL audio backend (SDL_mixer or raw SDL audio) is responsible
     * for uploading and playing the PCM data.
     *
     * IMPORTANT:
     *   The header intentionally contains no stb_vorbis references,
     *   because SDL_mixer embeds its own stb version internally.
     */
    class SDLSoundEffectAsset final : public retronomicon::asset::SoundEffectAsset {
    public:
        SDLSoundEffectAsset(const std::string& path);
        SDLSoundEffectAsset(std::string path, std::string name);

        ~SDLSoundEffectAsset() override = default;

        /**
         * @brief Decode WAV or OGG audio data into PCM memory.
         *
         * @param outData        Raw PCM bytes.
         * @param outFmt         SDL_AudioFormat (AUDIO_S16LSB, AUDIO_U8, etc.)
         * @param outFreq        Sample rate (Hz)
         * @param outChannels    1 = mono, 2 = stereo
         */
        bool decode(std::vector<Uint8>& outData,
                    SDL_AudioFormat& outFmt,
                    int& outFreq,
                    int& outChannels);

        // ---------------------------------------------------------------------
        // Dummy overrides — backend (SDL) owns playback, not the asset itself.
        // ---------------------------------------------------------------------
        bool load() override { return true; }
        void unload() override {}
        void play(bool /*loop*/ = false) override {}
        void stop() override {}

    private:
        /**
         * @brief Decode WAV files via SDL_LoadWAV.
         */
        bool loadWavFile(const std::string& path,
                         std::vector<Uint8>& data,
                         SDL_AudioFormat& fmt,
                         int& freq,
                         int& channels);

        /**
         * @brief Decode OGG files.
         *
         * Implementation options:
         *  - Use stb_vorbis in the .cpp only (NO symbol conflicts allowed).
         *  - OR use SDL_mixer's Mix_LoadWAV if you want to avoid stb entirely.
         *
         * Header remains neutral.
         */
        bool loadOggFile(const std::string& path,
                         std::vector<Uint8>& data,
                         SDL_AudioFormat& fmt,
                         int& freq,
                         int& channels);
    };

} // namespace retronomicon::sdl::asset
