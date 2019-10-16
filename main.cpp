#include <stdio.h>
#include <AudioPlayer.h>

int main(int argc, char **argv)
{
	if(2 != argc)
	{
		printf("usage:%s <media file> \n", argv[0]);
		return -1;
	}
	
	AudioPlayer *audioPlayer = new AudioPlayer();
	audioPlayer->Init();
	audioPlayer->SetUri(string(argv[1]));
	audioPlayer->Play();

	delete audioPlayer;
}

// main.cpp
