#include "SoundStream.h"

void SoundStream::ReadDataTo(ALuint Buffer)
{
    const int buffsize = 8192;
    Uint8 data[buffsize];
    int bytesWritten=Mix_getDataSegment(mux, data, buffsize, &AudioSpecification);
    alBufferData( Buffer, getFormat(), data, bytesWritten, AudioSpecification.freq);
    if(getStreamPosition()>=getStreamLength())
    {
        if(loop)
        {
            setStreamPosition(0);
        }
        else
        {
            fin=true;
        }
    }

}
void SoundStream::loadFromFile(std::string filepath)
{
    file = filepath;
    mux = Mix_LoadMUS( filepath.c_str());//, &AudioSpecification, &data, &length );
    currentState = AUDIOSTATE_Loaded;
    PlayTime=0;
    PauseTime=0;
    TotalPauseTime=0;
    for(unsigned int i=0;i<2;i++)
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
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: getFileType() without load for "<<getName()<<std::endl;
        return MUS_NONE;
    }
    return Mix_GetMusicType(mux);
}

void SoundStream::stopStream()
{
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: stopStream() without load"<<getName()<<std::endl;
        return;
    }
    currentState = AUDIOSTATE_Paused|AUDIOSTATE_Loaded;
    PauseTime = 0;
    PlayTime = 0;
    TotalPauseTime =0;
    //resetBuffers();
    Mix_StopStream(mux);
}

void SoundStream::pauseStream()
{
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: pauseStream() without load"<<getName()<<std::endl;
        return;
    }
    currentState = AUDIOSTATE_Paused|AUDIOSTATE_Loaded;
    PauseTime = SDL_GetTicks();
    Mix_StopStream(mux);
}
void SoundStream::startStream(bool looper)
{
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: startStream() without load for "<<getName()<<std::endl;
        return;
    }
    currentState = AUDIOSTATE_Played|AUDIOSTATE_Loaded;
    PlayTime = SDL_GetTicks();
    if(PauseTime>0)
    {
        TotalPauseTime += (PlayTime-PauseTime);
    }
    loop=looper;
    fin=false;
    Mix_StartStream(mux);
}
void SoundStream::setStreamPosition(double pos)
{
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: setStreamPosition() without load for "<<getName()<<std::endl;
        return;
    }
    PauseTime = 0;
    TotalPauseTime=0;
    PlayTime = SDL_GetTicks()-pos*1000;
    Mix_SetMusicPosition(mux,pos);
}
double SoundStream::getStreamLength()
{
     if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: getStreamLength() without load for "<<getName()<<std::endl;
        return 0;
    }
    return Mix_getMusicTime(mux);
}
double SoundStream::getStreamPosition()
{
    if(!(currentState&AUDIOSTATE_Loaded))
    {
        std::cout<<"ERROR: getStreamPosition() without load for "<<getName()<<std::endl;
        return 0;
    }
    double time = (double)(SDL_GetTicks()-(PlayTime+TotalPauseTime))/1000;
    if(time>getStreamLength())
        time=getStreamLength();
    return time;
}
Uint32 SoundStream::getState()
{
    return currentState;
}
bool SoundStream::isFinished()
{
    return fin;
}
