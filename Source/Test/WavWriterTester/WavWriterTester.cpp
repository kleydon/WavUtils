
//WavWriterTester.cpp


#include "WavWriterTester.hpp"

#include "WavHeader.hpp" // Verifies that float and double correspond to f32 and f64 values
#include "WavWriter.hpp"

#include <cmath> // M_PI
#include <cstring>



WavWriterTester::WavWriterTester() {
    outDirPath = nullptr;
    wavWriter = new WavWriter();
}



WavWriterTester::~WavWriterTester() {
    if (wavWriter) {
        delete wavWriter;
        wavWriter = nullptr;
    }
}



bool WavWriterTester::initialize(const char *outDirPath) {
     
    //printf("Initializing WavWriterTester.\n\n");
    
    //Error-check outDirPath
    if (!outDirPath) {
        fprintf(stderr, "Error: Output directory path is NULL.\n");
        return false;
    }
    char tempOutFilePath[MAX_PATH_LENGTH];
    sprintf(tempOutFilePath, "%s/WavWriterTesterTempFile.txt", outDirPath);
    FILE* fp = fopen(tempOutFilePath, "w");
    if (!fp) {
        fprintf(stderr, "Error: Invalid output directory path.\n");
        return false;
    }
    if (remove(tempOutFilePath) != 0) {
        fprintf(stderr, "Error: Unable to remove test file created to validate output directory path.\n");
        fclose(fp);
        return false;
    }
    fclose(fp);
    
    this->outDirPath = outDirPath;
   
    //Initialize sample arrays
    
    //int8 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        uint8_t sample = (uint8_t) ((127.0 * sin( (2 * M_PI * i) / 100.0)) + 127.0);
        uint8Samples1Ch[i] = sample;
        uint8Samples2Ch[i*2] = sample;
        uint8Samples2Ch[i*2 + 1] = sample;
    }
    
    //int16 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        int16_t sample = (int16_t) (32767.0 * sin( (2 * M_PI * i) / 100.0 ));
        int16Samples1Ch[i] = sample;
        int16Samples2Ch[i*2] = sample;
        int16Samples2Ch[i*2 + 1] = sample;
    }
    
    //int24 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        int32_t sample = (int32_t) (8388607.0 * sin( (2 * M_PI * i) / 100.0 ));
        int24Samples1Ch[i] = sample;
        int24Samples2Ch[i*2] = sample;
        int24Samples2Ch[i*2 + 1] = sample;
    }
    
    //int32 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        int32_t sample = (int32_t) (2147483647.0 * sin( (2 * M_PI * i) / 100.0 ));
        int32Samples1Ch[i] = sample;
        int32Samples2Ch[i*2] = sample;
        int32Samples2Ch[i*2 + 1] = sample;
    }
    
    //float32 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        float sample = (float) sin( (2 * M_PI * i) / 100.0 );
        float32Samples1Ch[i] = sample;
        float32Samples2Ch[i*2] = sample;
        float32Samples2Ch[i*2 + 1] = sample;
    }
    
    //float64 1 & 2 channel
    for (int i=0; i<NUM_SAMPLES; i++) {
        double sample = (double) sin( (2 * M_PI * i) / 100.0 );
        float64Samples1Ch[i] = sample;
        float64Samples2Ch[i*2] = sample;
        float64Samples2Ch[i*2 + 1] = sample;
    }
    
    return true;
}



