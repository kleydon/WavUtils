//main.cpp


#include <iostream>

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"
#include "TestWavUtils.hpp"


/*
 TO DO:
 
    Make a repo for this. (Locally, where to put it?)
 
    Writing: Rewind and update file length, based on what's known at the point of finishing the write.
 
    For both writing and reading, have a initialize() function that can return false - so that error checking can happen.
 
    For read: Make reading metadata implicit and private; perhaps in initialize().

    File path safety checks
 
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
