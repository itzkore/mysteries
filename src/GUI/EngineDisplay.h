#pragma once
#include <juce_opengl/juce_opengl.h>
#include <juce_graphics/juce_graphics.h>

class EngineDisplay : public juce::OpenGLAppComponent {
public:
    EngineDisplay(const juce::String& title = {});
    ~EngineDisplay() override;

    void paint(juce::Graphics& g) override;
    void initialise() override;
    void shutdown() override;
    void render() override;

    juce::String engineTitle;
};