bool WavWriterTester::runWavWriterTest() {
    
    printf("Running WavWriterTest.\n");
    
    //Write files all at once
    printf("    Writing files, all-at-once...\n");
    for (int i=0; i<NUM_FILE_PARAM_SETS; i++) {
        if (!writeFileAllAtOnce(&outFileParamSets[i])) {
            fprintf(stderr, "runWavWriterTest(): Problem writing file all-at-once.\n");
            return false;
        }
    }
    
    //Write files incrementally
    printf("    Writing files, incrementally...\n");
    for (int i=0; i<NUM_FILE_PARAM_SETS; i++) {
        if (!writeFileIncrementally(&outFileParamSets[i])) {
            fprintf(stderr, "runWavWriterTest(): Problem writing file incrementally.\n");
            return false;
        }
    }
    
    //Write files from int16 sample arrays
    printf("    Writing files from int16s...\n");
    for (int numChannels=1; numChannels<3; numChannels++) {
        if (!writeFileFromInt16s(numChannels)) {
            fprintf(stderr, "runWavWriterTest(): Problem writing file from int16s.\n");
            return false;
        }
    }
    
    printf("Done WavWriterTest.\n");

    printf("    To verify written files, check contents of output directory:\n    %s/\n\n", outDirPath);

    return true;
}



bool WavWriterTester::writeFileAllAtOnce(const OutFileParamSetDef* ofps) {
    
    uint32_t numChannels = ofps->numChannels;
    uint32_t byteDepth = ofps->byteDepth;
    bool samplesAreInts = ofps->samplesAreInts;
    
    char outFilePath[MAX_PATH_LENGTH];
    sprintf(outFilePath,
            "%s/fullwrite-%d%s%dch.wav",
            outDirPath,
            byteDepth*8,
            (samplesAreInts)? "i" : "f",
            numChannels);
    
    if(!setSampleData(numChannels, byteDepth, samplesAreInts)) {
        fprintf(stderr, "writeFileAllAtOnce(): Problem setting sampleData.\n");
        return false;
    }
    
    if(!wavWriter->initialize(outFilePath,
                              SAMPLE_RATE,
                              numChannels,
                              samplesAreInts,
                              byteDepth)) {
        fprintf(stderr, "writeFileAllAtOnce(): Problem initializing WavWriter.\n");
        return false;
    }
    
    if (!wavWriter->startWriting()) {
        fprintf(stderr, "writeFileAllAtOnce(): Problem starting writing.\n");
        return false;
    }
    
    if(!wavWriter->writeData(sampleData, NUM_SAMPLES*numChannels*byteDepth)) {
        fprintf(stderr, "writeFileAllAtOnce(): Problem writing data.\n");
        return false;
    }
    
    if(!wavWriter->finishWriting()) {
       fprintf(stderr, "writeFileAllAtOnce(): Problem finishing writing.\n");
        return false;
    }
    
    return true;
}



bool WavWriterTester::writeFileIncrementally(const OutFileParamSetDef* ofps) {
    
    uint32_t numChannels = ofps->numChannels;
    uint32_t byteDepth = ofps->byteDepth;
    bool samplesAreInts = ofps->samplesAreInts;
    
    char outFilePath[MAX_PATH_LENGTH];
    sprintf(outFilePath,
            "%s/incrwrite-%d%s%dch.wav",
            outDirPath,
            byteDepth*8,
            (samplesAreInts)? "i" : "f",
            numChannels);
    
    uint32_t SAMPLE_DATA_SIZE = (NUM_SAMPLES * numChannels * byteDepth);
    uint32_t BUFFER_SIZE = numChannels *  byteDepth * 20; //Pick a size that falls on even sample block boundary
    uint8_t buffer[BUFFER_SIZE];
    
    if (!setSampleData(numChannels, byteDepth, samplesAreInts)) {
        fprintf(stderr, "writeFileIncrementally(): Problem setting sampleData.\n");
        return false;
    }
    
    if (!wavWriter->initialize(outFilePath,
                          SAMPLE_RATE,
                          numChannels,
                          samplesAreInts,
                              byteDepth)) {
        fprintf(stderr, "writeFileIncrementally(): Problem initializing WavWriter.\n");
        return false;
    }
    
    if (!wavWriter->startWriting()) {
        fprintf(stderr, "writeFileIncrementally(): Problem starting writing.\n");
        return false;
    }
    
    uint32_t i = 0;
    while (i < SAMPLE_DATA_SIZE) {
        
        //Fill a buffer
        uint32_t j = 0;
        while (j < BUFFER_SIZE && i < SAMPLE_DATA_SIZE) {
            buffer[j] = sampleData[i];
            j++; i++;
        }
        
        //Write buffer to file
        if(!wavWriter->writeData(buffer, j)) {
            fprintf(stderr, "writeFileIncrementally(): Problem writing data.\n");
            return false;
        }
    }
    
    if (!wavWriter->finishWriting()) {
        fprintf(stderr, "writeFileIncrementally(): Problem finishing writing.\n");
        return false;
    }
    
    return true;
}



