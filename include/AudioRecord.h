#ifndef __AUDIO_RECORD__H
#define __AUDIO_RECORD__H

#include <Common.h>
#include <ALSADriver.h>

class AudioRecord
{
public:
	AudioRecord();
	~AudioRecord();
	bool Init();
	bool ReadPCM(uint8_t *buffer, int size);

private:
	ALSADriver *playerDriver;
};

#endif //__AUDIO_RECORD__H
