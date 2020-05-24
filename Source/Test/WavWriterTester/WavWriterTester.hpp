
//WavWriterTester.hpp

#ifndef __WAV_WRITER_TESTER_HPP__
#define __WAV_WRITER_TESTER_HPP__

#include <cstdint> //For uint8_t, etc.

#include "WavHeader.hpp" // Verifies that float and double correspond to f32 and f64 values
#include "WavWriter.hpp"



typedef struct {
    uint32_t sampleRate;
    uint32_t numSamples;
    uint32_t numChannels;
    bool samplesAreInts; //False if samples are 32 or 64-bit floating point values
    uint32_t byteDepth; //Number of significant bytes required to represent a single channel of a sample
} OutFileParamSetDef;




class WavWriterTester {
    
    public:
    
        WavWriterTester();
        ~ WavWriterTester();
    
        bool initialize(const char* outDirPath);
    
        bool runWavWriterTest();
            
    private:

        bool writeFileAllAtOnce(const OutFileParamSetDef* ofps);
        bool writeFileIncrementally(const OutFileParamSetDef* ofps);
        bool writeFileFromInt16s(uint32_t numChannels);

        //Writes samples to an array as held in wav-format data section
        bool setSampleData(uint32_t numChannels, uint32_t byteDepth, bool samplesAreInts);

        //Constants
        static const uint32_t MAX_PATH_LENGTH = 2048;
        static const uint32_t MAX_FILE_NAME_LENGTH = 100;
        static const uint32_t NUM_SAMPLES = 2048;
        static const uint32_t SAMPLE_RATE = 44100;
        static const uint32_t MAX_NUM_CHANNELS = 2;
        static const uint32_t MAX_BYTE_DEPTH = 8; //For double-precision floating point representation
    
        //File Descriptions
        const OutFileParamSetDef outFileParamSets[12] = {
            {
                //int8 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                true, //samplesAreInts
                1 //byteDepth
            },
            {
                //int8 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                true, //samplesAreInts
                1 //byteDepth
            },
            {
                //int16 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                true, //samplesAreInts
                2 //byteDepth
            },
            {
                //int16 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                true, //samplesAreInts
                2 //byteDepth
            },
            {
                //int24 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                true, //samplesAreInts
                3 //byteDepth
            },
            {
                //int24 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                true, //samplesAreInts
                3 //byteDepth
            },
            {
                //int32 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                true, //samplesAreInts
                4 //byteDepth
            },
            {
                //int32 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                true, //samplesAreInts
                4 //byteDepth
            },
            {
                //float32 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                false, //samplesAreInts
                4 //byteDepth
            },
            {
                //float32 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                false, //samplesAreInts
                4 //byteDepth
            },
            {
                //float64 mono
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                1, //numChannels
                false, //samplesAreInts
                8 //byteDepth
            },
            {
                //float64 stereo
                SAMPLE_RATE, //sampleRate
                NUM_SAMPLES, //numSamples
                2, //numChannels
                false, //samplesAreInts
                8 //byteDepth
            }
        };
        const uint32_t NUM_FILE_PARAM_SETS = sizeof(outFileParamSets) / sizeof(OutFileParamSetDef);
        
    
        //Sample Arrays
        uint8_t uint8Samples1Ch[NUM_SAMPLES*1];
        uint8_t uint8Samples2Ch[NUM_SAMPLES*2];

        int16_t int16Samples1Ch[NUM_SAMPLES*1];
        int16_t int16Samples2Ch[NUM_SAMPLES*2];

        int32_t int24Samples1Ch[NUM_SAMPLES*1];
        int32_t int24Samples2Ch[NUM_SAMPLES*2];

        int32_t int32Samples1Ch[NUM_SAMPLES*1];
        int32_t int32Samples2Ch[NUM_SAMPLES*2];

        float float32Samples1Ch[NUM_SAMPLES*1];
        float float32Samples2Ch[NUM_SAMPLES*2];

        double float64Samples1Ch[NUM_SAMPLES*1];
        double float64Samples2Ch[NUM_SAMPLES*2];

        //Array to hold sample data, in wav-file format
        uint8_t sampleData[NUM_SAMPLES*MAX_NUM_CHANNELS*MAX_BYTE_DEPTH]; //Large enough for all permutations
    
        WavWriter* wavWriter;
        const char* outDirPath;
};



#endif //__WAV_WRITER_TESTER_HPP__
