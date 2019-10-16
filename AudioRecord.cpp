#include <AudioRecord.h>

AudioRecord::AudioRecord()
{
    cout<<"AudioRecord()"<<endl;
    playerDriver = new ALSADriver(AUDIO_SAMPLES, AUDIO_CHANNELS, AUDIO_RATE, CAPTURE);
}

AudioRecord::~AudioRecord()
{
	cout<<"~AudioRecord()"<<endl;
	if(playerDriver) delete playerDriver;
}

bool AudioRecord::Init()
{
    return playerDriver->Init();
}

bool AudioRecord::ReadPCM(uint8_t *buffer, int size)
{
	return playerDriver->ReadPCM(buffer, size);
}

// AudioRecord.cpp
