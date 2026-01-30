#pragma once 

#include "../PluginProcessor.h"

class invisibleButton : public juce::Button{
    public:
        invisibleButton();    
        void paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown) override;
};

class knobLookAndFeel : public juce::LookAndFeel_V4{
    void drawRotarySlider(juce::Graphics& g, int x, int	y, int width, int height, float	sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider);
};

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

    knobLookAndFeel style;

    juce::Slider gainSlider; 
    juce::Slider panSlider; 
    juce::Slider pitchSlider; 
    juce::Slider startSlider; 
    juce::Slider endSlider; 
    juce::Slider attackSlider; 
    juce::Slider decaySlider; 

    juce::Slider gainModSlider; 
    juce::Slider startModSlider; 

    juce::Slider gainRndSlider; 
    juce::Slider panRndSlider; 
    juce::Slider pitchRndSlider; 

    juce::Label gainLabel;
    juce::Label panLabel;
    juce::Label pitchLabel;
    juce::Label startLabel;
    juce::Label endLabel;
    juce::Label attackLabel;
    juce::Label decayLabel;

    juce::Label velocityModLabel;
    juce::Label rndModLabel;

    juce::Label gainRndLabel;
    juce::Label panRndLabel;
    juce::Label pitchRndLabel;

    juce::Label gainModLabel;
    juce::Label startModLabel;

    juce::TextButton killAll{"PANIC!"};
    juce::ToggleButton monoButton{"MONO"};
    juce::ToggleButton modeButton{"MOD"};
    juce::ToggleButton clipButton{"CLIP"};
    invisibleButton github;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> startAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> endAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainModAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> startModAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainRndAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> panRndAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pitchRndAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> clipAttachment;

    void changeSliders(juce::Slider& slider, double start, double end);
    void changeLabels(juce::Label& label, juce::String text);

    void updateAttachments(int selectedPadIndex);
    void setMode(int mode);

    juce::Image waveformCache;
    std::unique_ptr<juce::Drawable> githubSvg;
    std::unique_ptr<juce::Drawable> logoSvg;
    void updateWaveformCache();

    int minLength;
    float decayRate;

    float radius;
    double scale;
    int cursor;

    int highX;
    int highY;
    int counter;

    std::vector<std::unique_ptr<invisibleButton>> padButtons;

    std::vector<juce::Rectangle<int>> rects;
    std::vector<float> flashes;

    std::atomic<int> selectedPad;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};

