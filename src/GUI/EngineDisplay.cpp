#include "EngineDisplay.h"

EngineDisplay::EngineDisplay(const juce::String& title)
    : juce::OpenGLAppComponent(), engineTitle(title)
{
    try {
        DBG("EngineDisplay constructor: " << title);
        // Attach OpenGL context if needed
    } catch (std::exception& e) {
        DBG("Exception in EngineDisplay constructor: " << e.what());
    } catch (...) {
        DBG("Unknown exception in EngineDisplay constructor");
    }
}


EngineDisplay::~EngineDisplay()
{
    // Fix: Only shutdown OpenGL if context is still attached
    if (openGLContext.isAttached())
    {
        shutdownOpenGL();
    }
}

void EngineDisplay::paint(juce::Graphics& g)
{
    try {
        DBG("EngineDisplay paint: " << engineTitle);
        g.fillAll(juce::Colours::black.withAlpha(0.3f));
        g.setColour(juce::Colours::cyan);
        g.setFont(20.0f);
        g.drawFittedText(engineTitle, getLocalBounds().removeFromTop(40), juce::Justification::centred, 1);
        // Placeholder for waveform/starfield
    } catch (std::exception& e) {
        DBG("Exception in EngineDisplay paint: " << e.what());
    } catch (...) {
        DBG("Unknown exception in EngineDisplay paint");
    }
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
