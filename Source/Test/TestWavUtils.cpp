//TestWavUtils.cpp


#include <cstring> //memset()
# include <cstdlib>
#include <cstdio>

#include "TestWavUtils.hpp"

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"

#include <cmath>


bool testWavReader(const char * inputSamplesDir) {
     
    printf("Testing WavReader...\n\n");
    
    const char inputFilePath[] = "/Users/krispin/Desktop/Krispins_Stuff/Projects/Public-Git-Repos/WavUtils/Source/Test/AudioSamples/Input/wav1ch8.wav";
        
    WavReader* wr = new WavReader();
    
    if (!wr->initialize(inputFilePath)) {
        fprintf(stderr, "Error: WavReader initialization failed; file may be corrupt.\n");
        return false;
    }
        
    printf("Metadata:\n");
    printf("   Input File Path: %s\n", wr->getReadFilePath());
    printf("   Sample Rate: %d\n", wr->getSampleRate());
    printf("   Number of Samples: %d\n", wr->getNumSamples());
    printf("   Number of Channels: %d\n", wr->getNumChannels());
    printf("   Representation: %s\n", wr->getSamplesAreInts() ? "Integer" : "Floating Point" );
    printf("   Byte Depth: %d\n", wr->getByteDepth());
    printf("   Sample Data Size: %d\n", wr->getSampleDataSize());
    printf("\n\n");
    
    uint32_t numChannels = wr->getNumChannels();
    uint32_t numSamples = wr->getNumSamples();
    uint32_t sampleDataSize = wr->getSampleDataSize();
    
    uint8_t sampleData[sampleDataSize];
    int16_t int16Samples[numSamples * numChannels];
    
    wr->prepareToRead();
    wr->readData(sampleData, sampleDataSize);
    for (int i=0; i<numSamples; i++) {
        int16_t sch1 = 0;
        int16_t sch2 = 0;
        wr->readInt16SampleFromArray(sampleData, sampleDataSize, i, sch1, sch2);
        int16Samples[i * numChannels] = sch1;
        if (numChannels == 2) {
            int16Samples[i * numChannels + 1] = sch2;
        }
    }
    
    wr->prepareToRead();
    wr->readDataToInt16s(int16Samples, numSamples);

    wr->finishReading();
    
    
    //printf("Sample Data:\n");
    //for (int i=0; i<numSamples; i++) {
    //    printf("%d ", int16Samples[i * numChannels]);
    //    if (numChannels == 2) {
    //        printf("%d ", int16Samples[i * numChannels + 1]);
    //    }
    //    printf("\n");
    //}
    //printf("\n");
    
    //printToPlot(int16Samples, numSamples, 100, numChannels);

    return true;
}



