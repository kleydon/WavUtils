//main.cpp

#include <iostream>

#include "WavWriterTester.hpp"
#include "WavReaderTester.hpp"



int main(int argc, const char * argv[]) {
    
    printf("\n\n~~~ WavUtilsTest ~~~\n\n");
    
    if (argc != 3) {
        printf("Usage: WavUtilsTest InputDir OutputDir\n");
        printf("  InputDir: 'ReferenceAudio' directory, from this project\n");
        printf("  OutputDir: A directory for writing output wav files\n");
        printf("\n\n");
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
