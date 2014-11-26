/*#include "Sound.h"
void hookMusicData(void* udata,Uint8 *stream, int len)
{
    alBufferData(getBuffer(), getFormat(), stream, len, AudioSpecification.freq );
}

void SoundStream::loadFromFile(std::string filepath)
{
    file = filepath;
    mus = Mix_LoadMUS( filepath.c_str());//, &AudioSpecification, &data, &length );
    alDeleteBuffers(1,&buffer);
    Mix_HookMusic(hookMusicData);
}

std::string SoundStream::getFileType()
{

}

*/
