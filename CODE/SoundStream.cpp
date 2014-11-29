#include "SoundStream.h"

void SoundStream::ReadDataTo(ALuint Buffer)
{
    const int buffsize = 8192;
    Uint8 data[buffsize];
    int bytesWritten=Mix_getDataSegment(mux, data, buffsize, &AudioSpecification);
    bytesRead+=bytesWritten;
    alBufferData( Buffer, getFormat(), data, bytesWritten, AudioSpecification.freq);

}
void SoundStream::loadFromFile(std::string filepath)
{
    file = filepath;
    mux = Mix_LoadMUS( filepath.c_str());//, &AudioSpecification, &data, &length );

    for(int i=0;i<2;i++)
    {
        ReadDataTo(getBuffers()[i]);
    }
}
ALenum SoundStream::getFormat()
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
int SoundStream::getNumBuffers()
{
    return sizeof(getBuffers())/sizeof(ALuint);
}
ALuint *SoundStream::getBuffers()
{
    if(alIsBuffer(buffers[0])==AL_FALSE && alIsBuffer(buffers[1])==AL_FALSE)
    {
        alGenBuffers(2,buffers);
    }
    return buffers;
}

Mix_MusicType SoundStream::getFileType()
{
    return Mix_GetMusicType(mux);
}

void SoundStream::stopStream()
{
    Mix_StopStream(mux);
    bytesRead = 0;
}
void SoundStream::pauseStream()
{
    double pos = getStreamPosition();
    Mix_StopStream(mux);
    Mix_SetMusicPosition(mux, pos);
}
void SoundStream::startStream()
{
    Mix_StartStream(mux);
}
void SoundStream::setStreamPosition(double pos)
{
    Mix_SetMusicPosition(mux,pos);
    bytesRead=pos*AudioSpecification.freq;
}
double SoundStream::getStreamPosition()
{
    double position = (long double)bytesRead/((double)AudioSpecification.freq);
    return position;
}
