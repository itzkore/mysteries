


#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
class VoidTextureSynthAudioProcessor : public juce::AudioProcessor
{
public:
    VoidTextureSynthAudioProcessor();
    ~VoidTextureSynthAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                                   { return true; }

    const juce::String getName() const override                       { return JucePlugin_Name; }
    bool  acceptsMidi()   const override                              { return JucePlugin_WantsMidiInput;  }
    bool  producesMidi()  const override                              { return JucePlugin_ProducesMidiOutput; }
    bool  isMidiEffect()  const override                              { return JucePlugin_IsMidiEffect; }
    double getTailLengthSeconds() const override                      { return 0.0; }

    int  getNumPrograms() override                                    { return 1; }
    int  getCurrentProgram() override                                 { return 0; }
    void setCurrentProgram (int)   override                           {}
    const juce::String getProgramName (int) override                  { return {}; }
    void changeProgramName (int, const juce::String&) override        {}

    void getStateInformation (juce::MemoryBlock&) override            {}
    void setStateInformation (const void*, int) override              {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoidTextureSynthAudioProcessor)
};
