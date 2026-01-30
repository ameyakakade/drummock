#include "Effects.h"

void distortFn (juce::AudioBuffer<float>& buffer){
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    for (int ch=0; ch<numChannels; ch++){
        auto* channelData = buffer.getWritePointer(ch);
        for(int i=0; i<numSamples; i++){
            float sample = channelData[i];
            if(sample>1.0f) channelData[i] = 1.0f;
            else if(sample<(-1.0f)) channelData[i] = -1.0f;
        }
    }
}

