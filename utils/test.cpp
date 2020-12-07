#include <iostream>
#include <cstdint>
#include <cmath>
 
typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;
 
void CreateWavFile(const char* filePath, const double* Amplitude, int VectorSize)
{
    int16_t Data;
    wav_hdr wavHeader;
    wavHeader.RIFF[0] = 'R'; wavHeader.RIFF[1] = 'I'; wavHeader.RIFF[2] = 'F'; wavHeader.RIFF[3] = 'F';
    wavHeader.WAVE[0] = 'W'; wavHeader.WAVE[1] = 'A'; wavHeader.WAVE[2] = 'V'; wavHeader.WAVE[3] = 'E';
    wavHeader.fmt[0] = 'f';  wavHeader.fmt[1] = 'm';  wavHeader.fmt[2] = 't';  wavHeader.fmt[3] = ' ';
    wavHeader.Subchunk2ID[0] = 'd'; wavHeader.Subchunk2ID[1] = 'a'; wavHeader.Subchunk2ID[2] = 't'; wavHeader.Subchunk2ID[3] = 'a';
    wavHeader.Subchunk1Size = 16;
    wavHeader.AudioFormat = 1;
    wavHeader.NumOfChan = 1;
    wavHeader.SamplesPerSec = 48000;
    wavHeader.bytesPerSec = 96000;
    wavHeader.bitsPerSample = 16;
    wavHeader.blockAlign = 2;
    wavHeader.Subchunk2Size = VectorSize * wavHeader.blockAlign;
    //wavHeader.NumOfChan*(static_cast<unsigned int>(VectorSize))*wavHeader.blockAlign;
    wavHeader.ChunkSize = sizeof(wav_hdr) - 8 + wavHeader.Subchunk2Size;
    // wavHeader.ChunkSize = 4 + (8 + wavHeader.Subchunk1Size) + (8 + wavHeader.Subchunk2Size);
    size_t headerSize = sizeof(wav_hdr);
 
    FILE* wavFile = fopen(filePath, "wb"); // <-----------  !!!!!!!!!!!!!!!!!!!!!!
    //FILE* wavFile = fopen(filePath, "w");
    fwrite(&wavHeader, 1, headerSize, wavFile);
    //fseek(wavFile, long(headerSize), SEEK_SET);    
    for(int i = 0; i < VectorSize; i++)
    {
        Data = int16_t(Amplitude[i]);
        fwrite(&Data, 2, 1, wavFile);
        //fwrite(&Data, 1, wavHeader.Subchunk1Size, wavFile);
        //long position = i*sizeof(uint16_t) + headerSize;
        //fseek(wavFile, position, SEEK_SET);
    }
    fclose(wavFile);
}
 
int main()
{
	int k = 16;
    double data[k];
    for (int i=0; i<k; i++)
        data[i] = 10000.0 * sin(2 * 3.14 * i / 48);
	//for (int i=50; i<100; i++)
    //   data[i] = 100.0 * sin(2 * 3.14 * i / 48);
 
    CreateWavFile("test.wav", data, k);
 
    system("pause");
    return 0;
}