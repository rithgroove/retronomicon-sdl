#pragma once

#include "retronomicon/audio/i_audio_player.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace retronomicon::sdl::audio {

    class SDLAudioPlayer : public retronomicon::audio::IAudioPlayer {
    public:
        SDLAudioPlayer();
        ~SDLAudioPlayer() override;

        /***************************** Initialization *****************************/
        bool init() override;
        void shutdown() override;

        /***************************** Global *****************************/
        void update() override {}
        void setMasterVolume(float volume) override;
        float getMasterVolume() const override { return m_masterVolume; }

        /***************************** Music *****************************/
        void playMusic(const std::string& path, bool loop = true, int fadeInMs = 0) override;
        void stopMusic(int fadeOutMs = 0) override;
        void setMusicPaused(bool paused) override;
        bool isMusicPlaying() const override;
        void setMusicVolume(float volume) override;
        float getMusicVolume() const override { return m_musicVolume; }

        /***************************** Sound Effects *****************************/
        bool loadSoundEffect(const std::string& name, const std::string& path) override;
        void playSoundEffect(const std::string& name,
                             float volume = 1.0f,
                             bool loop = false) override;
        void stopSoundEffect(const std::string& name = "") override;
        void setSfxVolume(float volume) override;
        float getSfxVolume() const override { return m_sfxVolume; }
        void unloadSoundEffect(const std::string& name) override;
        void clearSoundCache() override;

        /***************************** 3D Audio (not supported) *****************************/
        void setListenerPosition(const retronomicon::audio::Vec3&) override {}
        void playSoundEffect3D(const std::string&,
                               const retronomicon::audio::Vec3&,
                               float,
                               bool) override {}

    private:
        /***************************** Mixer State *****************************/
        float m_masterVolume = 1.0f;
        float m_musicVolume  = 1.0f;
        float m_sfxVolume    = 1.0f;

        /***************************** Music *****************************/
        Mix_Music* m_music = nullptr;
        std::string m_musicPath;

        /***************************** SFX Cache *****************************/
        std::unordered_map<std::string, Mix_Chunk*> m_sfxCache;

        /***************************** Internal Helpers *****************************/
        void applyGlobalVolume();
    };

} // namespace retronomicon::sdl::audio
