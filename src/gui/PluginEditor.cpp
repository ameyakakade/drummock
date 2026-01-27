#include "../PluginProcessor.h"
#include "PluginEditor.h"

namespace Style {
    const auto mainBackground     = juce::Colour(0xff1E1E2E); 
    const auto thumbBackground    = juce::Colour(0xff551E2E); 
    const auto accentColor        = juce::Colour(0xffF5C2E7); 
    const auto thumbColor1        = juce::Colour(0xff89DCEB);
    const auto thumbColor2        = juce::Colour(0xff94E2D5);
    const auto thumbOverlayColor  = juce::Colour(0xff313244);
    const auto playHeadColor      = juce::Colour(0xffF38BA8);
    const auto flashColor         = juce::Colour(0xffF9E2AF);
    const auto knobTextColor      = juce::Colour(0xffCDD6F4);
    const auto knobTextColorActive= juce::Colour(0xffA6E3A1);
    const auto knobMain           = juce::Colour(0xffBAC2DE);
    const auto transparent        = juce::Colour(0x00000000);
}

invisibleButton::invisibleButton() : juce::Button("invisible_button"){

}


void invisibleButton::paintButton (juce::Graphics& g, bool isMouseOver, bool isButtonDown){

}

void knobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider){
    g.setColour(Style::knobMain);
    
    auto pi = juce::MathConstants<float>::pi;
    auto twoPi = juce::MathConstants<float>::twoPi; // Very useful for circles

    int border_width = slider.isMouseOverOrDragging() ? 8.0 : 3.0;
    int diameter = std::min(width, height) - border_width*2;
    int middleX = width/2;
    int middleY = height/2;
    int left = x + middleX;
    int top = y + middleY;

    juce::Path arc;

    arc.addCentredArc(middleX, middleY, diameter/2, diameter/2, 0, rotaryStartAngle, rotaryStartAngle+(sliderPosProportional*(rotaryEndAngle-rotaryStartAngle)), true);

    g.strokePath(arc, juce::PathStrokeType(border_width));

    slider.isMouseOverOrDragging() ? g.setColour(Style::knobTextColorActive) : g.setColour(Style::knobTextColor);    

    // g.setFont(); 
    
    juce::String value(slider.getValue(), 2, false);

    g.drawText(value, x, y, width, height, juce::Justification::centred, false);

}

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    minLength = 1024*4; //minimum length for playhead to be drawn
    decayRate = 0.7f;
    flashes.resize(8);

    radius = 10.0;

    int x = 30;
    int y = 30;
    for(int i=0; i<8; i++){
        juce::Rectangle<int> rect(x, y ,150, 150);
        rects.push_back(rect);
        x += 180;
        if(x > 620){
            y += 180; x = 30;
        }
        padButtons.emplace_back(std::make_unique<invisibleButton> ());
    }

    updateAttachments(0);

    for(int i=0; i<8; i++){
        addAndMakeVisible(*padButtons[i]);
        padButtons[i]->onClick = [this, i]{updateAttachments(i);};
    }

    changeSliders(gainSlider, 0.0, 1.0);
    changeSliders(pitchSlider, 0.0, 1.0);
    changeSliders(startSlider, 0.0, 1.0);
    changeSliders(endSlider, 0.0, 1.0);
    changeSliders(attackSlider, 0.0, 1.0);
    changeSliders(decaySlider, 0.0, 1.0);

    changeLabels(gainLabel, "Volume");
    changeLabels(pitchLabel, "Pitch");
    changeLabels(startLabel, "Start");
    changeLabels(endLabel, "End");
    changeLabels(attackLabel, "Attack");
    changeLabels(decayLabel, "Decay");

    addAndMakeVisible (gainSlider);
    addAndMakeVisible (pitchSlider);
    addAndMakeVisible (startSlider);
    addAndMakeVisible (endSlider);
    addAndMakeVisible (attackSlider);
    addAndMakeVisible (decaySlider);

    addAndMakeVisible(gainLabel);
    addAndMakeVisible(pitchLabel);
    addAndMakeVisible(startLabel);
    addAndMakeVisible(endLabel);
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);

    setSize (750, 540);
    startTimerHz(60);
    
    highX = highY = -1;

}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{

}

