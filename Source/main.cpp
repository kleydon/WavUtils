//main.cpp


#include <iostream>

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"
#include "TestWavUtils.hpp"


/*
 TO DO:

    File path safety checks

    For both writing and reading, have a initialize() function that can return false - so that error checking can happen.
 
    For read: Make reading metadata implicit and private; perhaps in initialize().
 
    Revisit sample rate value checks
 
    Architect tests, and main...
 
 */




int main(int argc, const char * argv[]) {
    
    printf("Wav Utils\n\n");


    if (!testWavWriter(nullptr)) {
        fputs("Error: testWavWriter() returned false.", stderr);
        return(1);
    }
    
    if(!testWavReader(nullptr)) {
        fputs("Error: testWavWriter() returned false.", stderr);
        return(1);
    }

    
    return 0;
}
