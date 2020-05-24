# WavUtils

Utilities for reading and writing wav files; all at once or incrementally.

## Usage:

```
#include "WavWriter.hpp"

*Reading*
WavReader* wr = new WavReader();
wr->initialize(inputWavFilePath);
wr->prepareToRead(); //File metadata available after this
wr->readData(sampleDataArray, wr->getSampleDataSize());
wr->finishReading();

*Writing*
WavWriter* ww = new WavWriter();
ww->initialize(outputWavFilePath);
ww->prepareToRead(); //File metadata available after this
ww->readData(sampleDataArray, wr->getSampleDataSize());
ww->finishReading();



```
