//WavReaderTester.cpp


#include <cmath>

#include "WavReaderTester.hpp"

#include "WavHeader.hpp"
#include "WavReader.hpp"

#include <stdlib.h> //malloc and free



WavReaderTester::WavReaderTester() {
    inDirPath = nullptr;
    sampleData = nullptr;
    int16Samples = nullptr;
    wavReader = new WavReader();
}



WavReaderTester::~WavReaderTester() {
    if (wavReader) {
        delete wavReader;
        wavReader = nullptr;
    }
    if (sampleData) {
        free(sampleData);
        sampleData = nullptr;
    }
    if (int16Samples) {
        free(int16Samples);
        int16Samples = nullptr;
    }
}



bool WavReaderTester::initialize(const char *inDirPath) {
     
    //printf("Initializing WavReaderTester.\n\n");
        
    //Error-check inDirPath
    if (!inDirPath) {
        fprintf(stderr, "Error: Input directory path is NULL.\n");
        return false;
    }
    char tempFilePath[MAX_PATH_LENGTH];
    sprintf(tempFilePath, "%s/WavReaderTesterTempFile.txt", inDirPath);
    FILE* fp = fopen(tempFilePath, "w");
    if (!fp) {
        fprintf(stderr, "Error: Unable to open temp file at path:\n%s.\n", tempFilePath);
        return false;
    }
    if (remove(tempFilePath) != 0) {
        fprintf(stderr, "Error: Unable to remove test file created to validate directory path.\n");
        fclose(fp);
        return false;
    }
    fclose(fp);
     
    this->inDirPath = inDirPath;
    
    if (sampleData) {
        free(sampleData);
        sampleData = nullptr;
    }
    
    if (int16Samples) {
        free(int16Samples);
        int16Samples = nullptr;
    }
    
    return true;
}



bool WavReaderTester::runWavReaderTest() {
    
    printf("Running WavReaderTest.\n");
    
    //Reading files all at once
    printf("    Testing reading files, all-at-once...\n");
    for (int i=0; i<NUM_FILE_PARAM_SETS; i++) {
        testReadFileAllAtOnce(&inFileParamSets[i]);
    }
    
    //Read files incrementally
    printf("    Testing reading files, incrementally...\n");
    for (int i=0; i<NUM_FILE_PARAM_SETS; i++) {
        testReadFileIncrementally(&inFileParamSets[i]);
    }
    
    //Read files from int16 sample arrays
    printf("    Testing reading files from int16s...\n");
    for (int i=0; i<NUM_FILE_PARAM_SETS; i++) {
        if (!testReadFileToInt16s(&inFileParamSets[i])) {
            fprintf(stderr, "runWavReaderTest(): Error test-reading file to int16s.\n");
            return false;
        }
    }
    
    printf("Done WavReaderTest.\n\n");

    return true;
}



bool WavReaderTester::testReadFileAllAtOnce(const InFileParamSetDef* ifps) {
    
    const char* fileName = ifps->fileName;

    char inFilePath[MAX_PATH_LENGTH];
    sprintf(inFilePath,
            "%s/%s",
            inDirPath,
            fileName);
    
    if (!wavReader->initialize(inFilePath)) {
        fprintf(stderr, "readFileAllAtOnce(): Problem initializing WavReader.\n");
        return false;
    }
    
    if (!wavReader->prepareToRead()) {
        fprintf(stderr, "readFileAllAtOnce(): Problem preparing to read.\n");
        return false;
    }
    
    //Allocate sampleData
    
    uint32_t sampleDataSize = wavReader->getSampleDataSize();
    if (sampleData) {
        free(sampleData);
        sampleData = nullptr;
    }
    sampleData = (uint8_t*) malloc(sampleDataSize);

    if (!wavReader->readData(sampleData, sampleDataSize)) {
        fprintf(stderr, "readFileAllAtOnce(): Problem reading data.\n");
        return false;
    }
    
    if (!wavReader->finishReading()) {
        fprintf(stderr, "readFileAllAtOnce(): Problem finishing reading.\n");
        return false;
    }
    
    //If file metadata or content doesn't match expectations
    if (!validates(ifps, VALIDATION_SOURCE_SAMPLEDATA)) {
        fprintf(stderr, "testReadFileAllAtOnce(): Data or metadata doesn't validate.\n");
        return false;
    }
    
    return true;
}



