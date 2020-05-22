//main.cpp


#include <iostream>

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"
#include "TestWavUtils.hpp"


/*
 TO DO:

    In writing, and finishing writes: Verify that lengths are on sample block boundaries.

    Check to ensure file size doesn't get out of hand
 
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
