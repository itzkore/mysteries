#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>
#include <cmath>

/**
 * OrbVisualizer - Circular audio visualization component that shows real-time
 * audio data as a glowing orb with reactive animations.
 * 
 * Features:
 * - Compact circular design with customizable colors
 * - Multiple visualization modes (Energy, Spectrum, Phase)
 * - Ambient glow and particle effects for dark ambient aesthetics
 * - MIDI-responsive visuals with size/color modulation
 * - Validation-safe implementation for pluginval level 10
 */
class OrbVisualizer : public juce::Component, public juce::Timer
{
public:
    enum class DisplayMode { 
        Energy,    // Simple energy visualization (amplitude)
        Spectrum,  // Frequency domain visualization
        Phase      // Phase/stereo correlation visualization
    };
    
    OrbVisualizer();
    ~OrbVisualizer() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setVisible(bool shouldBeVisible) override;
    
    //==============================================================================
    // Call this from audio thread to update the audio data
    void pushAudioData(const float* audioData, int numSamples);
    void pushAudioData(const juce::AudioBuffer<float>& buffer);
    
    //==============================================================================
    // MIDI activity tracking for visual feedback
    void setMidiActivity(bool isActive, float velocity = 1.0f);
    
    //==============================================================================
    // Timer callback for smooth animation
    void timerCallback() override;
    
    //==============================================================================
    // Display mode interface
    DisplayMode getDisplayMode() const { return currentMode; }
    void setDisplayMode(DisplayMode mode);
    
    //==============================================================================
    // Color customization
    void setOrbColor(juce::Colour baseColor);
    void setGlowColor(juce::Colour glowColor);
    void setBackgroundColor(juce::Colour bgColor);
    
    //==============================================================================
    // Animation controls
    void setRotationSpeed(float speed) { rotationSpeed = speed; }
    void setPulsationAmount(float amount) { pulsationAmount = juce::jlimit(0.0f, 1.0f, amount); }
    
    //==============================================================================
    // Manual display activation for validation safety
    void startDisplay() { if (!isTimerRunning()) startTimerHz(30); }
    void stopDisplay() { if (isTimerRunning()) stopTimer(); }
    
    // Helper for validation-safe display detection
    bool isDisplayed() const { return isVisible() && getParentComponent() != nullptr; }
    
private:
    //==============================================================================
    // Data storage
    static constexpr int bufferSize = 256; // Smaller buffer for efficiency
    static constexpr int fftOrder = 9;
    static constexpr int fftSize = 1 << fftOrder;
    static constexpr int numBins = fftSize / 2;
    
    std::array<float, bufferSize> audioBuffer;
    std::array<float, bufferSize> energyHistory;
    std::array<float, fftSize * 2> fftData;
    std::array<float, numBins> spectrumData;
    int writePos = 0;
    
    //==============================================================================
    // FFT processing
    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window { fftSize, juce::dsp::WindowingFunction<float>::hann };
    
    //==============================================================================
    // Visual parameters
    DisplayMode currentMode = DisplayMode::Energy;
    // Base orb color with dark grey purple tone
    juce::Colour orbColor = juce::Colour(0x55534463);       // Dark grey purple (33% opacity)
    // Glow effect color with dark purple tone
    juce::Colour glowColor = juce::Colour(0x40483D5A);      // Dark purple glow (25% opacity)
    juce::Colour backgroundColor = juce::Colour(0xFF0A0A0F); // Background color
    
    float rotationSpeed = 0.5f;        // How fast the orb rotates
    float rotationPhase = 0.0f;        // Current rotation position
    float pulsationAmount = 0.3f;      // How much the orb pulses (0-1)
    float currentEnergy = 0.0f;        // Current audio energy
    float peakEnergy = 0.0f;           // Peak energy (with slow decay)
    
    // MIDI activity tracking
    bool midiActive = false;           // Is any MIDI note currently active
    float midiVelocity = 0.0f;         // Current MIDI velocity (normalized 0-1)
    float midiDecay = 0.98f;           // Decay rate for MIDI velocity
    
    //==============================================================================
    // Particle system for ambient effects
    struct Particle {
        float x, y;        // Position
        float vx, vy;      // Velocity
        float life;        // Remaining life (0-1)
        float size;        // Particle size
        juce::Colour color; // Particle color
    };
    
    static constexpr int maxParticles = 50;
    std::array<Particle, maxParticles> particles;
    int activeParticles = 0;
    
    //==============================================================================
    // Helper methods
    void calculateSpectrum();
    void updateParticles();
    void emitParticles(int count);
    void drawOrb(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawParticles(juce::Graphics& g);
    void drawEnergyOrb(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawSpectrumOrb(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawPhaseOrb(juce::Graphics& g, juce::Rectangle<float> bounds);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OrbVisualizer)
};
