#include "retronomicon/asset/sdl_music_asset.h"

#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>

#include <cstring>
#include <filesystem>
#include <algorithm>
#include <iostream>

namespace retronomicon::sdl::asset {

    SDLMusicAsset::SDLMusicAsset(const std::string& path)
        : SDLMusicAsset(path, std::filesystem::path(path).filename().string()) {}

    SDLMusicAsset::SDLMusicAsset(const std::string& path, const std::string& name)
        : MusicAsset(path, name) {}


    /**************************************************************************
     * Decode Entry
     **************************************************************************/

    bool SDLMusicAsset::decode(
        std::vector<Uint8>& outPCM,
        SDL_AudioFormat& outFmt,
        int& outFreq,
        int& outChannels)
    {
        std::string ext = std::filesystem::path(m_path).extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        if (ext == ".wav")
            return loadWavFile(m_path, outPCM, outFmt, outFreq, outChannels);

        if (ext == ".ogg")
            return loadOggFile(m_path, outPCM, outFmt, outFreq, outChannels);

        std::cerr << "[SDLMusicAsset] Unsupported music format: " << ext << "\n";
        return false;
    }


    /**************************************************************************
     * WAV Loader (SDL Does This Best)
     **************************************************************************/

    bool SDLMusicAsset::loadWavFile(const std::string& path,
                                    std::vector<Uint8>& outPCM,
                                    SDL_AudioFormat& outFmt,
                                    int& outFreq,
                                    int& outChannels)
    {
        SDL_AudioSpec spec{};
        Uint8* buffer = nullptr;
        Uint32 length = 0;

        if (!SDL_LoadWAV(path.c_str(), &spec, &buffer, &length)) {
            std::cerr << "[SDL] Failed to load WAV: " << path << "\n";
            return false;
        }

        outFmt      = spec.format;
        outFreq     = spec.freq;
        outChannels = spec.channels;

        outPCM.resize(length);
        std::memcpy(outPCM.data(), buffer, length);

        SDL_FreeWAV(buffer);
        return true;
    }


    /**************************************************************************
     * OGG Loader (SDL_mixer — No stb_vorbis!)
     **************************************************************************/

    bool SDLMusicAsset::loadOggFile(const std::string& path,
                                    std::vector<Uint8>& outPCM,
                                    SDL_AudioFormat& outFmt,
                                    int& outFreq,
                                    int& outChannels)
    {
        // SDL_mixer treats any supported format (WAV, OGG, MP3, etc.)
        // as a "WAV chunk" after decoding.
        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "[SDL_mixer] Failed to decode OGG: " << path
                      << " (" << Mix_GetError() << ")\n";
            return false;
        }

        // Copy decoded PCM data
        outPCM.resize(chunk->alen);
        std::memcpy(outPCM.data(), chunk->abuf, chunk->alen);

        // SDL_mixer decodes into the mixer output format.
        // Use the settings defined by Mix_OpenAudio().
        outFmt      = MIX_DEFAULT_FORMAT; // Usually AUDIO_S16LSB
        outFreq     = 44100;
        outChannels = 2;

        Mix_FreeChunk(chunk);
        return true;
    }


    /**************************************************************************
     * to_string
     **************************************************************************/

    std::string SDLMusicAsset::to_string() const {
        return "[SDLMusicAsset]\n-name=" + m_name +
               "\n-path=" + m_path + "\n";
    }

} // namespace retronomicon::sdl::asset