bool WavWriterTester::writeFileFromInt16s(uint32_t numChannels) {
    
    char outFilePath[MAX_PATH_LENGTH];
    sprintf(outFilePath,
            "%s/int16write-%dch.wav",
            outDirPath,
            numChannels);
        
    uint32_t byteDepth = 2;
    bool samplesAreInts = true;
    int16_t* int16Samples = (numChannels==1) ? int16Samples1Ch : int16Samples2Ch;

    if (!wavWriter->initialize(outFilePath,
                               SAMPLE_RATE,
                               numChannels,
                               samplesAreInts,
                               byteDepth)) {
        fprintf(stderr, "writeFileFromInt16s(): Unable to initialize wavWriter.\n");
        return false;
    }
    
    if (!wavWriter->startWriting()) {
        fprintf(stderr, "writeFileFromInt16s(): Problem starting writing.\n");
        return false;
    }
    
    if (!wavWriter->writeDataFromInt16s(int16Samples, NUM_SAMPLES)) {
        fprintf(stderr, "writeFileFromInt16s(): Problem writing data.\n");
        return false;
    }
    
    if(!wavWriter->finishWriting()) {
        fprintf(stderr, "writeFileFromInt16s(): Problem finishing writing.\n");
        return false;
    }
    
    return true;
}



bool WavWriterTester::setSampleData(uint32_t numChannels, uint32_t byteDepth, bool samplesAreInts) {
    
    //Set source
    uint8_t* srcSampleBytes = nullptr;
    if (samplesAreInts) {
        switch (byteDepth) {
            case 1:
                srcSampleBytes = (numChannels==1) ? (uint8_t*)uint8Samples1Ch : (uint8_t*)uint8Samples2Ch;
                break;
            
            case 2:
                srcSampleBytes = (numChannels==1) ? (uint8_t*)int16Samples1Ch : (uint8_t*)int16Samples2Ch;
                break;

            case 3:
                srcSampleBytes = (numChannels==1) ? (uint8_t*)int24Samples1Ch : (uint8_t*)int24Samples2Ch;
                break;

            case 4:
                srcSampleBytes = (numChannels==1) ? (uint8_t*)int32Samples1Ch : (uint8_t*)int32Samples2Ch;
                break;

            default:
                fprintf(stderr, "Should never reach here.");
                return false;
                break;
        }
    }
    else { //floating-point
        if (byteDepth == 4) {
            srcSampleBytes = (numChannels==1) ? (uint8_t*)float32Samples1Ch : (uint8_t*)float32Samples2Ch;
        }
        else { //byteDepth == 8
            srcSampleBytes = (numChannels==1) ? (uint8_t*)float64Samples1Ch : (uint8_t*)float64Samples2Ch;
        }
    }
    
    uint32_t destBlockSize = numChannels * byteDepth;
    uint32_t containerSize = (byteDepth==3 && samplesAreInts) ? byteDepth + 1 : byteDepth;
    for (int i=0; i<NUM_SAMPLES; i++) {
        uint8_t* bytes = &srcSampleBytes[i*numChannels*containerSize];
        uint32_t blockIndex = i * destBlockSize;
        int k = 0;
        for (int j=0; j<destBlockSize; j++) {
            if (byteDepth==3 && samplesAreInts && k == 3) { //Handle edge case of 2ch int24, in int32 containers; skip padding byte
                k++;
            }
            sampleData[blockIndex + j] = bytes[k];
            k++;
        }
    }
    
    return true;
}

