#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <array>

class WaveformDisplay : public juce::Component, public juce::Timer
{
public:
    enum class DisplayMode { Waveform, Spectrum, Oscilloscope };
    
    WaveformDisplay();
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void setVisible(bool shouldBeVisible) override;
    
    // Call this from audio thread to update the waveform data
    void pushAudioData(const float* audioData, int numSamples);
    void pushAudioData(const juce::AudioBuffer<float>& buffer);
    
    // Timer callback for smooth animation
    void timerCallback() override;
    
    // Display mode interface
    DisplayMode getDisplayMode() const { return currentMode; }
    void setDisplayMode(DisplayMode mode) { currentMode = mode; repaint(); }
    
    // Manual display activation for validation safety
    void startDisplay() { if (!isTimerRunning()) startTimerHz(20); }
    
    // Smart validation detection
    bool isInValidationMode() const;
    void setValidationSafeMode(bool enabled) { validationSafeMode = enabled; }
    void stopDisplay() { if (isTimerRunning()) stopTimer(); }
    
    // Helper for validation-safe display detection
    bool isDisplayed() const { return isVisible() && getParentComponent() != nullptr; }
    
private:
    static constexpr int bufferSize = 512;
    static constexpr int fftOrder = 9;
    static constexpr int fftSize = 1 << fftOrder;
    static constexpr int numBins = fftSize / 2;
    
    // Waveform data
    std::array<float, bufferSize> waveformBuffer;
    std::array<float, fftSize> spectrumData;
    
    int writePos = 0;
    DisplayMode currentMode = DisplayMode::Waveform;
    
    // Visual effects
    float peakLevel = 0.0f;
    float peakDecay = 0.95f;
    
    // FFT for spectrum analysis
    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;
    std::array<float, fftSize * 2> fftData;
    
    // Colors for the void aesthetic
    juce::Colour primaryColor = juce::Colours::cyan;
    juce::Colour secondaryColor = juce::Colour(0xFF4A90E2);
    juce::Colour accentColor = juce::Colour(0xffff6b35);
    juce::Colour backgroundColor = juce::Colour(0xFF0A0A0F);
    
    // Validation safety control
    bool validationSafeMode = false;
    
    void drawWaveform(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawSpectrum(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawOscilloscope(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawPeakMeter(juce::Graphics& g, juce::Rectangle<int> bounds);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
