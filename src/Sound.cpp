#include "../include/Sound.hpp"

bool Sound::initialize(){
    stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
        &spec, 
        nullptr,
        nullptr
    );

    if(!stream){
        SDL_Log("Failed to create audio stream. SDL error: %s\n", SDL_GetError());
        return false;
    }

    const float FREQUENCY = 440.0f;
    const float SAMPLE_RATE = 44100.0f;
    const float VOLUME = 0.15f;

    float phase = 0.0f;
    float phaseIncrement = FREQUENCY / SAMPLE_RATE;

    for (uint32_t i = 0; i < BUFFER_SIZE; ++i) {
        audioBuffer[i] = (phase < 0.5f) ? VOLUME : -VOLUME;
        phase += phaseIncrement;
        if (phase >= 1.0f) phase -= 1.0f;
    }

    SDL_ResumeAudioStreamDevice(stream);
    return true;

}

void Sound::play(){
    if (MUTE) {
        SDL_ClearAudioStream(stream); 
        return;
    }

    if (SDL_GetAudioStreamQueued(stream) < static_cast<int>(BUFFER_SIZE * sizeof(float))) {
        SDL_PutAudioStreamData(stream, audioBuffer, sizeof(audioBuffer));
    }
}

void Sound::stop(){
    SDL_ClearAudioStream(stream);
}

void Sound::mute(){
    MUTE = !MUTE;
}

Sound::~Sound(){
    if(stream){
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }
}