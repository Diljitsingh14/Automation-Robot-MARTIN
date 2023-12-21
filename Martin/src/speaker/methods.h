#ifndef SPEAKER_METHODS
#define SPEAKER_METHODS

#include "Arduino.h"
#include "SD.h"
#include "config.h"
#include "driver/i2s.h"

#define NUM_BYTES_TO_READ_FROM_FILE 1024

static const i2s_port_t i2s_speaker_port = I2S_SPEAKER_PORT;

struct WavHeader_Struct
{
    //   RIFF Section
    char RIFFSectionID[4]; // Letters "RIFF"
    uint32_t Size;         // Size of entire file less 8
    char RiffFormat[4];    // Letters "WAVE"

    //   Format Section
    char FormatSectionID[4]; // letters "fmt"
    uint32_t FormatSize;     // Size of format section less 8
    uint16_t FormatID;       // 1=uncompressed PCM
    uint16_t NumChannels;    // 1=mono,2=stereo
    uint32_t SampleRate;     // 44100, 16000, 8000 etc.
    uint32_t ByteRate;       // =SampleRate * Channels * (BitsPerSample/8)
    uint16_t BlockAlign;     // =Channels * (BitsPerSample/8)
    uint16_t BitsPerSample;  // 8,16,24 or 32

    // Data Section
    char DataSectionID[4]; // The letters "data"
    uint32_t DataSize;     // Size of the data that follows
} WavHeader;

File WavFile;

void PlayWav()
{
    static bool ReadingFile = true;                   // True if reading file from SD. false if filling I2S buffer
    static byte Samples[NUM_BYTES_TO_READ_FROM_FILE]; // Memory allocated to store the data read in from the wav file
    static uint16_t BytesRead;                        // Num bytes actually read from the wav file which will either be
                                                      // NUM_BYTES_TO_READ_FROM_FILE or less than this if we are very
                                                      // near the end of the file. i.e. we can't read beyond the file.

    if (ReadingFile) // Read next chunk of data in from file if needed
    {
        BytesRead = ReadFile(Samples); // Read data into our memory buffer, return num bytes read in
        ReadingFile = false;           // Switch to sending the buffer to the I2S
    }
    else
        ReadingFile = FillI2SBuffer(Samples, BytesRead); // We keep calling this routine until it returns true, at which point
                                                         // this will swap us back to Reading the next block of data from the file.
                                                         // Reading true means it has managed to push all the data to the I2S
                                                         // Handler, false means there still more to do and you should call this
                                                         // routine again and again until it returns true.
}

uint16_t ReadFile(byte *Samples)
{
    static uint32_t BytesReadSoFar = 0; // Number of bytes read from file so far
    uint16_t BytesToRead;               // Number of bytes to read from the file

    if (BytesReadSoFar + NUM_BYTES_TO_READ_FROM_FILE > WavHeader.DataSize) // If next read will go past the end then adjust the
        BytesToRead = WavHeader.DataSize - BytesReadSoFar;                 // amount to read to whatever is remaining to read
    else
        BytesToRead = NUM_BYTES_TO_READ_FROM_FILE; // Default to max to read

    WavFile.read(Samples, BytesToRead); // Read in the bytes from the file
    BytesReadSoFar += BytesToRead;      // Update the total bytes red in so far

    if (BytesReadSoFar >= WavHeader.DataSize) // Have we read in all the data?
    {
        WavFile.seek(44);   // Reset to start of wav data
        BytesReadSoFar = 0; // Clear to no bytes read in so far
    }
    return BytesToRead; // return the number of bytes read into buffer
}