void AudioPluginAudioProcessorEditor::updateWaveformCache(){
    
    waveformCache = juce::Image(juce::Image::ARGB, getWidth(), getHeight(), true);

    juce::Graphics g(waveformCache);

    for(int i=0; i<8; i++){
        g.saveState();

        auto& thumb = processorRef.thumbs[i];
        juce::Rectangle<int> rect = rects[i];
        juce::Path roundedPath;
        roundedPath.addRoundedRectangle(rect, radius);
        g.reduceClipRegion(roundedPath);

        g.setColour(Style::thumbBackground);
        g.fillRoundedRectangle(rect.toFloat(), radius);

        juce::ColourGradient waveformGrad (
                juce::Colour(Style::thumbColor1),         (float)rect.getX(), (float)rect.getY(),          // Top Color
                juce::Colour(Style::thumbColor2),         (float)rect.getX(), (float)rect.getBottom(),     // Bottom Color
                false
            );
            // Add a "Hot" center point for extra 3D pop
            waveformGrad.addColour(0.5, juce::Colours::white); 
    
        g.setGradientFill(waveformGrad); // Apply the gradient brush

        thumb->drawChannels(g, rect, 0, thumb->getTotalLength(), 1);

        g.setColour(Style::thumbBackground.withAlpha(0.2f));
        for (int y = rect.getY(); y < rect.getBottom(); y += 3){
            g.drawHorizontalLine(y, (float)rect.getX(), (float)rect.getRight());
        }

        g.setColour (Style::thumbOverlayColor);
        g.setOpacity(0.85f);
        float s = processorRef.start[i]->load(std::memory_order_relaxed);
        float e = processorRef.end[i]->load(std::memory_order_relaxed);
        g.fillRect(rect.getX(), rect.getY(), int(rect.getWidth()*s), rect.getHeight());
        g.fillRect(int(rect.getX()+(rect.getWidth()*e)), rect.getY(), int(rect.getWidth()*(1-e)+1), rect.getHeight());
        g.restoreState();
    }
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (Style::mainBackground);

    if (waveformCache.isValid())
        g.drawImageAt(waveformCache, 0, 0);
    else
        updateWaveformCache(); // Safety fallback


    for(int t=0; t<30; t++){
        g.saveState();
        if(processorRef.pool.states[t]->state.load(std::memory_order_relaxed)){
            int len = processorRef.pool.states[t]->length.load(std::memory_order_relaxed);
            int posi = processorRef.pool.states[t]->position.load(std::memory_order_relaxed);
            int id = processorRef.pool.states[t]->id.load(std::memory_order_relaxed);
            juce::Path roundedPath;
            roundedPath.addRoundedRectangle(rects[id], radius);
            g.reduceClipRegion(roundedPath);
            g.setColour (Style::playHeadColor);
            if(len>minLength){
                float offset = (float)posi/(float)len*150.0f;
                if(offset<150){
                    g.setOpacity(1-(offset/300.0));
                    float x = rects[id].getX();
                    float y = rects[id].getY();
                    g.drawLine(x + offset, y, x + offset, y + 150, 2);
                } 
            }
            g.restoreState();
        }
        g.setOpacity(1);
    }

    for(int i=0; i<flashes.size(); i++){
        bool flag = processorRef.padStates[i]->load(std::memory_order_relaxed);
        g.saveState();
        juce::Path roundedPath;
        roundedPath.addRoundedRectangle(rects[i], radius);
        g.reduceClipRegion(roundedPath);
        if(flag){
            flashes[i] = 1.0f;
            processorRef.padStates[i]->store(false, std::memory_order_relaxed);
        }
        else{
            g.setColour (Style::flashColor);
            g.setOpacity(flashes[i]);
            g.fillRoundedRectangle(rects[i].toFloat(), 5.0);
            flashes[i] *= decayRate;
        }
        g.restoreState();
    }

    if(highX!=-1 or highY!=-1){
        g.setColour (Style::accentColor);
        g.setOpacity(0.8);
        int r = 40;
        g.fillEllipse(highX-(r/2), highY-(r/2), r, r);
    }
    
    g.setColour (Style::accentColor);
    g.drawRoundedRectangle(rects[selectedPad.load(std::memory_order_relaxed)].toFloat(), radius, 2.0);

    g.setOpacity(1);
}

