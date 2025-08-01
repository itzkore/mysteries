#include <juce_audio_processors/juce_audio_processors.h>

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Main Display Parameters - Essential controls for the main screen
    params.push_back(std::make_unique<juce::AudioParameterFloat>("masterVolume", "Master Volume", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterCutoff", "Filter Cutoff", 20.0f, 20000.0f, 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("filterResonance", "Filter Resonance", 0.1f, 10.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("attackTime", "Attack Time", 0.001f, 5.0f, 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("releaseTime", "Release Time", 0.01f, 10.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("performanceX", "Performance X", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("performanceY", "Performance Y", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("presetSelector", "Preset", juce::StringArray{"Dark Void", "Synthwave Pulse", "Ambient Drone", "Experimental Texture", "Deep Space"}, 0));
    
    // Oscillator Section - Advanced parameters
    params.push_back(std::make_unique<juce::AudioParameterChoice>("oscWave", "Oscillator Wave", juce::StringArray{"Saw", "Square", "Triangle", "Noise"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("oscDetune", "Detune", -50.0f, 50.0f, 0.0f));
    
    // Industrial Effects
    params.push_back(std::make_unique<juce::AudioParameterFloat>("distortion", "Industrial Distortion", 0.0f, 10.0f, 2.0f));
    
    // Macros - Advanced modulation system
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macroTension", "Tension", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macroShadow", "Shadow", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macroGhost", "Ghost", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macroHeat", "Heat", 0.0f, 1.0f, 0.5f));
    
    return { params.begin(), params.end() };
}
