#ifndef SOUND_PROXY_H
#define SOUND_PROXY_H

#include <math.h>


#define M_PI 3.1459
#define LOW_QUALITY_SOUND 22050
#define HIGH_QUALITY_SOUND 44100
#define GEN_SAMP_RATE HIGH_QUALITY_SOUND

#define BUF_SIZE (GEN_SAMP_RATE*2)	/* 2 second buffer */
#define BIG_SOUND_BUFFER 4096
#define MED_SOUND_BUFFER 2048 
#define SMALL_SOUND_BUFFER 1024 

#define SUBCHUNK1SIZE 16
#define PCM_FORMAT 1


class SoundUtils
{

private:
	int volume,pan,speed;
	int buffer[BUF_SIZE];
	int sizeOfWav;
	//SAMPLE *snd;


	//unsigned char* create_wav(unsigned long num_samples, /*short*/ int * data, int s_rate,int & sizeOfWav);
public:
	SoundUtils(const char * path);
	~SoundUtils();

	void PlaySound();
	unsigned char *doSoundCreateTest();

};

#endif //SOUND_PROXY_H