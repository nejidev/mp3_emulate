#ifndef __AUDIO_PLAYER_ALSA_DRIVER__H
#define __AUDIO_PLAYER_ALSA_DRIVER__H
#include <alsa/asoundlib.h>
#include <Common.h>

enum ALSADevice
{
	PLAYBACK = 1,
	CAPTURE  = 2,
};

class ALSADriver
{
public:
	ALSADriver();
	ALSADriver(int samples, int channels, int rate, int deviceType);
	~ALSADriver();
	bool Init();
	bool WritePCM(uint8_t *buffer, int size);
	bool ReadPCM(uint8_t *buffer, int size);

private:
	int rate;
	int samples;
	int channels;
	int deviceType;
	snd_pcm_t *sndDev;
};

#endif // __AUDIO_PLAYER_ALSA_DRIVER__H
