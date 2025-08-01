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
    
    // Main Tab Macro Controls - 4 macro controls for the main interface
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro1", "Macro 1", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro2", "Macro 2", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro3", "Macro 3", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("macro4", "Macro 4", 0.0f, 1.0f, 0.0f));
    
    // Synth Engine 1 Layer Parameters - Enabled by default for ambient pads
    // Oscillator Layer
    params.push_back(std::make_unique<juce::AudioParameterBool>("osc1Enable", "Oscillator Enable", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Level", "Oscillator Level", 0.0f, 1.0f, 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Pan", "Oscillator Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("osc1Waveform", "Oscillator Waveform", juce::StringArray{"Saw", "Square", "Triangle", "Sine"}, 2)); // Default to Triangle
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Detune", "Oscillator Detune", -50.0f, 50.0f, 5.0f)); // Default 5 cents detune
    params.push_back(std::make_unique<juce::AudioParameterFloat>("osc1Octave", "Oscillator Octave", -3.0f, 3.0f, 0.0f));
    
    // Sub Oscillator Layer - Enable for ambient pads
    params.push_back(std::make_unique<juce::AudioParameterBool>("subEnable", "Sub Oscillator Enable", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("subLevel", "Sub Level", 0.0f, 1.0f, 0.3f)); // Subtle level
    params.push_back(std::make_unique<juce::AudioParameterFloat>("subPan", "Sub Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("subWaveform", "Sub Waveform", juce::StringArray{"Saw", "Square", "Triangle", "Sine"}, 3));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("subOctaveOffset", "Sub Octave Offset", -2.0f, 1.0f, -1.0f));
    
    // Noise Layer - Enable for atmospheric texture
    params.push_back(std::make_unique<juce::AudioParameterBool>("noiseEnable", "Noise Enable", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("noiseLevel", "Noise Level", 0.0f, 1.0f, 0.2f)); // Subtle 20%
    params.push_back(std::make_unique<juce::AudioParameterFloat>("noisePan", "Noise Pan", -1.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("noiseType", "Noise Type", juce::StringArray{"White", "Pink", "Brown", "Blue"}, 1)); // Default to Pink
    params.push_back(std::make_unique<juce::AudioParameterFloat>("noiseFilterCutoff", "Noise Filter Cutoff", 100.0f, 20000.0f, 3000.0f)); // 3kHz for air
    
    // Sampler Layer
    params.push_back(std::make_unique<juce::AudioParameterBool>("samplerEnable", "Sampler Enable", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("samplerLevel", "Sampler Level", 0.0f, 1.0f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("samplerPan", "Sampler Pan", -1.0f, 1.0f, 0.0f));
    
    return { params.begin(), params.end() };
}
