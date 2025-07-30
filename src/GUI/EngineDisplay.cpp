#include "EngineDisplay.h"

EngineDisplay::EngineDisplay(const juce::String& title)
    : juce::OpenGLAppComponent(), engineTitle(title)
{
    // Attach OpenGL context if needed
}


EngineDisplay::~EngineDisplay()
{
    shutdownOpenGL();
}

void EngineDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black.withAlpha(0.3f));
    g.setColour(juce::Colours::cyan);
    g.setFont(20.0f);
    g.drawFittedText(engineTitle, getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
    // Placeholder for waveform/starfield
}

void EngineDisplay::initialise()
{
    // OpenGL initialization code here
}

void EngineDisplay::shutdown()
{
    // OpenGL shutdown code here
}

void EngineDisplay::render()
{
    // OpenGL rendering code here
    juce::OpenGLHelpers::clear(juce::Colours::black);
}
