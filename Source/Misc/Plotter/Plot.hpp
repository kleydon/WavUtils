//Plot.hpp

#ifndef __QX_PLOT_HPP__
#define __QX_PLOT_HPP__

#include <cstdint> //For uint8_t, etc.

void int16PrintToPlot(const int16_t int16Samples[], //Interleaved; numSamples * nunChannels long
                      int32_t numSamples,
                      int32_t numSamplesToPrint,
                      int32_t numChannels);

#endif  //__QX_PLOT_HPP__
