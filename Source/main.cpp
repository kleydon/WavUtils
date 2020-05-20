//main.cpp


#include <iostream>

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"
#include "TestWavUtils.hpp"


/*
 TO DO:

    For writing: Is numSamples no longer necessary? Can it be removed?
 
    Have checks for uninitialized usage...
 
    Revisit sample rate value checks
 
    Architect tests, and main...
 
    Look at file includes...
 
 */




int main(int argc, const char * argv[]) {
    
    printf("Wav Utils\n\n");


    if (!testWavWriter(nullptr)) {
        fputs("Error: testWavWriter() returned false.\n", stderr);
        return(1);
    }
    
    if(!testWavReader(nullptr)) {
        fputs("Error: testWavWriter() returned false.\n", stderr);
        return(1);
    }

    
    return 0;
}
