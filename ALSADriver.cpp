#include <ALSADriver.h>

ALSADriver::ALSADriver()
{
    cout<<"ALSADriver()"<<endl;
}

ALSADriver::ALSADriver(int samples, int channels, int rate, int deviceType)
{
	cout<<"ALSADriver()"<<endl;
    
    this->samples    = samples;
    this->channels   = channels;
    this->rate       = rate;
    this->deviceType = deviceType;
	
    if(0 > snd_pcm_open(&sndDev, "default", (PLAYBACK == deviceType) ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE, 0)) 
    {
        printf("snd_pcm_open err \n");
    }
}

ALSADriver::~ALSADriver()
{
	cout<<"~ALSADriver()"<<endl;
	if(sndDev) snd_pcm_close(sndDev);
}

bool ALSADriver::Init()
{
    if(! sndDev)
    {
        printf("snd_pcm_open is null \n");
        return false;
    }

    printf("ALSADriver::Init() samples:%d channels:%d rate:%d \n", samples, channels, rate);

	//硬件信息和PCM流配置
	snd_pcm_hw_params_t *hw_params;
    int dir  = 0;
	unsigned int rate = this->rate;
    snd_pcm_uframes_t chunk_size = 0;

	//1. 分配snd_pcm_hw_params_t结构体
    if(0 > snd_pcm_hw_params_malloc(&hw_params))
    {
        printf("snd_pcm_hw_params_malloc err \n");
        return false;
    }

    //2. 初始化hw_params
    if(0 > snd_pcm_hw_params_any(sndDev, hw_params))
    {
        printf("snd_pcm_hw_params_any err \n");
        return false;
    }

    //3. 初始化访问权限
    if(0 > snd_pcm_hw_params_set_access(sndDev, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) 
    {
        printf("snd_pcm_hw_params_any err \n");
        return false;
    }

    //4. 设置 pcm 格式 16位 双声道
    snd_pcm_format_t snd_format;
    switch(samples)
    {
        case  8: { snd_format = SND_PCM_FORMAT_U8; } break;
        case 16: { snd_format = SND_PCM_FORMAT_S16_LE; } break;
        default:
        {
            printf("samples not support err \n");
            return false;
        }
        break;
    }
    if(0 > snd_pcm_hw_params_set_format(sndDev, hw_params, SND_PCM_FORMAT_S16_LE)) 
    {
        printf("snd_pcm_hw_params_set_format err \n");
        return false;
    }

	//5. 设置采样率
    if(0 > snd_pcm_hw_params_set_rate_near(sndDev, hw_params, &rate, &dir)) 
    {
        printf("snd_pcm_hw_params_set_rate_near err \n");
        return false;
    }

    //6. 设置通道数量
    if(0 > snd_pcm_hw_params_set_channels(sndDev, hw_params, channels)) 
    {
        printf("snd_pcm_hw_params_set_channels err \n");
        return false;
    }
    
    //7. 设置hw_params    
    if(0 > snd_pcm_hw_params(sndDev, hw_params)) 
    {
        printf("snd_pcm_hw_params err \n");
        return false;
    }
    
    //8. 设置缓存大小 注意这里不是 pcm 的大小 是 frame 
    //The configured buffer and period sizes are stored in “frames” in the runtime. 1 frame = channels * sample_size
    snd_pcm_hw_params_get_period_size(hw_params, &chunk_size, 0);
    printf("get chunk_size:%d \n", chunk_size);
    
    snd_pcm_hw_params_free(hw_params);

    if(0 > snd_pcm_prepare(sndDev)) 
    {
        printf("snd_pcm_prepare err \n");
        return false;
    }
	return true;
}

bool ALSADriver::WritePCM(uint8_t *buffer, int size)
{
    if(! sndDev)
    {
        printf("snd_pcm_open is null \n");
        return false;
    }

    //实际 pcm 大小转为 frame size = bufferSize/(2*16/8)
    int ret;
    int frame_size = size / (channels*samples/8);
    ret = snd_pcm_writei(sndDev, buffer, frame_size);
    
    if(-EPIPE == ret)
    {
        snd_pcm_prepare(sndDev);
    }
    else if(frame_size != ret)
    {
        printf("ALSADriver::WritePCM() err:%s \n", snd_strerror(ret));
        return false;
    }

    return true;
}

bool ALSADriver::ReadPCM(uint8_t *buffer, int size)
{
    if(! sndDev)
    {
        printf("snd_pcm_open is null \n");
        return false;
    }

    int ret;
    int frame_size = size / (channels*samples/8);
    ret = snd_pcm_readi(sndDev, buffer, frame_size);

    if(-EPIPE == ret)
    {
        snd_pcm_prepare(sndDev);
    }
    else if(frame_size != ret)
    {
        printf("ALSADriver::ReadPCM() err:%s \n", snd_strerror(ret));
        return false;
    }
    return true;
}

// ALSADriver.cpp
