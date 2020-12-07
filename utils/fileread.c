#include "wave.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1 
#define FALSE 0

// WAVE header structure

unsigned char buffer4[4];
unsigned char buffer2[2];

FILE *ptr;
char *filename;
struct HEADER header;

int *fileread(char *str) 
{
ptr = fopen("test.wav", "rb");
if (ptr == NULL)
{
	printf("Error opening file\n");
	exit(1);
}
int read = 0;
read = fread(header.riff, sizeof(header.riff), 1, ptr);
read = fread(buffer4, sizeof(buffer4), 1, ptr);
header.overall_size  = buffer4[0] | (buffer4[1]<<8) | (buffer4[2]<<16) | (buffer4[3]<<24);
read = fread(header.wave, sizeof(header.wave), 1, ptr);
read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
read = fread(buffer4, sizeof(buffer4), 1, ptr);
header.length_of_fmt = buffer4[0] |(buffer4[1] << 8) |(buffer4[2] << 16) |(buffer4[3] << 24);
read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);
header.format_type = buffer2[0] | (buffer2[1] << 8);
char format_name[10] = "";
if (header.format_type == 1)
	strcpy(format_name,"PCM"); 
else if (header.format_type == 6)
	strcpy(format_name, "A-law");
else if (header.format_type == 7)
	strcpy(format_name, "Mu-law");
read = fread(buffer2, sizeof(buffer2), 1, ptr);
header.channels = buffer2[0] | (buffer2[1] << 8);
read = fread(buffer4, sizeof(buffer4), 1, ptr);
header.sample_rate = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
read = fread(buffer4, sizeof(buffer4), 1, ptr);
header.byterate  = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24);
read = fread(buffer2, sizeof(buffer2), 1, ptr);
header.block_align = buffer2[0] | (buffer2[1] << 8);
read = fread(buffer2, sizeof(buffer2), 1, ptr);
header.bits_per_sample = buffer2[0] | (buffer2[1] << 8);
read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
read = fread(buffer4, sizeof(buffer4), 1, ptr);
header.data_size = buffer4[0] | (buffer4[1] << 8) | (buffer4[2] << 16) | (buffer4[3] << 24 );
long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
int data[num_samples];
if (header.format_type == 1)
{
		long i =0;
		char data_buffer[size_of_each_sample];
		int  size_is_correct = TRUE;
		long bytes_in_each_channel = (size_of_each_sample / header.channels);
		if ((bytes_in_each_channel  * header.channels) != size_of_each_sample)
		{
			printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
			size_is_correct = FALSE;
		}
		if (size_is_correct)
		{ 
			long low_limit = 0l;
			long high_limit = 0l;

			switch (header.bits_per_sample) {
				case 8:
					low_limit = -128;
					high_limit = 127;
					break;
				case 16:
					low_limit = -32768;
					high_limit = 32767;
					break;
				case 32:
					low_limit = -2147483648;
					high_limit = 2147483647;
					break;
			}					
			for (i =1; i <= num_samples; i++)
			{
				read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
				if (read == 1)
				{
					unsigned int  xchannels = 0;
					int data_in_channel = 0;
					int offset = 0; // move the offset for every iteration in the loop below
					for (xchannels = 0; xchannels < header.channels; xchannels ++ )
					{
						if (bytes_in_each_channel == 4) {
							data_in_channel = (data_buffer[offset] & 0x00ff) | 
												((data_buffer[offset + 1] & 0x00ff) <<8) | 
												((data_buffer[offset + 2] & 0x00ff) <<16) | 
												(data_buffer[offset + 3]<<24);
						}
						else if (bytes_in_each_channel == 2) 
						{
							data_in_channel = (data_buffer[offset] & 0x00ff) |
												(data_buffer[offset + 1] << 8);
						}
						else if (bytes_in_each_channel == 1) 
						{
							data_in_channel = data_buffer[offset] & 0x00ff;
							data_in_channel -= 128; //in wave, 8-bit are unsigned, so shifting to signed
						}
						offset += bytes_in_each_channel;		
						if (data_in_channel < low_limit || data_in_channel > high_limit)
					data[i] = data_in_channel;
				}
			} 
		} 
	}
}
fclose(ptr);
free(filename); // cleanup
return (data);
}