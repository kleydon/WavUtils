//TestWavUtils.hpp

#ifndef __TEST_WAV_UTILS_HPP__
#define __TEST_WAV_UTILS_HPP__

#include <cstdio> //For FILE
#include <cstdint> //For uint8_t, etc.


bool testWavReader(const char * inputSamplesDir);

bool testWavWriter(const char * inputSamplesDir);

bool testWavReaderThenWriter(const char * inputSamplesDir, const char * outputSamplesDir);


#endif
