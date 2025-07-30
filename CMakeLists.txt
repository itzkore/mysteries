cmake_minimum_required(VERSION 3.22)
project(VoidTextureSynth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# JUCE submodule
add_subdirectory(ext/JUCE)

juce_add_plugin(VoidTextureSynth
    COMPANY_NAME "Voidwalker"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    PLUGIN_MANUFACTURER_CODE "VDSY"
    PLUGIN_CODE "VDS1"
    FORMATS VST3
    PRODUCT_NAME "Void Texture Synth"
)

# Source files
target_sources(VoidTextureSynth PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/GUI/VoidLookAndFeel.cpp
    src/DSP/VoidOscillator.cpp
    src/DSP/DarkFilter.cpp
    src/DSP/SynthVoice.cpp
    src/DSP/FX/ReverbFX.h
    src/DSP/FX/DelayFX.h
    src/DSP/FX/BitCrusherFX.h
    src/Engines/WavetableEngine.cpp
    src/Engines/WavetableEngine.h
    src/Engines/GranularEngine.cpp
    src/Engines/GranularEngine.h
    src/GUI/ControlPane.cpp
    src/GUI/ControlPane.h
    src/GUI/DisplayArea.cpp
    src/GUI/DisplayArea.h
    src/Core/Analyzer.cpp
    src/Core/Analyzer.h
    src/Core/MidiLearn.cpp
    src/Core/PerformanceOverlay.cpp
    src/Core/PerformanceOverlay.h
    src/Core/PresetManager.cpp
    src/Core/PresetManager.h
    src/Core/UnitTest.cpp
    src/Modulation/ChaosGen.cpp
    src/Modulation/ChaosGen.h
    src/Modulation/ModMatrix.cpp
    src/Modulation/ModMatrix.h
    src/Resources/ResourceManager.cpp
    src/Resources/ResourceManager.h
    src/GUI/MacroPanel.h
    src/GUI/EngineDisplay.h
)

target_compile_definitions(VoidTextureSynth PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(VoidTextureSynth PRIVATE
    juce::juce_audio_plugin_client
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_audio_utils
)
