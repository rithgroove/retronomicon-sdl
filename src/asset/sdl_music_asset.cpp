#include "retronomicon/asset/sdl_music_asset.h"
#include <cstring>

#include <SDL.h>
#include <SDL_audio.h>
#include <filesystem>
#include <algorithm>
#include <iostream>

#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.c"

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
        SDL_AudioSpec spec;
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
     * OGG Loader (stb_vorbis)
     **************************************************************************/

    bool SDLMusicAsset::loadOggFile(const std::string& path,
                                    std::vector<Uint8>& outPCM,
                                    SDL_AudioFormat& outFmt,
                                    int& outFreq,
                                    int& outChannels)
    {
        int channels = 0;
        int sampleRate = 0;
        short* pcmData = nullptr;

        int samples = stb_vorbis_decode_filename(path.c_str(),
                                                 &channels,
                                                 &sampleRate,
                                                 &pcmData);

        if (samples < 0) {
            std::cerr << "[SDL] Failed to decode OGG: " << path << "\n";
            return false;
        }

        outChannels = channels;
        outFreq     = sampleRate;
        outFmt      = AUDIO_S16LSB;

        size_t byteCount = samples * channels * sizeof(short);
        outPCM.resize(byteCount);

        std::memcpy(outPCM.data(), pcmData, byteCount);
        free(pcmData);

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