bool WavReaderTester::testReadFileIncrementally(const InFileParamSetDef* ifps) {
    
    const char* fileName = ifps->fileName;
    const uint32_t numChannels = ifps->numChannels;
    const uint32_t byteDepth = ifps->byteDepth;
    
    char inFilePath[MAX_PATH_LENGTH];
    sprintf(inFilePath,
            "%s/%s",
            inDirPath,
            fileName);
        
    if (!wavReader->initialize(inFilePath)) {
        fprintf(stderr, "readFileIncrementally(): Problem initializing WavReader.\n");
        return false;
    }
    
    if (!wavReader->prepareToRead()) {
        fprintf(stderr, "readFileIncrementally(): Problem preparing to read.\n");
        return false;
    }
    
    //Allocate sampleData
    const uint32_t sampleDataSize = wavReader->getNumSamples() * wavReader->getNumChannels() * wavReader->getByteDepth();
    if (sampleData) {
        free(sampleData);
        sampleData = nullptr;
    }
    sampleData = (uint8_t*) malloc(sampleDataSize);

    const uint32_t bufferSize = numChannels *  byteDepth * 1; //Pick a size that falls on even sample block boundary
    uint8_t buffer[bufferSize];
    
    uint32_t i = 0;
    while (i < sampleDataSize) {
        
        //Read buffer from file
        const uint32_t numBytesToRead = (sampleDataSize - i < bufferSize) ? sampleDataSize - i : bufferSize;
        //printf("%d\n", numBytesToRead);
        if (!wavReader->readData(buffer, numBytesToRead)) {
            fprintf(stderr, "readFileIncrementally(): Problem reading data.\n");
            return false;
        }
        
        //Store buffer in sample data
        for (int j = 0; j < numBytesToRead; j++) {
            sampleData[i] = buffer[j];
            i++;
        }
    }
    
    if (!wavReader->finishReading()) {
        fprintf(stderr, "readFileIncrementally(): Problem finishing reading.\n");
        return false;
    }
    
    
    //If file metadata or content doesn't match expectations
    if (!validates(ifps, VALIDATION_SOURCE_SAMPLEDATA)) {
        fprintf(stderr, "testReadFileIncrementally(): Data or metadata doesn't validate.\n");
        return false;
    }
    
    return true;
}



bool WavReaderTester::testReadFileToInt16s(const InFileParamSetDef* ifps)  {
    
    const char* fileName = ifps->fileName;
    
    char inFilePath[MAX_PATH_LENGTH];
    sprintf(inFilePath,
            "%s/%s",
            inDirPath,
            fileName);
        
    if (!wavReader->initialize(inFilePath)) {
        fprintf(stderr, "readDataToInt16s(): Unable to initialize wavReader.\n");
        return false;
    }
    
    if (!wavReader->prepareToRead()) {
        fprintf(stderr, "readDataToInt16s(): Problem starting writing.\n");
        return false;
    }

    //Allocate int16 samples
    const size_t numInt16SampleBytes = wavReader->getNumSamples() * wavReader->getNumChannels() * 2; //2 bytes in int16
    if (int16Samples) {
        free(int16Samples);
        int16Samples = nullptr;
    }
    int16Samples = (int16_t*) malloc(numInt16SampleBytes);
    
    
    if (!wavReader->readDataToInt16s(int16Samples, wavReader->getNumSamples())) {
        fprintf(stderr, "readDataToInt16s(): Problem reading data.\n");
        return false;
    }

    if(!wavReader->finishReading()) {
        fprintf(stderr, "readFileFromInt16s(): Problem finishing reading.\n");
        return false;
    }
    
    //If file metadata or content doesn't match expectations
    if (!validates(ifps, VALIDATION_SOURCE_INT16SAMPLES)) {
        fprintf(stderr, "testReadFileIncrementally(): Data or metadata doesn't validate.\n");
        return false;
    }
    
    return true;
}



