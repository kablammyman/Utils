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
	unsigned char* write_to_buffer_little_endian(unsigned int word, int num_bytes);
	void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file);
	void read_little_endian(unsigned char *buffer, int num_bytes, FILE *wav_file);

public:
	SoundUtils(const char * path);
	~SoundUtils();

	void LoadWav(char * filename);
	void PlaySound();
	void AppendToWave(unsigned char *dest, unsigned char *src, int numBytes, int & offset);
	unsigned char *CreateWav(unsigned long num_samples, /*short*/ int * data, int s_rate, int & sizeOfWav);
	void WriteWav(char * filename, unsigned long num_samples, /*short*/ int * data, int s_rate);
	unsigned char *DoSoundCreateTest();

};

#endif //SOUND_PROXY_H