#include "Speaker.h"


bool Speaker::isLooping()
{
    int val;
    alGetSourcei(getSource(),AL_LOOPING,&val);
    return val==AL_TRUE;
}

glm::vec3 Speaker::getVelocity()
{
    float x,y,z;
    alGetSource3f(getSource(),AL_VELOCITY,&x,&y,&z);
    return glm::vec3(x,y,z);
}
float Speaker::getRolloffFactor()
{
    float fac;
    alGetSourcef(getSource(),AL_ROLLOFF_FACTOR,&fac);
    return fac;
}
float Speaker::getPitch()
{
    float pitch;
    alGetSourcef(getSource(),AL_PITCH,&pitch);
    return pitch;
}
float Speaker::getGain()
{
    float gain;
    alGetSourcef(getSource(),AL_GAIN,&gain);
    return gain;
}

ALuint Speaker::getSource()
{
    if(alIsSource(source)==AL_FALSE)
        alGenSources(1,&source);
    return source;
}
Sound* Speaker::getBuffer(){return buffer;}




void Speaker::setVelocity(glm::vec3 vel)
{
    alSource3f(getSource(),AL_VELOCITY,vel.x,vel.y,vel.z);
}
void Speaker::setRolloffFactor(float fac)
{
    alSourcef(getSource(),AL_ROLLOFF_FACTOR,fac);
}
void Speaker::setPitch(float pitch)
{
    if(pitch<0){std::cout<<"gain cannot be negative for "<<Name<<std::endl; return;}
    alSourcef(getSource(),AL_PITCH,pitch);
}
void Speaker::setGain(float gain)
{
    if(gain<0){std::cout<<"gain cannot be negative for "<<Name<<std::endl; return;}
    alSourcef(getSource(),AL_GAIN,gain);
}
void Speaker::setBuffer(Sound* snd)
{
    buffer = snd;
    alSourcei(getSource(),AL_BUFFER,buffer->getBuffer());
}



void Speaker::Play(bool loop)
{
    alSourcei(getSource(),AL_LOOPING,loop?AL_TRUE:AL_FALSE);
    alSourcePlay(getSource());
}
void Speaker::Pause()
{
    alSourcePause(getSource());
}
void Speaker::Stop()
{
    alSourceStop(getSource());
}
void Speaker::Rewind()
{
    alSourceRewind(getSource());
}
void Speaker::deleteSource()
{
    alDeleteSources(1,&source);
}


void Speaker::Update()
{
    glm::mat4 AbsoluteMatrix = getAbsoluteMatrix();
    glm::vec3 pos = glm::vec3(-AbsoluteMatrix[3][0],AbsoluteMatrix[3][1],-AbsoluteMatrix[3][2]);
    alSource3f(getSource(),AL_POSITION,pos.x,pos.y,pos.z);

}
