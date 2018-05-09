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


	//SAMPLE *snd; 
	unsigned char* write_to_buffer_little_endian(unsigned int word, int num_bytes);
	void write_little_endian(unsigned int word, int num_bytes, FILE *wav_file);
	void read_little_endian(unsigned char *buffer, int num_bytes, FILE *wav_file);

public:
	int volume, pan, speed;
	unsigned int sample_rate;
	unsigned int num_channels;
	unsigned int bytes_per_sample;
	unsigned int byte_rate;
	unsigned long i;	/* counter for samples */
	unsigned char *buffer;
	unsigned int sizeOfWav;

	//new load fn()
	char id[5];
	unsigned long size;
	short format_tag, channels, block_align,bits_per_sample;
	unsigned long format_length, avg_bytes_sec, data_size;

	SoundUtils(const char * path);
	~SoundUtils();
	inline unsigned char *GetSoundBuffer() { return buffer; }
	inline unsigned int GetSoundSize() { return sizeOfWav; }
	void LoadWav(const char * filename);
	void PlaySound();
	void AppendToWave(unsigned char *dest, unsigned char *src, int numBytes, int & offset);
	unsigned char *CreateWav(unsigned long num_samples, /*short*/ unsigned int * data, int s_rate, unsigned int & sizeOfWav);
	void WriteWav(char * filename, unsigned long num_samples, /*short*/ int * data, int s_rate);
	unsigned char *DoSoundCreateTest();

};

#endif //SOUND_PROXY_H