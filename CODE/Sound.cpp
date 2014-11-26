#include "Sound.h"

#define RIFF        0x46464952      /* "RIFF" */
#define WAVE        0x45564157      /* "WAVE" */
#define FORM        0x4d524f46      /* "FORM" */
#define OGGS        0x5367674f      /* "OggS" */
#define CREA        0x61657243      /* "Crea" */
#define FLAC        0x43614C66      /* "fLaC" */

void Sound::loadFromFile(std::string filepath)
{
    Uint32 magic;
    SDL_AudioSpec *loaded;
    int samplesize;
    SDL_RWops *src = SDL_RWFromFile(filepath.c_str(), "rb");
    int freesrc = 1;
    /* rcg06012001 Make sure src is valid */
    if ( ! src ) {
        SDL_SetError("Mix_LoadWAV_RW with NULL src");
    }



    /* Allocate the chunk memory */
    chunk = (Mix_Chunk *)SDL_malloc(sizeof(Mix_Chunk));
    if ( chunk == NULL ) {
        SDL_SetError("Out of memory");
        if ( freesrc ) {
            SDL_RWclose(src);
        }
    }

    /* Find out what kind of audio file this is */
    magic = SDL_ReadLE32(src);
    /* Seek backwards for compatibility with older loaders */
    SDL_RWseek(src, -(int)sizeof(Uint32), RW_SEEK_CUR);

    switch (magic) {
        case WAVE:
        case RIFF:
            loaded = SDL_LoadWAV_RW(src, freesrc, &AudioSpecification,
                    (Uint8 **)&chunk->abuf, &chunk->alen);
            break;
        case FORM:
            /*loaded = Mix_LoadAIFF_RW(src, freesrc, &AudioSpecification,
                    (Uint8 **)&chunk->abuf, &chunk->alen);*/
            break;
    #ifdef OGG_MUSIC
        case OGGS:
            loaded = Mix_LoadOGG_RW(src, freesrc, &AudioSpecification,
                    (Uint8 **)&chunk->abuf, &chunk->alen);
            break;
    #endif
    #ifdef FLAC_MUSIC
        case FLAC:
            loaded = Mix_LoadFLAC_RW(src, freesrc, &AudioSpecification,
                    (Uint8 **)&chunk->abuf, &chunk->alen);
            break;
    #endif
        case CREA:
           /* loaded = Mix_LoadVOC_RW(src, freesrc, &AudioSpecification,
                    (Uint8 **)&chunk->abuf, &chunk->alen);*/
            break;
        default:
            SDL_SetError("Unrecognized sound file type");
            if ( freesrc ) {
                SDL_RWclose(src);
            }
            loaded = NULL;
            break;
    }
    if ( !loaded ) {
        /* The individual loaders have closed src if needed */
        SDL_free(chunk);
    }
    alDeleteBuffers(1,&buffer);
    alBufferData( getBuffer(), getFormat(), chunk->abuf, chunk->alen, AudioSpecification.freq);
}
ALenum Sound::getFormat()
{
    if(Format != 0)
    {
        return Format;
    }
    if(AudioSpecification.channels == 1)
    {
        if(SDL_AUDIO_BITSIZE(AudioSpecification.format) == 8)
            Format = AL_FORMAT_MONO8;
        else
            Format = AL_FORMAT_MONO16;
    }
    else
    {
        if(SDL_AUDIO_BITSIZE(AudioSpecification.format) == 8)
            Format = AL_FORMAT_STEREO8;
        else
            Format = AL_FORMAT_STEREO16;
    }
    return Format;
}
ALuint Sound::getBuffer()
{
    if(alIsBuffer(buffer)==AL_FALSE)
        alGenBuffers(1,&buffer);
    return buffer;
}

int Sound::getFrequency()
{
    int freq;
    alGetBufferi(getBuffer(),AL_FREQUENCY,&freq);
    return freq;
}
int Sound::getDepthBits()
{
    int db;
    alGetBufferi(getBuffer(),AL_BITS,&db);
    return db;
}
int Sound::getChannels()
{
    int chan;
    alGetBufferi(getBuffer(),AL_CHANNELS,&chan);
    return chan;
}
int Sound::getSize()
{
    int ssize;
    alGetBufferi(getBuffer(),AL_SIZE,&ssize);
    return ssize;
}

std::string Sound::getFilePath()
{
    return file;
}

