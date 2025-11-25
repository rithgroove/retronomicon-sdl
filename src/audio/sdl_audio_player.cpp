#include "retronomicon/audio/sdl_audio_player.h"
#include "retronomicon/math/math_utils.h"

#include <iostream>

namespace retronomicon::sdl::audio {

using retronomicon::math::clamp;
using retronomicon::audio::Vec3;

/**************************************************************************
 * Constructor / Destructor
 **************************************************************************/

SDLAudioPlayer::SDLAudioPlayer() = default;

SDLAudioPlayer::~SDLAudioPlayer() {
    shutdown();
}

/**************************************************************************
 * Initialization
 **************************************************************************/

bool SDLAudioPlayer::init() {

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "[SDL_mixer] SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int initted = Mix_Init(flags);

    if ((initted & flags) != flags) {
        std::cerr << "[SDL_mixer] Mix_Init missing loaders: " << Mix_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "[SDL_mixer] Mix_OpenAudio failed: " << Mix_GetError() << "\n";
        return false;
    }

    Mix_AllocateChannels(64);

    std::cout << "[SDL_mixer] Audio initialized\n";
    return true;
}

/**************************************************************************
 * Shutdown
 **************************************************************************/

void SDLAudioPlayer::shutdown() {
    clearSoundCache();

    if (m_music) {
        Mix_FreeMusic(m_music);
        m_music = nullptr;
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    std::cout << "[SDL_mixer] Shutdown complete\n";
}

/**************************************************************************
 * Volume
 **************************************************************************/

void SDLAudioPlayer::setMasterVolume(float volume) {
    m_masterVolume = clamp(volume, 0.0f, 1.0f);
    applyGlobalVolume();
}

void SDLAudioPlayer::applyGlobalVolume() {
    int volMusic = static_cast<int>(m_masterVolume * m_musicVolume * MIX_MAX_VOLUME);
    int volSfx   = static_cast<int>(m_masterVolume * m_sfxVolume * MIX_MAX_VOLUME);

    Mix_VolumeMusic(volMusic);

    for (auto& [_, chunk] : m_sfxCache) {
        chunk->volume = volSfx;
    }
}

/**************************************************************************
 * Music
 **************************************************************************/

void SDLAudioPlayer::playMusic(const std::string& path, bool loop, int fadeInMs) {
    if (m_musicPath == path && m_music != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(m_music);
    }

    m_musicPath = path;
    m_music = Mix_LoadMUS(path.c_str());

    if (!m_music) {
        std::cerr << "[SDL_mixer] Failed to load music: " << path << "\n";
        return;
    }

    int loops = loop ? -1 : 1;

    if (fadeInMs > 0)
        Mix_FadeInMusic(m_music, loops, fadeInMs);
    else
        Mix_PlayMusic(m_music, loops);

    setMusicVolume(m_musicVolume);
}

void SDLAudioPlayer::stopMusic(int fadeOutMs) {
    if (fadeOutMs > 0)
        Mix_FadeOutMusic(fadeOutMs);
    else
        Mix_HaltMusic();
}

void SDLAudioPlayer::setMusicPaused(bool paused) {
    if (paused) Mix_PauseMusic();
    else        Mix_ResumeMusic();
}

bool SDLAudioPlayer::isMusicPlaying() const {
    return Mix_PlayingMusic() == 1;
}

void SDLAudioPlayer::setMusicVolume(float volume) {
    m_musicVolume = clamp(volume, 0.0f, 1.0f);
    int vol = static_cast<int>(m_masterVolume * m_musicVolume * MIX_MAX_VOLUME);
    Mix_VolumeMusic(vol);
}

/**************************************************************************
 * Sound Effects
 **************************************************************************/

bool SDLAudioPlayer::loadSoundEffect(const std::string& name, const std::string& path) {

    if (m_sfxCache.find(name) != m_sfxCache.end())
        return true;

    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "[SDL_mixer] Failed to load SFX: " << path << "\n";
        return false;
    }

    chunk->volume = static_cast<int>(m_masterVolume * m_sfxVolume * MIX_MAX_VOLUME);
    m_sfxCache[name] = chunk;
    return true;
}

void SDLAudioPlayer::playSoundEffect(const std::string& name, float volume, bool loop) {
    auto it = m_sfxCache.find(name);
    if (it == m_sfxCache.end()) {
        std::cerr << "[SDL_mixer] Unknown SFX: " << name << "\n";
        return;
    }

    Mix_Chunk* chunk = it->second;

    int ch = Mix_PlayChannel(-1, chunk, loop ? -1 : 0);
    if (ch < 0) {
        std::cerr << "[SDL_mixer] Failed to play SFX: " << name << "\n";
        return;
    }

    int vol = static_cast<int>(m_masterVolume * m_sfxVolume * volume * MIX_MAX_VOLUME);
    Mix_Volume(ch, vol);
}

void SDLAudioPlayer::stopSoundEffect(const std::string& name) {
    if (name.empty()) {
        Mix_HaltChannel(-1);
        return;
    }

    auto it = m_sfxCache.find(name);
    if (it == m_sfxCache.end())
        return;

    // SDL_mixer cannot stop a specific chunk without a channel lookup.
    // So we simply stop all.
    Mix_HaltChannel(-1);
}

void SDLAudioPlayer::setSfxVolume(float volume) {
    m_sfxVolume = clamp(volume, 0.0f, 1.0f);
    applyGlobalVolume();
}

void SDLAudioPlayer::unloadSoundEffect(const std::string& name) {
    auto it = m_sfxCache.find(name);
    if (it == m_sfxCache.end())
        return;

    Mix_FreeChunk(it->second);
    m_sfxCache.erase(it);
}

void SDLAudioPlayer::clearSoundCache() {
    for (auto& [_, chunk] : m_sfxCache)
        Mix_FreeChunk(chunk);

    m_sfxCache.clear();
}

} // namespace retronomicon::sdl::audio
