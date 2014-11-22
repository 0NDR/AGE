#include "AudioController.h"

void AudioController::makeCurrent()
{
    alcMakeContextCurrent(AudioContext);
}
void AudioController::Suspend()
{
    alcSuspendContext(AudioContext);
}
void AudioController::Process()
{
    alcProcessContext(AudioContext);
}
void AudioController::setContext(ALCcontext *con)
{
    AudioContext = con;
}
void AudioController::setContext(ALCint* attlist)
{
    AudioContext = alcCreateContext(AudioDevice,attlist);
}
void AudioController::setDevice(ALCdevice *dev)
{
    AudioDevice = dev;
}
void AudioController::setDevice(ALCchar* devname)
{
    AudioDevice = alcOpenDevice(devname);
}
ALCdevice *AudioController::getDevice()
{
    return AudioDevice;
}
ALCcontext *AudioController::getContext()
{
    return AudioContext;
}