bool testWavWriter(const char * outputSamplesDir) {
    
     printf("Testing WavWriter...\n\n");
    
    const char outputFilePath[] = "/Users/krispin/Desktop/Krispins_Stuff/Projects/Public-Git-Repos/WavUtils/Source/Test/AudioSamples/Output/wav1ch16.wav";
    uint32_t sampleRate = 44100;
    uint32_t numSamples = floor(44100 * 0.05);
    uint32_t numChannels = 1;
    bool samplesAreInts = true;
    uint32_t byteDepth = 2;
    
    
    uint8_t int8Samples[numSamples*numChannels];
    for (int i=0; i<numSamples; i++) {
        int8Samples[i*numChannels] = (int8_t) ((127.0 * sin( (2 * M_PI * i) / 100.0)) + 127.0);
        if (numChannels == 2) {
            int8Samples[i*numChannels + 1] = int8Samples[i*numChannels];
        }
    }
    
    int16_t int16Samples[numSamples*numChannels];
    for (int i=0; i<numSamples; i++) {
        int16Samples[i*numChannels] = (int16_t) (32767.0 * sin( (2 * M_PI * i) / 100.0 ));
        if (numChannels == 2) {
            int16Samples[i*numChannels + 1] = int16Samples[i*numChannels];
        }
    }
    
    uint32_t int24Samples[numSamples*numChannels]; // NOTE: Stored in 4 bytes here.
    for (int i=0; i<numSamples; i++) {
        int24Samples[i*numChannels] = (int32_t) (8388607.0 * sin( (2 * M_PI * i) / 100.0 ));
        if (numChannels == 2) {
            int24Samples[i*numChannels + 1] = int24Samples[i*numChannels];
        }
    }
    
    int32_t int32Samples[numSamples*numChannels];
    for (int i=0; i<numSamples; i++) {
        int32Samples[i*numChannels] = (int32_t) (2147483647.0 * sin( (2 * M_PI * i) / 100.0 ));
        if (numChannels == 2) {
            int32Samples[i*numChannels + 1] = int32Samples[i*numChannels];
        }
    }
    
    float float32Samples[numSamples*numChannels];
    for (int i=0; i<numSamples; i++) {
        float32Samples[i*numChannels] = (float) sin( (2 * M_PI * i) / 100.0 );
        if (numChannels == 2) {
            float32Samples[i*numChannels + 1] = float32Samples[i*numChannels];
        }
    }
    
    double float64Samples[numSamples*numChannels];
    for (int i=0; i<numSamples; i++) {
        float64Samples[i*numChannels] = (double) sin( (2 * M_PI * i) / 100.0 );
        if (numChannels == 2) {
            float64Samples[i*numChannels + 1] = float64Samples[i*numChannels];
        }
    }
    
    //Set source 
    uint8_t* srcSampleBytes = nullptr;
    if (samplesAreInts) {
        switch (byteDepth) {
            case 1:
                srcSampleBytes = (uint8_t*)int8Samples;
                break;
            
            case 2:
                srcSampleBytes = (uint8_t*)int16Samples;
                break;

            case 3:
                srcSampleBytes = (uint8_t*)int24Samples;
                break;

            case 4:
                srcSampleBytes = (uint8_t*)int32Samples;
                break;

            default:
                fprintf(stderr, "Should never reach here.");
                exit(1);
                break;
        }
    }
    else { //floating-point
        if (byteDepth == 4) {
            srcSampleBytes = (uint8_t*)float32Samples;
        }
        else { //byteDepth == 8
            srcSampleBytes = (uint8_t*)float64Samples;
        }
    }
    
    uint32_t sampleDataSize = numSamples * byteDepth * numChannels;
    uint8_t sampleData[sampleDataSize];
    uint32_t destBlockSize = numChannels * byteDepth;
    uint32_t containerSize = (byteDepth==3 && samplesAreInts) ? byteDepth + 1 : byteDepth;
    for (int i=0; i<numSamples; i++) {
        uint8_t* bytes = &srcSampleBytes[i*numChannels*containerSize];
        uint32_t blockIndex = i * destBlockSize;
        int k = 0;
        for (int j=0; j<destBlockSize; j++) {
            if (byteDepth==3 && samplesAreInts && k == 3) { //Handle edge case of 2ch int24, in int32 containers; skip padding byte
                k++;
            }
            sampleData[blockIndex + j] = bytes[k];
            k++;
        }
    }
    
    
    //Test the writer


    WavWriter* ww = new WavWriter();

    bool ok = ww->initialize(outputFilePath,
                             sampleRate,
                             numSamples,
                             numChannels,
                             samplesAreInts,
                             byteDepth);
    if (!ok) {
        fprintf(stderr, "Error: Problem initializing WavWriter.\n");
        return false;
    }

    printf("Metadata Parameters:\n");
    printf("   Output File Path: %s\n", ww->getWriteFilePath());
    printf("   Sample Rate: %d\n", ww->getSampleRate());
    printf("   Number of Samples: %d\n", ww->getNumSamples());
    printf("   Number of Channels: %d\n", ww->getNumChannels());
    printf("   Representation: %s\n", ww->getSamplesAreInts() ? "Integer" : "Floating Point" );
    printf("   Byte Depth: %d\n", ww->getByteDepth());
    printf("   Sample Data Size: %d\n", ww->getSampleDataSize());
    printf("\n\n");

    ww->startWriting();
        
    ww->writeData(sampleData, sampleDataSize);
    //ww->writeDataFromInt16s(int16Samples, numSamples);

    ww->finishWriting();
    
    return true;
}



bool testWavReaderThenWriter(const char * inputSamplesDir, const char * outputSamplesDir) {
    printf("Testing WavReader -> WavWriter...\n\n");
    // *** SHOULD IT BE THE OTHER WAY AROUND? ***
    
    return true;
}
