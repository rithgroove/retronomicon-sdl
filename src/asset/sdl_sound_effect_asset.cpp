#include "retronomicon/asset/sdl_sound_effect_asset.h"

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>

#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace retronomicon::sdl::asset {

    SDLSoundEffectAsset::SDLSoundEffectAsset(const std::string& path)
        : SDLSoundEffectAsset(path, std::filesystem::path(path).filename().string()) {}

    SDLSoundEffectAsset::SDLSoundEffectAsset(std::string path, std::string name)
        : SoundEffectAsset(std::move(path), std::move(name)) {}

    /**************************************************************************
     * Decoder Entry
     **************************************************************************/

    bool SDLSoundEffectAsset::decode(
        std::vector<Uint8>& outData,
        SDL_AudioFormat& outFmt,
        int& outFreq,
        int& outChannels)
    {
        std::string ext = std::filesystem::path(m_path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == ".wav")
            return loadWavFile(m_path, outData, outFmt, outFreq, outChannels);

        if (ext == ".ogg")
            return loadOggFile(m_path, outData, outFmt, outFreq, outChannels);

        std::cerr << "[SDLSoundEffectAsset] Unsupported audio format: " << ext << "\n";
        return false;
    }

    /**************************************************************************
     * WAV Decoder (SDL handles this beautifully)
     **************************************************************************/

    bool SDLSoundEffectAsset::loadWavFile(
        const std::string& path,
        std::vector<Uint8>& data,
        SDL_AudioFormat& fmt,
        int& freq,
        int& channels)
    {
        SDL_AudioSpec spec{};
        Uint8* wavBuffer = nullptr;
        Uint32 wavLength = 0;

        if (!SDL_LoadWAV(path.c_str(), &spec, &wavBuffer, &wavLength)) {
            std::cerr << "[SDL] Failed to load WAV: " << path << "\n";
            return false;
        }

        fmt      = spec.format;
        freq     = spec.freq;
        channels = spec.channels;

        data.resize(wavLength);
        std::memcpy(data.data(), wavBuffer, wavLength);

        SDL_FreeWAV(wavBuffer);
        return true;
    }

    /**************************************************************************
     * OGG Decoder (SDL_mixer version — NO stb_vorbis!)
     **************************************************************************/

    bool SDLSoundEffectAsset::loadOggFile(
        const std::string& path,
        std::vector<Uint8>& data,
        SDL_AudioFormat& fmt,
        int& freq,
        int& channels)
    {
        // SDL_mixer loads WAV/OGG/MP3/etc as "WAV" chunks
        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "[SDL_mixer] Failed to load OGG: " << path
                      << " (" << Mix_GetError() << ")\n";
            return false;
        }

        // SDL_mixer stores raw PCM in the chunk
        data.resize(chunk->alen);
        std::memcpy(data.data(), chunk->abuf, chunk->alen);

        // Since SDL_mixer decodes to its mixer format,
        // we assume the output format matches Mix_OpenAudio() settings.
        fmt      = MIX_DEFAULT_FORMAT;  // Usually AUDIO_S16LSB
        freq     = 44100;               // Depends on Mix_OpenAudio()
        channels = 2;                   // Stereo mixer output

        Mix_FreeChunk(chunk);
        return true;
    }

} // namespace retronomicon::sdl::asset
