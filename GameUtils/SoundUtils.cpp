#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>//memmove

#include "SoundUtils.h"


SoundUtils::SoundUtils(const char* sndFile)
{
	volume = 255;
	pan = 128;
	speed = 1000;
	LoadWav(sndFile);
	/*FILE *ptr_myfile;
	ptr_myfile = fopen(sndFile, "rb");
	if (!ptr_myfile)
	{
		printf("Unable to open file!");
		return;
	}

	fread(&buffer, sizeof(buffer), 1, ptr_myfile);
	fclose(ptr_myfile);*/
}

void SoundUtils::PlaySound()
{
	//play_sample(snd, volume, pan, speed, 0);
}

SoundUtils::~SoundUtils()
{
	//destroy_sample(snd);
	//snd = NULL;
}

//////////////////////////////////////////////////////////////////////////////////
void SoundUtils::write_little_endian(unsigned int word, int num_bytes, FILE *wav_file)
{
    unsigned buf;
    while(num_bytes>0)
    {   buf = word & 0xff;
	//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
    	fwrite(&buf, 1,1, wav_file);
        num_bytes--;
	word >>= 8;
    }
}
void SoundUtils::read_little_endian(unsigned char *buffer, int num_bytes, FILE *wav_file)
{
	unsigned buf;
	while (num_bytes>0)
	{
		fread(&buf, 1, 1, wav_file);
		num_bytes--;
		buffer[num_bytes] <<= 8;
	}
}
unsigned char* SoundUtils::write_to_buffer_little_endian(unsigned int word, int num_bytes)
{
    unsigned buf;
	unsigned char *buffer;
	int offset = 0;
	buffer = (unsigned char*) malloc(num_bytes);
    while(num_bytes>0)
    {   buf = word & 0xff;
		//void * memmove ( void * destination, const void * source, size_t num );
	//needs an offset so mem is overwritten
    	memmove(buffer+offset,&buf, 1);
        num_bytes--;
		offset++;
	word >>= 8;
    }
	return buffer;
}



void SoundUtils::LoadWav(const char * filename)
{
	//http://rogerchansdigitalworld.blogspot.com/2010/05/how-to-read-wav-format-file-in-c.html
	FILE* fp = fopen(filename, "rb");
	if (fp) 
	{
		
		fread(id, sizeof(char), 4, fp);
		id[4] = '\0';

		if (!strcmp(id, "RIFF")) 
		{
			fread(&size, sizeof(unsigned long), 1, fp);
			fread(id, sizeof(char), 4, fp);
			id[4] = '\0';

			if (!strcmp(id, "WAVE")) {
				fread(&format_length, sizeof(unsigned long), 1, fp);
				fread(&format_tag, sizeof(short), 1, fp);
				fread(&channels, sizeof(short), 1, fp);
				fread(&sample_rate, sizeof(unsigned long), 1, fp);
				fread(&avg_bytes_sec, sizeof(unsigned long), 1, fp);
				fread(&block_align, sizeof(short), 1, fp);
				fread(&bits_per_sample, sizeof(short), 1, fp);
				fread(id, sizeof(char), 4, fp);
				fread(&data_size, sizeof(unsigned long), 1, fp);

				sizeOfWav = data_size / sizeof(unsigned char);
				buffer = (unsigned char*)malloc(data_size);
				fread(buffer, sizeof(short), sizeOfWav, fp);
			}
			else 
			{
				return;
				//cout << "Error: RIFF file but not a wave file\n";
			}
		}
		else 
		{
			return;
			//cout << "Error: not a RIFF file\n";
		}
	}
	fclose(fp);
}

/*void SoundUtils::LoadWav(const char * filename)
{
FILE* wav_file;


wav_file = fopen(filename, "rb");
assert(wav_file);	// make sure it opened

// obtain file size:
fseek(wav_file, 0, SEEK_END);
long lSize = ftell(wav_file);
rewind(wav_file);

// allocate memory to contain the whole file:
buffer = (unsigned char*)malloc(sizeof(unsigned char)*lSize);
if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

// copy the file into the buffer:
long result = fread(buffer, 1, lSize, wav_file);
if (result != lSize) {
fputs("Reading error", stderr); exit(3);
}
sizeOfWav = lSize;
fclose(wav_file);
}*/




/* information about the WAV file format from
	http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 */

void SoundUtils::WriteWav(char * filename, unsigned long num_samples, int * data, int s_rate)
{
    FILE* wav_file;
    unsigned int sample_rate;
    unsigned int num_channels;
    unsigned int bytes_per_sample;
    unsigned int byte_rate;
    unsigned long i;	/* counter for samples */

    num_channels = 1;	/* monoaural */
    bytes_per_sample = 2;

    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;

    byte_rate = sample_rate*num_channels*bytes_per_sample;

    wav_file = fopen(filename, "w");
    assert(wav_file);	/* make sure it opened */

    /* write RIFF header */
    fwrite("RIFF", 1, 4, wav_file);
    write_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4, wav_file);
    fwrite("WAVE", 1, 4, wav_file);

    /* write fmt  subchunk */
    fwrite("fmt ", 1, 4, wav_file);
    write_little_endian(SUBCHUNK1SIZE, 4, wav_file);	/* SubChunk1Size is 16 */
    write_little_endian(PCM_FORMAT, 2, wav_file);	/* PCM is format 1 */
    write_little_endian(num_channels, 2, wav_file);
    write_little_endian(sample_rate, 4, wav_file);
    write_little_endian(byte_rate, 4, wav_file);
    write_little_endian(num_channels*bytes_per_sample, 2, wav_file);  /* block align */
    write_little_endian(8*bytes_per_sample, 2, wav_file);  /* bits/sample */

    /* write data subchunk */
    fwrite("data", 1, 4, wav_file);
    write_little_endian(bytes_per_sample* num_samples*num_channels, 4, wav_file);
    for (i=0; i< num_samples; i++)
    { 	write_little_endian((unsigned int)(data[i]),bytes_per_sample, wav_file);
    }

    fclose(wav_file);
}



