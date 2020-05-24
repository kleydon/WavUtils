//TestFileParamSetDef.hpp

#ifndef __TEST_FILE_PARAM_SET_DEF_HPP__
#define __TEST_FILE_PARAM_SET_DEF_HPP__

#include <cstdint> //For uint8_t, etc.



typedef struct {
    uint32_t sampleRate;
    uint32_t numSamples;
    uint32_t numChannels;
    bool samplesAreInts; //False if samples are 32 or 64-bit floating point values
    uint32_t byteDepth; //Number of significant bytes required to represent a single channel of a sample
} FileParamSetDef;










#endif //__TEST_FILE_PARAM_SET_DEF_HPP__
