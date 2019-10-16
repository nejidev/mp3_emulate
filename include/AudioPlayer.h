#ifndef __AUDIO_PLAYER__H
#define __AUDIO_PLAYER__H

#include <Common.h>
#include <ALSADriver.h>

#define __STDC_CONSTANT_MACROS

#ifdef __cplusplus
extern "C" 
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/samplefmt.h>
#ifdef __cplusplus
}
#endif

#define MAX_AUDIO_FRAME_SIZE (48000*32/4) // 1 second of 48Khz 32bit audio size


class AudioPlayer{
public:
	AudioPlayer();
	~AudioPlayer();
	void SetUri(string uri);
	bool Init();
	bool Play();
	bool WritePCM(uint8_t *buffer, int size);
	
private:
	bool FFmpegInit();
	
	ALSADriver        *playerDriver;
	string            uri;
	AVFormatContext	  *formatCtx;
	AVCodecContext	  *codecCtx;
	AVCodec			  *codec;
	AVPacket		  *packet;
	uint8_t			  *outBuffer;
	AVFrame			  *frame;
	int64_t           in_channel_layout;
	struct SwrContext *convertCtx;
	int               audioStream;
	uint64_t          out_channel_layout;
	int               out_nb_samples;
	AVSampleFormat    out_sample_fmt;
	int               out_sample_rate;
	int               out_channels;
	int               out_buffer_size;

};

#endif //__AUDIO_PLAYER__H
