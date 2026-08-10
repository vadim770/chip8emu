#include <SDL3/SDL.h>

class Sound{
    public:
        bool initialize();
        ~Sound();
        void play();
        void stop();


    private:
        SDL_AudioSpec spec{
            .format = SDL_AUDIO_F32LE,
            .channels = 1,
            .freq = 44100
        };

        SDL_AudioStream* stream{nullptr};
        static constexpr uint32_t BUFFER_SIZE{512};
        float audioBuffer[BUFFER_SIZE]{0};
};