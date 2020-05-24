//WavReaderTester.hpp

#ifndef __WAV_READER_TESTER_HPP__
#define __WAV_READER_TESTER_HPP__

#include <cstdint> //For uint8_t, etc.

#include "WavHeader.hpp" // Verifies that float and double correspond to f32 and f64 values
#include "WavReader.hpp"



typedef struct {
    const char* fileName;
    uint32_t sampleRate;
    uint32_t numChannels;
    bool samplesAreInts; //False if samples are 32 or 64-bit floating point values
    uint32_t byteDepth; //Number of significant bytes required to represent a single channel of a sample
} InFileParamSetDef;


typedef enum {
    VALIDATION_SOURCE_SAMPLEDATA = 0,
    VALIDATION_SOURCE_INT16SAMPLES = 1
} ValidationSource;


class WavReaderTester {
    
    public:
    
        WavReaderTester();
        ~ WavReaderTester();
    
        bool initialize(const char* inDirPath);
    
        bool runWavReaderTest();
            
    private:
    
        bool testReadFileAllAtOnce(const InFileParamSetDef* ifps);
        bool testReadFileIncrementally(const InFileParamSetDef* ifps);
        bool testReadFileToInt16s(const InFileParamSetDef* ifps);
        bool validates(const InFileParamSetDef* ifps, ValidationSource validationSource);
        
        //Constants
        static const uint32_t MAX_PATH_LENGTH = 2048;
        static const uint32_t MAX_FILE_NAME_LENGTH = 100;
        static const uint32_t SAMPLE_RATE = 44100;
        constexpr static const float SINE_FREQUENCY = 440.0;
        static const uint32_t MAX_NUM_CHANNELS = 2;
        static const uint32_t MAX_BYTE_DEPTH = 8; //For double-precision floating point representation
    
        //File Descriptions
        const InFileParamSetDef inFileParamSets[12] = {
            {
                //int8 mono
                "440HzSine1ChUInt8.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                true, //samplesAreInts
                1 //byteDepth
            },
            {
                //int8 stereo
                "440HzSine2ChUInt8.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                true, //samplesAreInts
                1 //byteDepth
            },
            {
                //int16 mono
                "440HzSine1ChInt16.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                true, //samplesAreInts
                2 //byteDepth
            },
            {
                //int16 stereo
                "440HzSine2ChInt16.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                true, //samplesAreInts
                2 //byteDepth
            },
            {
                //int24 mono
                "440HzSine1ChInt24.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                true, //samplesAreInts
                3 //byteDepth
            },
            {
                //int24 stereo
                "440HzSine2ChInt24.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                true, //samplesAreInts
                3 //byteDepth
            },
            {
                //int32 mono
                "440HzSine1ChInt32.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                true, //samplesAreInts
                4 //byteDepth
            },
            {
                //int32 stereo
                "440HzSine2ChInt32.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                true, //samplesAreInts
                4 //byteDepth
            },
            {
                //float32 mono
                "440HzSine1ChFloat32.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                false, //samplesAreInts
                4 //byteDepth
            },
            {
                //float32 stereo
                "440HzSine2ChFloat32.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                false, //samplesAreInts
                4 //byteDepth
            },
            {
                //float64 mono
                "440HzSine1ChFloat64.wav", //fileName
                SAMPLE_RATE, //sampleRate
                1, //numChannels
                false, //samplesAreInts
                8 //byteDepth
            },
            {
                //float64 stereo
                "440HzSine2ChFloat64.wav", //fileName
                SAMPLE_RATE, //sampleRate
                2, //numChannels
                false, //samplesAreInts
                8 //byteDepth
            }
        };
        const uint32_t NUM_FILE_PARAM_SETS = sizeof(inFileParamSets) / sizeof(InFileParamSetDef);
    
        //Array to hold sample data, in wav-file format
        uint8_t* sampleData;
        int16_t* int16Samples;
    
        WavReader* wavReader;
        const char* inDirPath;
};




#endif //__WAV_READER_TESTER_HPP__
