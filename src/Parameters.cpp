#include <JuceHeader.h>

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    // Oscillator Section
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC_WAVE", "Oscillator Wave", juce::StringArray{"Saw", "Square", "Triangle", "Noise"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC_DETUNE", "Detune", -50.0f, 50.0f, 0.0f));
    // Dark Filter Section
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff", 20.0f, 20000.0f, 1000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_RESONANCE", "Resonance", 0.0f, 10.0f, 1.0f));
    // Industrial Effects
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DISTORTION", "Industrial Distortion", 0.0f, 10.0f, 2.0f));
    // Macros
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TENSION", "Tension", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SHADOW", "Shadow", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GHOST", "Ghost", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HEAT", "Heat", 0.0f, 1.0f, 0.5f));
    return { params.begin(), params.end() };
}
