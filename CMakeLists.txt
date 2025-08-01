
cmake_minimum_required(VERSION 3.22)
project(VoidTextureSynth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Set default build type to Release for better performance
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# JUCE submodule
add_subdirectory(ext/JUCE)

# Optimize for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -ffast-math")
    endif()
endif()

# Only enable debug flags for explicit Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(NOT MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    endif()
endif()

juce_add_plugin(VoidTextureSynth
    COMPANY_NAME "Voidwalker"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    PLUGIN_MANUFACTURER_CODE "VDSY"
    PLUGIN_CODE "VDS1"
    FORMATS VST3
    PRODUCT_NAME "VoidTextureSynth"
    VST3_CATEGORIES "Instrument|Synth"
    DEFINES "JUCE_VST3_CAN_REPLACE_VST2=0;JucePlugin_IsSynth=1;JucePlugin_WantsMidiInput=1;JucePlugin_ProducesMidiOutput=0"
)
target_sources(VoidTextureSynth PRIVATE
    src/Engines/SynthEngine1.cpp
    src/Engines/SynthEngine1.h
    src/Synth/OscillatorLayer.cpp
    src/Synth/SubLayer.cpp
    src/Synth/NoiseLayer.cpp
    src/Synth/SamplerLayer.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/Parameters.cpp
    src/Parameters.h
    src/GUI/EngineDisplay.cpp
    src/GUI/MacroPanel.cpp
    src/GUI/VoidLookAndFeel.cpp
    src/GUI/SynthEngine1Panel.cpp
    src/GUI/SynthEngine1Panel.h
    src/DSP/VoidOscillator.cpp
    src/DSP/DarkFilter.cpp
    src/DSP/SynthVoice.cpp
    src/DSP/FX/ReverbFX.h
    src/DSP/FX/DelayFX.h
    src/DSP/FX/BitCrusherFX.h
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
)

target_compile_definitions(VoidTextureSynth PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JucePlugin_IsSynth=1
    JucePlugin_WantsMidiInput=1
    JucePlugin_ProducesMidiOutput=0
)

target_link_libraries(VoidTextureSynth PRIVATE
    juce::juce_audio_plugin_client
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_opengl
    juce::juce_audio_formats
)

# Ensure Source/Synth headers are available
target_include_directories(VoidTextureSynth PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Synth
)
