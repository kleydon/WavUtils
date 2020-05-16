//WavWriter.cpp


#include <cstring> //memset()


#include "WavWriter.hpp"



WavWriter::WavWriter(const char* writeFilePath,
                     uint32_t sampleRate,
                     uint32_t numSamples,
                     uint32_t numChannels,
                     bool samplesAreInts, //False if samples are 32 or 64-bit floating point values
                     uint32_t byteDepth) {
    
    init(writeFilePath,
         sampleRate,
         numSamples,
         numChannels,
         samplesAreInts, //False if samples are 32 or 64-bit floating point values
         byteDepth);
}



WavWriter::~WavWriter() {
}



void WavWriter::init(const char* writeFilePath,
                     uint32_t sampleRate,
                     uint32_t numSamples,
                     uint32_t numChannels,
                     bool samplesAreInts, //False if samples are 32 or 64-bit floating point values
                     uint32_t byteDepth) {
        
    // ****** ERROR CHECK ALL PARAMETERS BEFORE USING IN WRITE - BUT HOW TO NOT USE EXCEPTIONS? *******

    this->writeFilePath = writeFilePath;
    this->writeFile = nullptr;
    
    this->numSamplesWritten = 0;
    
    this->numSamples = numSamples;
    this->sampleRate = sampleRate;
    this->numChannels = numChannels;
    this->samplesAreInts = samplesAreInts;
    this->byteDepth = byteDepth;
}



bool WavWriter::openFile() {
    
    if (!writeFile) {
        writeFile = fopen(writeFilePath, "w+b");
        if (writeFile == NULL) {
            fprintf(stderr, "Error: Unable to open output file for writing.\n");
            writeFile = nullptr;
            return false;
        }
    }
    
    return true;
}



bool WavWriter::closeFile() {
    return closeFile(nullptr);
}



bool WavWriter::closeFile(const char* errorMessage) {
    
    if (errorMessage) {
        fprintf(stderr, "%s\n", errorMessage);
    }
    
    if (writeFile) {
        fclose(writeFile);
        writeFile = nullptr;
    }
    
    return true;
}



//Write functions



bool WavWriter::startWriting() {
        
    if (!(numChannels == 1 || numChannels == 2)) {
        closeFile("Error: Number of channels must be 1 or 2");
        return false;
    }
    
    if (sampleRate < 8000) { // Other constraints?
        closeFile("Error: Unsupported sample rate.");
        return false;
    }
    
    if ( !((samplesAreInts && (byteDepth == 1 || byteDepth == 2 || byteDepth == 3 || byteDepth == 4)) ||
          (!samplesAreInts && (byteDepth == 4 || byteDepth == 8))) ) {
        closeFile("Error: Invalid bits-per-sample value, or invalid combination of bits-per-sample and number of channels.");
        return false;
    }
    
    
    if (!openFile()) {
        return false;
    }

    //Write RIFF header
    uint8_t riffHeaderData[RIFF_HEADER_SIZE];
    RiffHeader* rh = (RiffHeader*) riffHeaderData;
    rh->chunkId[0] = 'R';
    rh->chunkId[1] = 'I';
    rh->chunkId[2] = 'F';
    rh->chunkId[3] = 'F';
    rh->fileSizeLess8 =
        4 + //End of riff header
        FORMAT_SUBCHUNK_SIZE + //Format subchunk
        ((samplesAreInts) ? 0 : FACT_SUBCHUNK_SIZE) + //Fact subchunk
        (8 + (numSamples * numChannels * byteDepth)); //Data subchunk
    //printf("fileSizeLess8:%d\n", rh->fileSizeLess8);
    rh->formatName[0] = 'W';
    rh->formatName[1] = 'A';
    rh->formatName[2] = 'V';
    rh->formatName[3] = 'E';
    size_t numToWrite = 1;
    size_t numWritten = 0;
    numWritten = fwrite(riffHeaderData, RIFF_HEADER_SIZE, 1, writeFile);
    if (numWritten < numToWrite) {
        closeFile("Error: Problem writing RIFF header.");
        return false;
    }
    
    //Write format chunk
    uint8_t formatSubchunkData[FORMAT_SUBCHUNK_SIZE];
    FormatSubchunk* fsc = (FormatSubchunk*) formatSubchunkData;
    fsc->formatSubchunkId[0] = 'f';
    fsc->formatSubchunkId[1] = 'm';
    fsc->formatSubchunkId[2] = 't';
    fsc->formatSubchunkId[3] = ' ';
    fsc->formatSubchunkSize = 16;  // ***** MAY BE DIFFERENT FOR FLOAT!!! *****
    fsc->audioFormat = (samplesAreInts) ? AUDIO_FORMAT_INT : AUDIO_FORMAT_FLOAT;
    fsc->numChannels = numChannels;
    fsc->sampleRate = sampleRate;
    fsc->byteRate = sampleRate * numChannels * byteDepth;
    fsc->blockAlign = numChannels * byteDepth;
    fsc->bitsPerSample = byteDepth * 8;
    numToWrite = 1;
    numWritten = 0;
    numWritten = fwrite(formatSubchunkData, FORMAT_SUBCHUNK_SIZE, 1, writeFile);
    if (numWritten < numToWrite) {
        closeFile("Error: Problem writing format subchunk.");
        return false;
    }
    
    //"fact" subchunk, supposedly required for floating-point representation
    //See: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
    if (!samplesAreInts) {
        //Write fact chunk
        uint8_t factSubchunkData[FACT_SUBCHUNK_SIZE];
        FactSubchunk* factsc = (FactSubchunk*) factSubchunkData;
        factsc->factSubchunkId[0] = 'f';
        factsc->factSubchunkId[1] = 'a';
        factsc->factSubchunkId[2] = 'c';
        factsc->factSubchunkId[3] = 't';
        factsc->factSubchunkSize = 4;
        factsc->numSamplesPerChannel = numSamples;
        numToWrite = 1;
        numWritten = 0;
        numWritten = fwrite(factSubchunkData, FACT_SUBCHUNK_SIZE, 1, writeFile);
        if (numWritten < numToWrite) {
            closeFile("Error: Problem writing fact subchunk.");
            return false;
        }
    }
    
    uint8_t dataSubchunkHeader[SUBCHUNK_HEADER_SIZE];
    SubchunkHeader* dsh = (SubchunkHeader*) dataSubchunkHeader;
    dsh->subchunkId[0] = 'd';
    dsh->subchunkId[1] = 'a';
    dsh->subchunkId[2] = 't';
    dsh->subchunkId[3] = 'a';
    dsh->subchunkSize = (numSamples * numChannels * byteDepth);
    numToWrite = 1;
    numWritten = 0;
    numWritten = fwrite(dataSubchunkHeader, SUBCHUNK_HEADER_SIZE, 1, writeFile);
    if (numWritten < numToWrite) {
        closeFile("Error: Problem writing data subchunk header.");
        return false;
    }
    
    return true;
}