bool FillI2SBuffer(byte *Samples, uint16_t BytesInBuffer)
{
    // Writes bytes to buffer, returns true if all bytes sent else false, keeps track itself of how many left
    // to write, so just keep calling this routine until returns true to know they've all been written, then
    // you can re-fill the buffer

    size_t BytesWritten;           // Returned by the I2S write routine,
    static uint16_t BufferIdx = 0; // Current pos of buffer to output next
    uint8_t *DataPtr;              // Point to next data to send to I2S
    uint16_t BytesToSend;          // Number of bytes to send to I2S

    // To make the code eaier to understand I'm using to variables to some calculations, normally I'd write this calcs
    // directly into the line of code where they belong, but this make it easier to understand what's happening

    DataPtr = Samples + BufferIdx;                                       // Set address to next byte in buffer to send out
    BytesToSend = BytesInBuffer - BufferIdx;                             // This is amount to send (total less what we've already sent)
    i2s_write(i2s_speaker_port, DataPtr, BytesToSend, &BytesWritten, 1); // Send the bytes, wait 1 RTOS tick to complete
    BufferIdx += BytesWritten;                                           // increasue by number of bytes actually written

    if (BufferIdx >= BytesInBuffer)
    {
        // sent out all bytes in buffer, reset and return true to indicate this
        BufferIdx = 0;
        return true;
    }
    else
        return false; // Still more data to send to I2S so return false to indicate this
}

bool ValidWavData(WavHeader_Struct *Wav)
{

    if (memcmp(Wav->RIFFSectionID, "RIFF", 4) != 0)
    {
        Serial.print("Invalid data - Not RIFF format");
        return false;
    }
    if (memcmp(Wav->RiffFormat, "WAVE", 4) != 0)
    {
        Serial.print("Invalid data - Not Wave file");
        return false;
    }
    if (memcmp(Wav->FormatSectionID, "fmt", 3) != 0)
    {
        Serial.print("Invalid data - No format section found");
        return false;
    }
    if (memcmp(Wav->DataSectionID, "data", 4) != 0)
    {
        Serial.print("Invalid data - data section not found");
        return false;
    }
    if (Wav->FormatID != 1)
    {
        Serial.print("Invalid data - format Id must be 1");
        return false;
    }
    if (Wav->FormatSize != 16)
    {
        Serial.print("Invalid data - format section size must be 16.");
        return false;
    }
    if ((Wav->NumChannels != 1) & (Wav->NumChannels != 2))
    {
        Serial.print("Invalid data - only mono or stereo permitted.");
        return false;
    }
    if (Wav->SampleRate > 48000)
    {
        Serial.print("Invalid data - Sample rate cannot be greater than 48000");
        return false;
    }
    if ((Wav->BitsPerSample != 8) & (Wav->BitsPerSample != 16))
    {
        Serial.print("Invalid data - Only 8 or 16 bits per sample permitted.");
        return false;
    }
    return true;
}

void DumpWAVHeader(WavHeader_Struct *Wav)
{
    if (memcmp(Wav->RIFFSectionID, "RIFF", 4) != 0)
    {
        Serial.print("Not a RIFF format file - ");
        PrintData(Wav->RIFFSectionID, 4);
        return;
    }
    if (memcmp(Wav->RiffFormat, "WAVE", 4) != 0)
    {
        Serial.print("Not a WAVE file - ");
        PrintData(Wav->RiffFormat, 4);
        return;
    }
    if (memcmp(Wav->FormatSectionID, "fmt", 3) != 0)
    {
        Serial.print("fmt ID not present - ");
        PrintData(Wav->FormatSectionID, 3);
        return;
    }
    if (memcmp(Wav->DataSectionID, "data", 4) != 0)
    {
        Serial.print("data ID not present - ");
        PrintData(Wav->DataSectionID, 4);
        return;
    }
    // All looks good, dump the data
    Serial.print("Total size :");
    Serial.println(Wav->Size);
    Serial.print("Format section size :");
    Serial.println(Wav->FormatSize);
    Serial.print("Wave format :");
    Serial.println(Wav->FormatID);
    Serial.print("Channels :");
    Serial.println(Wav->NumChannels);
    Serial.print("Sample Rate :");
    Serial.println(Wav->SampleRate);
    Serial.print("Byte Rate :");
    Serial.println(Wav->ByteRate);
    Serial.print("Block Align :");
    Serial.println(Wav->BlockAlign);
    Serial.print("Bits Per Sample :");
    Serial.println(Wav->BitsPerSample);
    Serial.print("Data Size :");
    Serial.println(Wav->DataSize);
}

void PrintData(const char *Data, uint8_t NumBytes)
{
    for (uint8_t i = 0; i < NumBytes; i++)
        Serial.print(Data[i]);
    Serial.println();
}

#endif