void SoundUtils::AppendToWave(unsigned char *dest, unsigned char *src, int numBytes, int & offset)
{
//	char *buffer = (char *)memcpy (dest+offset,src,numBytes);
	//memmove (dest+offset,src,numBytes);
	for(int i = 0; i < numBytes; i++)
		dest[i + offset] = src[i];

	offset += numBytes;
	//return buffer;
	
}
unsigned char *SoundUtils::CreateWav(unsigned long num_samples, /*short*/ unsigned int * data, int s_rate, unsigned int & sizeOfWav)
{
	int blockCount = 0;// when we realloc, we willnned a running count on current size
	unsigned char *wav_file;//final result

    num_channels = 1;	/* monoaural */
    bytes_per_sample = 2;

    if (s_rate<=0) sample_rate = 44100;
    else sample_rate = (unsigned int) s_rate;

    byte_rate = sample_rate*num_channels*bytes_per_sample;

//--------------------------------------
    /* write RIFF header */
    unsigned char riffHeader[5] = "RIFF";
    unsigned char *riffData = write_to_buffer_little_endian(36 + bytes_per_sample* num_samples*num_channels, 4);
    unsigned char waveHeader[5] = "WAVE";

    /* write fmt  subchunk */
    unsigned char fmtSubchunk[5] = "fmt ";
    unsigned char *subChunk = write_to_buffer_little_endian(SUBCHUNK1SIZE, 4);	/* SubChunk1Size is 16 */
    unsigned char *format =write_to_buffer_little_endian(PCM_FORMAT, 2);	/* PCM is format 1 */
    unsigned char *channels = write_to_buffer_little_endian(num_channels, 2);
    unsigned char *sampRate =write_to_buffer_little_endian(sample_rate, 4);
    unsigned char *byteRate =write_to_buffer_little_endian(byte_rate, 4);
    unsigned char *align =write_to_buffer_little_endian(num_channels*bytes_per_sample, 2);  /* block align */
    unsigned char *bitsPerSample =write_to_buffer_little_endian(8*bytes_per_sample, 2);  /* bits/sample */

    /* write data subchunk */
    unsigned char dataSubchunk[5] = "data";
    unsigned char *dataChunk =write_to_buffer_little_endian(bytes_per_sample* num_samples*num_channels, 4);
//-------------------------------------
	wav_file = (unsigned char*) malloc(45);//add up the previous mem allocations to get 45
	 memset (wav_file,'-',44); 
	//piece together wav broiler plate stuff here
	AppendToWave(wav_file, riffHeader, 4, blockCount);
	AppendToWave(wav_file, riffData, 4, blockCount);
	AppendToWave(wav_file, waveHeader, 4, blockCount);
	AppendToWave(wav_file, fmtSubchunk, 4, blockCount);

	AppendToWave(wav_file, subChunk, 4, blockCount);
	AppendToWave(wav_file, format, 2, blockCount);
	AppendToWave(wav_file, channels, 2, blockCount);
	AppendToWave(wav_file, sampRate, 4, blockCount);
	AppendToWave(wav_file, byteRate, 4, blockCount);
	AppendToWave(wav_file, align, 2, blockCount);
	AppendToWave(wav_file, bitsPerSample, 2, blockCount);
	AppendToWave(wav_file, dataSubchunk, 4, blockCount);
	AppendToWave(wav_file, dataChunk, 4, blockCount);
	
	//now add sound data that was passed in from param
    for (i=0; i< num_samples; i++)
	{
     	
		unsigned char *temp = write_to_buffer_little_endian((unsigned int)(data[i]),bytes_per_sample);
		wav_file = (unsigned char*) realloc (wav_file, blockCount+bytes_per_sample * sizeof(unsigned char*));
		AppendToWave(wav_file, temp, bytes_per_sample, blockCount);

	}
	sizeOfWav = blockCount;
	return wav_file;
	
}

unsigned char* SoundUtils::DoSoundCreateTest()
{
    float amplitude = 32000;//volume
    float freq_Hz = 261.626;//this is middle C //240;//note
    float phase=0;

    float freq_radians_per_sample = freq_Hz*2*M_PI/GEN_SAMP_RATE;
	unsigned int buf[BUF_SIZE];
    // fill buffer with a sine wave 
    for (int i=0; i<BUF_SIZE; i++)
    {
        phase += freq_radians_per_sample;
		buffer[i] = (int)(amplitude * sin(phase));
    }
	
    return CreateWav(BUF_SIZE, buf, GEN_SAMP_RATE ,sizeOfWav);
}

/*int SoundUtils::GetDurationInSeconds(struct header* hdr)
{
    int numSamples = hdr->subchunk2_size / (hdr->num_channels * (hdr->bits_per_sample/8));
    int durationSeconds = numSamples / hdr->sample_rate;
    return durationSeconds;
}*/