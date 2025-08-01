#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

class ADSRComponent : public juce::Component, private juce::Timer
{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& apvts, const juce::String& prefix);
    ~ADSRComponent() override;
    
    void resized() override;
    void paint(juce::Graphics& g) override;
    
    void setVisible(bool shouldBeVisible) override;
    
private:
    void timerCallback() override;
    void updateEnvelopeDisplay();
    
    juce::AudioProcessorValueTreeState& apvts;
    juce::String parameterPrefix;
    
    // ADSR Sliders
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    juce::Label titleLabel;
    
    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    
    // Envelope visualization
    juce::Path envelopePath;
    std::vector<juce::Point<float>> envelopePoints;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