bool WavReaderTester::validates(const InFileParamSetDef* ifps, ValidationSource validationSource) {

    const char* fileName = ifps->fileName;
    
    //Validate metadata
    
    if (ifps->numChannels != wavReader->getNumChannels()) {
        fprintf(stderr, "Error: Expected and actual numChannels for %s don't match.\n", fileName);
        return false;
    }

    if (ifps->byteDepth != wavReader->getByteDepth()) {
       fprintf(stderr, "Error: Expected and actual byteDepth for %s don't match.\n", fileName);
       return false;
    }
    
    if (ifps->samplesAreInts != wavReader->getSamplesAreInts()) {
       fprintf(stderr, "Error: Expected and actual samplesAreInts for %s don't match.\n", fileName);
       return false;
    }
    
    
    //Validate content
    
    // Verify that all files contain ~440Hz (sine) waveform - in both channels, if there are two channels
    
    int16_t sampleCh1 = 0;
    int16_t sampleCh2 = 0;
    int16_t prevSampleCh1 = 0;
    int16_t prevSampleCh2 = 0;
    uint32_t positiveZeroCrossCountCh1 = 0;
    uint32_t positiveZeroCrossCountCh2 = 0;
    uint32_t numSamples = wavReader->getNumSamples();
    uint32_t numChannels = wavReader->getNumChannels();
    uint32_t sampleDataSize = wavReader->getNumSamples() * wavReader->getNumChannels() *  wavReader->getByteDepth();
    for (uint32_t i=0; i<numSamples; i++) {
        
        prevSampleCh1 = sampleCh1;
        prevSampleCh2 = sampleCh2;
        
        if (validationSource == VALIDATION_SOURCE_SAMPLEDATA) {
            wavReader->readInt16SampleFromArray(sampleData, //wav-format sample data
                                                sampleDataSize,
                                                i,
                                                sampleCh1,
                                                sampleCh2);
        }
        else { // validationSource == VALIDATION_SOURCE_INT16SAMPLES
            sampleCh1 = int16Samples[i*numChannels];
            sampleCh2 = (numChannels == 1) ? 0 : int16Samples[i*numChannels+1];
        }
        
        if (prevSampleCh1 < 0 && sampleCh1 >= 0) {
            positiveZeroCrossCountCh1++;
        }
        if (prevSampleCh2 < 0 && sampleCh2 >= 0) {
            positiveZeroCrossCountCh2++;
        }
    }
    
    double seconds = ((double)numSamples / (double)SAMPLE_RATE);
    double cyclesPerSecondCh1 = (double)positiveZeroCrossCountCh1 / seconds;
    double cyclesPerSecondCh2 = (double)positiveZeroCrossCountCh2 / seconds;
    
    //printf("%f %f\n", cyclesPerSecondCh1, cyclesPerSecondCh2 );

    double epsilon = 80.0;
    if (cyclesPerSecondCh1 < SINE_FREQUENCY - epsilon || cyclesPerSecondCh1 > SINE_FREQUENCY + epsilon) {
       fprintf(stderr, "Error: channel1 frequency (%.0fHz) isn't within +/-%.0fHz of %.0fHz, for %s.\n",
               ceil(cyclesPerSecondCh1),
               epsilon,
               SINE_FREQUENCY,
               fileName);
       return false;
    }
    if (numChannels == 2) {
        if (cyclesPerSecondCh2 < SINE_FREQUENCY - epsilon || cyclesPerSecondCh2 > SINE_FREQUENCY + epsilon) {
            fprintf(stderr, "Error: channel2 frequency (%.0fHz) isn't within +/-%.0fHz of %.0fHz, for %s.\n",
                    ceil(cyclesPerSecondCh2),
                    epsilon,
                    SINE_FREQUENCY,
                    fileName);
            return false;
        }
    }

    return true;
}