void AudioPluginAudioProcessorEditor::resized()
// TRANSLATION: "I am defining the function that runs when the window size changes."
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(75, 400, 90, 110);
    pitchSlider.setBounds(175, 400, 90, 110);
    startSlider.setBounds(275, 400, 90, 110);
    endSlider.setBounds(375, 400, 90, 110);
    attackSlider.setBounds(475, 400, 90, 110);
    decaySlider.setBounds(575, 400, 90, 110);

    gainLabel.setBounds(75,380,90,20);
    pitchLabel.setBounds(175,380,90,20);
    startLabel.setBounds(275,380,90,20);
    endLabel.setBounds(375,380,90,20);
    attackLabel.setBounds(475,380,90,20);
    decayLabel.setBounds(575,380,90,20);
    
    for(int i=0; i<8; i++){
        padButtons[i]->setBounds(rects[i]);
    }
}


void AudioPluginAudioProcessorEditor::timerCallback(){
    repaint();
}




bool AudioPluginAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray &files){
    return true;
}

void AudioPluginAudioProcessorEditor::fileDragEnter(const juce::StringArray &files, int x, int y){
}

void AudioPluginAudioProcessorEditor::fileDragMove(const juce::StringArray &files, int x, int y){
    highX = x;
    highY = y;
}

void AudioPluginAudioProcessorEditor::fileDragExit(const juce::StringArray &files){
    highX = -1;
    highY = -1;
}

void AudioPluginAudioProcessorEditor::filesDropped(const juce::StringArray &files, int x, int y){
    highX = -1;
    highY = -1;
    int id = -1;
    int s = files.size();
    for(int i=0; i<8; i++){
        auto& rect = rects[i];
        int rx = rect.getX();
        int ry = rect.getY();
        int h = rect.getHeight();
        int w = rect.getWidth();
        if((x-rx<w) and (y-ry<h) and (x-rx>0) and (y-ry>0)) id = i;
    }
    if(id!=-1){
        for(int i=0; i<s; i++){
            std::string path = files[i].toStdString();
            processorRef.updateFile(path, id);
            id++;
            if(id>=8) break;
        }
    }
}


void AudioPluginAudioProcessorEditor::updateAttachments(int selectedPadIndex){
    
    gainAttachment.reset();
    pitchAttachment.reset();
    startAttachment.reset();
    endAttachment.reset();
    attackAttachment.reset();
    decayAttachment.reset();

    selectedPad.store(selectedPadIndex);
    int i = selectedPad.load();

    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "Gain"+std::to_string(i), gainSlider); 
    pitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "Pitch"+std::to_string(i), pitchSlider); 
    startAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "Start"+std::to_string(i), startSlider); 
    endAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "End"+std::to_string(i), endSlider); 
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "Attack"+std::to_string(i), attackSlider); 
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.states, "Decay"+std::to_string(i), decaySlider); 
}


void AudioPluginAudioProcessorEditor::changeSliders(juce::Slider& slider, double start, double end){
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setRange (start, end, 0.01); // Min, Max, Step Size
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setColour(juce::Slider::textBoxOutlineColourId, Style::transparent);
    slider.setLookAndFeel(&style);
}

void AudioPluginAudioProcessorEditor::changeLabels(juce::Label& label, juce::String text){
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
}
