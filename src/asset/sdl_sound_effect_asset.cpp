#include "retronomicon/asset/sdl_sound_effect_asset.h"
#include <SDL.h>
#include <SDL_audio.h>

#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <filesystem>

#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.c"

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

        bool ok = false;

        if (ext == ".wav")
            ok = loadWavFile(m_path, outData, outFmt, outFreq, outChannels);
        else if (ext == ".ogg")
            ok = loadOggFile(m_path, outData, outFmt, outFreq, outChannels);
        else {
            std::cerr << "[SDLSoundEffectAsset] Unsupported audio format: " << ext << "\n";
            return false;
        }

        return ok;
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
        SDL_AudioSpec spec;
        Uint8* wavBuffer = nullptr;
        Uint32 wavLength = 0;

        if (!SDL_LoadWAV(path.c_str(), &spec, &wavBuffer, &wavLength)) {
            std::cerr << "[SDL] Failed to load WAV: " << path << "\n";
            return false;
        }

        fmt = spec.format;       // AUDIO_S16LSB, etc.
        freq = spec.freq;
        channels = spec.channels;

        data.resize(wavLength);
        std::memcpy(data.data(), wavBuffer, wavLength);

        SDL_FreeWAV(wavBuffer);
        return true;
    }

    /**************************************************************************
     * OGG Decoder (stb_vorbis)
     **************************************************************************/

    bool SDLSoundEffectAsset::loadOggFile(
        const std::string& path,
        std::vector<Uint8>& data,
        SDL_AudioFormat& fmt,
        int& freq,
        int& channels)
    {
        int c = 0;
        int sr = 0;
        short* output = nullptr;

        int samples = stb_vorbis_decode_filename(path.c_str(), &c, &sr, &output);
        if (samples < 0)
            return false;

        channels = c;
        freq = sr;

        fmt = AUDIO_S16LSB; // stb_vorbis gives 16-bit signed little-endian_pcm

        size_t size = samples * channels * sizeof(short);
        data.resize(size);
        std::memcpy(data.data(), output, size);

        free(output);
        return true;
    }

} // namespace retronomicon::sdl::asset
