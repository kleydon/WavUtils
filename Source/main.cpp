//main.cpp

#include <iostream>

#include "WavWriterTester.hpp"
#include "WavReaderTester.hpp"



int main(int argc, const char * argv[]) {
    
    printf("\n\n~~~ WavUtilsTest ~~~\n\n\n");
    
    if (argc != 3) {
        printf("Usage: WavUtilsTest InputDir OutputDir\n\n");
        printf("  InputDir: Reference audio directory for this project,\n");
        printf("            i.e: Source/Test/ReferenceAudio.\n\n");
        printf("  OutputDir: A directory to write output wav files to\n");
        printf("\n\n\n");
        exit(0);
    }

    const char* referenceAudioDirectory = argv[1];
    const char* outputDirectory = argv[2];
        
    //Testing WavReader...
    WavReaderTester* wrt = new WavReaderTester();
    wrt->initialize(referenceAudioDirectory);
    wrt->runWavReaderTest();
    
    //Testing WavWriter...
    WavWriterTester* wwt = new WavWriterTester();
    wwt->initialize(outputDirectory);
    wwt->runWavWriterTest();
    
    return 0;
}
