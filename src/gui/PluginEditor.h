#pragma once 

#include "../PluginProcessor.h"

class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor, public juce::Timer, public juce::FileDragAndDropTarget

{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);

    ~AudioPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    // drag and drop logic
    bool isInterestedInFileDrag(const juce::StringArray &files) override;
    void fileDragEnter(const juce::StringArray &files, int x, int y) override;
    void fileDragMove(const juce::StringArray &files, int x, int y) override;
    void fileDragExit(const juce::StringArray &files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

private:
    AudioPluginAudioProcessor& processorRef;

    juce::Slider gainSlider; 
    double gain;
    int minLength;
    float decayRate;

    int highX;
    int highY;

    std::vector<juce::Rectangle<int>> rects;
    std::vector<float> flashes;

    int selectedPad;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
