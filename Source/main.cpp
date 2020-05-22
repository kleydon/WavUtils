//main.cpp


#include <iostream>

#include "WavHeader.hpp"
#include "WavReader.hpp"
#include "WavWriter.hpp"
#include "TestWavUtils.hpp"


/*
 TO DO:
 
    Architect tests, and main...
 
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