bool WavWriter::writeData(const uint8_t sampleData[], //WAV format bytes
                          uint32_t sampleDataSize) {
    
    //Requires that:
    // 1) File is open for writing
    // 2) Header has already been written
    // 3) File pointer is at the right location for writing data
    
    //Error-checking...
    int64_t numSamplesLeftToWrite = numSamples - numSamplesWritten;
    int64_t numSamplesThisWrite = sampleDataSize / (numChannels * byteDepth);
    if ( numSamplesLeftToWrite < numSamplesThisWrite ) {
        closeFile("Error: Attempt to write too many samples.");
        return false;
    }
    
    size_t numBytesWritten = 0;
    numBytesWritten = fwrite(sampleData, 1, sampleDataSize, writeFile);
    numSamplesWritten += numBytesWritten / (byteDepth * numChannels);
    if (numBytesWritten < sampleDataSize) {
        closeFile("Error: Problem writing sample data.");
        return false;
    }
    
    return true;
}



bool WavWriter::writeDataFromInt16s(const int16_t int16Samples[], //channels interleaved; length = numInt16Samples * numChannels
                                    uint32_t numInt16Samples) {
 
    //Error checking...
    int64_t numSamplesLeftToWrite = numSamples - numSamplesWritten;
    int64_t numSamplesThisWrite = numInt16Samples;
    if ( numSamplesLeftToWrite < numSamplesThisWrite ) {
        closeFile("Error: Attempt to write too many samples.");
        return false;
    }
     
    const uint32_t bufferSize = (numChannels * byteDepth);
    uint8_t buffer[bufferSize];
    for (uint32_t i = 0; i < numInt16Samples; i++) {
        
        int16_t int16SampleCh1 = int16Samples[i*numChannels];
        int16_t int16SampleCh2 = (numChannels == 2) ? int16Samples[i*numChannels + 1] : 0;
                
        writeInt16SampleToArray(int16SampleCh1,
                                int16SampleCh2, //Irrelevant if only one channel
                                0, //sampleIndex
                                buffer,
                                bufferSize);
                
        writeData(buffer, bufferSize);  // Updates numSamplesWritten
    }
    
    return true;
}



bool WavWriter::finishWriting() {
    
    //Update file length, based on numSamplesWritten and numSamples
    
    return (closeFile());
}



bool WavWriter::writeInt16SampleToArray(int16_t int16SampleCh1,
                                        int16_t int16SampleCh2,
                                        uint32_t sampleIndex,
                                        const uint8_t sampleData[], //Wav format bytes; samples interleaved if multiple channels
                                        uint32_t sampleDataSize) {

    uint8_t srcBytes[sizeof(int16_t)*numChannels];
    ((int16_t*)srcBytes)[0] = int16SampleCh1;
    if (numChannels == 2) {
        ((int16_t*)srcBytes)[1] = int16SampleCh2;
    }
    
    uint8_t* destBytes = (uint8_t *)(sampleData + (sampleIndex * numChannels * byteDepth));
    memcpy(destBytes, srcBytes, sizeof(int16_t)*numChannels);
    
    return true;
}



//Accessors



const char* WavWriter::getWriteFilePath() {
    return writeFilePath;
}



uint32_t WavWriter::getSampleRate() {
    return sampleRate;
}



uint32_t WavWriter::getNumSamples() {
    return numSamples;
}



uint32_t WavWriter::getNumChannels() {
    return numChannels;
}



bool WavWriter::getSamplesAreInts() {
    return samplesAreInts;
}



uint32_t WavWriter::getByteDepth() {
    return byteDepth;
}



uint32_t WavWriter::getSampleDataSize() {
    return numSamples * byteDepth * numChannels;
}



