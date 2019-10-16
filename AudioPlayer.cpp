#include <AudioPlayer.h>

AudioPlayer::AudioPlayer()
{
	cout<<"AudioPlayer()"<<endl;
	playerDriver = new ALSADriver(AUDIO_SAMPLES, AUDIO_CHANNELS, AUDIO_RATE, PLAYBACK);
}

AudioPlayer::~AudioPlayer()
{
	cout<<"~AudioPlayer()"<<endl;
	swr_free(&convertCtx);
	av_free(outBuffer);
	avcodec_close(codecCtx);
	avformat_close_input(&formatCtx);

	if(playerDriver) delete playerDriver;
}

bool AudioPlayer::Init()
{
	return playerDriver->Init();
}

void AudioPlayer::SetUri(string uri)
{
	this->uri = uri;
}

bool AudioPlayer::Play()
{
	int ret;
	int got_frame;

	if(! uri.empty())
	{
		if(FFmpegInit())
		{
			while(0 <= av_read_frame(formatCtx, packet))
			{
				if(packet->stream_index == audioStream)
				{
					ret = avcodec_decode_audio4(codecCtx, frame, &got_frame, packet);
					if(0 > ret)
					{
						printf("avcodec decode audio4 error \n");
						return false;
					}
					if(0 < got_frame)
					{
						//转码
						swr_convert(convertCtx, &outBuffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
					}
					WritePCM(outBuffer, out_buffer_size);	
				}
				av_free_packet(packet);
			}
		}
	}
	return true;
}

bool AudioPlayer::FFmpegInit()
{
	av_register_all();
	avformat_network_init();

	formatCtx = avformat_alloc_context();
	if(0 != avformat_open_input(&formatCtx, uri.c_str(), NULL, NULL))
	{
		printf("avformat open input error \n");
		return false;
	}
	
	//查找 stream info
	if(0 > avformat_find_stream_info(formatCtx, NULL))
	{
		printf("avformat find stream info error \n");
		return -1;
	}

	//打印媒体信息
	av_dump_format(formatCtx, 0, uri.c_str(), false);

	//查找音频 stream
	audioStream = -1;
	int i;
	for(i=0; i<formatCtx->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_AUDIO == formatCtx->streams[i]->codec->codec_type)
		{
			audioStream = i;
			break;
		}
	}
	if(-1 == audioStream)
	{
		printf("find audio stream error \n");
		return false;
	}

	//set codec
	codecCtx = formatCtx->streams[audioStream]->codec;
	codec = avcodec_find_decoder(codecCtx->codec_id);
	if(! codec)
	{
		printf("find codec error \n");
		return -1;
	}
	if(0 > avcodec_open2(codecCtx, codec, NULL))
	{
		printf("open codec error \n");
		return -1;
	}

	packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	av_init_packet(packet);

	outBuffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	frame = av_frame_alloc();

	out_channel_layout = AV_CH_LAYOUT_STEREO;
	out_nb_samples     = codecCtx->frame_size;
	out_sample_fmt     = AV_SAMPLE_FMT_S16;
	out_sample_rate    = 44100;
	out_channels       = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
	out_buffer_size    = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);

	in_channel_layout = av_get_default_channel_layout(codecCtx->channels);

	//swr 设置转换
	convertCtx = swr_alloc();
	convertCtx = swr_alloc_set_opts(convertCtx, out_channel_layout, out_sample_fmt, out_sample_rate, in_channel_layout, codecCtx->sample_fmt, codecCtx->sample_rate, 0, NULL);
	swr_init(convertCtx);

	return true;
}

bool AudioPlayer::WritePCM(uint8_t *buffer, int size)
{
	return playerDriver->WritePCM(buffer, size);
}