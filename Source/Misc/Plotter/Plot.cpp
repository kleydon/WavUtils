//Plot.cpp

#include <iostream>


void int16PrintToPlot(const int16_t int16Samples[], //Interleaved; numSamples * nunChannels long
                      int32_t numSamples,
                      int32_t numSamplesToPrint,
                      int32_t numChannels) {
    
    printf("\n\nimport numpy as np\n");
    printf("import matplotlib.pyplot as plt\n");
    printf("ch1 = [\n");
    
    for (int i=0; i < numSamplesToPrint; i++) {
        printf("%d,", int16Samples[i * numChannels]);
        printf("\n");
    }
    
    printf("]\n");
     
    if (numChannels == 2) {
        
        printf("ch2 = [\n");

        for (int i=0; i< numSamplesToPrint; i++) {
            printf("%d,", int16Samples[i * numChannels + 1]);
            printf("\n");
        }
        printf("]\n");
        
        printf("plt.plot(ch2[0:%d], '.')\n", numSamplesToPrint);
    }
     
    printf("plt.plot(ch1[0:%d])\n", numSamplesToPrint);
    
    printf("mx = np.ones(%d) * %d\n", numSamplesToPrint, 32767);
    printf("plt.plot(mx, '-')\n");
    
    printf("plt.show()\n\n");
}
