# WavUtils

Classes for reading and writing Wav files in C++.
- Read or write incrementally (frame by frame), or all at once
- Support for 8/16/24/32-bit int samples, or 32/64-bit float samples
- Conversion from any of the formats above to/from 16-bit integer format
- Little-Endian - i.e: iOS, Android NDK, Windows, and OSX (post-PowerPC)

## Usage

### Read:
```C++
#include "WavReader.hpp"
...
WavReader* wr = new WavReader();
wr->initialize(inputWavFilePath);
wr->prepareToRead();  // Metadata available after this
wr->readData(sampleData, wr->getSampleDataSize());
wr->finishReading();
```

### Write:

```C++
#include "WavWriter.hpp"
...
WavWriter* ww = new WavWriter();
ww->initialize(outputWavFilePath,
               sampleRate,
               numChannels,  // 1 || 2
               samplesAreInts, // false for 32/64-bit float vals
               byteDepth);  // 1, 2, 3, 4 for int samples; 4 or 8 for float samples
ww->startWriting(); //Writes header
ww->writeData(sampleData, sampleDataSize);
ww->finishReading();
```
### Incrementally Read & Write:
```C++
...
wr->readData(bufferA, bufferASize);
wr->readData(bufferB, bufferBSize);
...
ww->writeData(bufferA, bufferASize);
ww->writeData(bufferB, bufferBSize);
...
```
### Conversion to/from Int16 Samples, during Read & Write
```C++
...
wr->readDataToInt16s(int16Samples,
                     numInt16Samples);

ww->writeDataFromInt16s(int16Samples,
                        numInt16Samples);
...
```

### In-Memory Conversion to/from Int16 Samples
```C++
...
wr->readInt16SampleFromArray(sampleData,  // Wav-format "source" sample data array
                             sampleDataSize,
                             sampleIndex,  //Index of sample to be read
                             int16SampleCh1,  // Sample's channel 1 value, as int16
                             int16SampleCh2);  // Sample's channel 2 value (if available), as int16
...
ww->writeInt16SampleToArray(int16SampleCh1,  // Sample's channel 1 value, as int16
                            int16SampleCh2,  // Sample's channel 2 value (if available), as int16
                            sampleIndex,  //Index of sample to be written
                            sampleData,  // Wav-format "target" sample data array
                            sampleDataSize);
...
```


