#include "MainDisplay.h"

MainDisplay::MainDisplay(SynthEngine1& synthEngine, juce::AudioProcessorValueTreeState& apvts)
    : synthEngine(synthEngine), apvts(apvts)
{
    // Minimal version for testing - just one control to verify it works
    masterVolume = std::make_unique<juce::Slider>();
    masterVolume->setRange(0.0, 1.0, 0.01);
    masterVolume->setValue(0.7);
    addAndMakeVisible(*masterVolume);
}

MainDisplay::~MainDisplay() = default;

void MainDisplay::paint(juce::Graphics& g)
{
    // Simple dark background
    g.fillAll(juce::Colour(0xFF1A1A2E));
    
    // Title
    g.setColour(juce::Colour(0xFF00FFFF));
    g.setFont(24.0f);
    g.drawText("MAIN DISPLAY", getLocalBounds().removeFromTop(40), juce::Justification::centred);
}

void MainDisplay::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(60); // Space for title
    
    // Center the volume control
    if (masterVolume)
        masterVolume->setBounds(bounds.getCentreX() - 50, bounds.getCentreY() - 50, 100, 100);
}
}

    // Envelope Controls - Attack and Release for expressive control
    attackTime = std::make_unique<juce::Slider>(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow);
    attackTime->setRange(0.001, 5.0, 0.001);
    attackTime->setValue(0.1);
    attackTime->setSkewFactorFromMidPoint(0.3);
    attackTime->setTextValueSuffix(" s");
    addAndMakeVisible(*attackTime);

    releaseTime = std::make_unique<juce::Slider>(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow);
    releaseTime->setRange(0.01, 10.0, 0.01);
    releaseTime->setValue(1.0);
    releaseTime->setSkewFactorFromMidPoint(2.0);
    releaseTime->setTextValueSuffix(" s");
    addAndMakeVisible(*releaseTime);

    // Performance XY Pad - Stunning visual control
    performanceX = std::make_unique<juce::Slider>(juce::Slider::LinearHorizontal, juce::Slider::NoTextBox);
    performanceX->setRange(0.0, 1.0, 0.01);
    performanceX->setValue(0.5);
    addAndMakeVisible(*performanceX);

    performanceY = std::make_unique<juce::Slider>(juce::Slider::LinearVertical, juce::Slider::NoTextBox);
    performanceY->setRange(0.0, 1.0, 0.01);
    performanceY->setValue(0.5);
    addAndMakeVisible(*performanceY);

    // Preset Selector - Quick access to textures
    presetSelector = std::make_unique<juce::ComboBox>();
    presetSelector->addItem("Dark Void", 1);
    presetSelector->addItem("Synthwave Pulse", 2);
    presetSelector->addItem("Ambient Drone", 3);
    presetSelector->addItem("Experimental Texture", 4);
    presetSelector->addItem("Deep Space", 5);
    presetSelector->setSelectedId(1);
    addAndMakeVisible(*presetSelector);

    // Create APVTS attachments for real-time control
    // Note: These parameter IDs should match those in Parameters.h
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "masterVolume", *masterVolume));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "filterCutoff", *filterCutoff));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "filterResonance", *filterResonance));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "attackTime", *attackTime));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "releaseTime", *releaseTime));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "performanceX", *performanceX));
    sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "performanceY", *performanceY));
    
    comboAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "presetSelector", *presetSelector);
}

MainDisplay::~MainDisplay() = default;

void MainDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Stunning dark gradient background
    juce::ColourGradient gradient(
        juce::Colour(0xFF0A0A0F),  // Very dark blue-black
        bounds.getTopLeft().toFloat(),
        juce::Colour(0xFF1A1A2E),  // Slightly lighter dark blue
        bounds.getBottomRight().toFloat(),
        false
    );
    g.setGradientFill(gradient);
    g.fillAll();

    // Add subtle geometric patterns for visual interest
    g.setColour(juce::Colour(0x10FFFFFF)); // Very subtle white overlay
    auto centerX = bounds.getCentreX();
    auto centerY = bounds.getCentreY();
    
    // Draw radiating lines from center - creates depth
    for (int i = 0; i < 12; ++i)
    {
        float angle = i * juce::MathConstants<float>::pi / 6.0f;
        float x1 = centerX + 50 * std::cos(angle);
        float y1 = centerY + 50 * std::sin(angle);
        float x2 = centerX + 200 * std::cos(angle);
        float y2 = centerY + 200 * std::sin(angle);
        g.drawLine(x1, y1, x2, y2, 0.5f);
    }

    // Performance XY Pad visual area
    auto xyPadBounds = juce::Rectangle<int>(bounds.getCentreX() - 150, bounds.getCentreY() - 100, 300, 200);
    
    // XY Pad background
    g.setColour(juce::Colour(0xFF2A2D34));
    g.fillRoundedRectangle(xyPadBounds.toFloat(), 8.0f);
    
    // XY Pad border with cyan glow
    g.setColour(juce::Colour(0xFF00FFFF));
    g.drawRoundedRectangle(xyPadBounds.toFloat(), 8.0f, 2.0f);
    
    // XY Position indicator
    float xPos = xyPadBounds.getX() + (performanceX->getValue() * xyPadBounds.getWidth());
    float yPos = xyPadBounds.getY() + ((1.0f - performanceY->getValue()) * xyPadBounds.getHeight());
    
    // Glowing crosshair at XY position
    g.setColour(juce::Colour(0xFF00FFFF));
    g.drawLine(xyPadBounds.getX(), yPos, xyPadBounds.getRight(), yPos, 1.0f);
    g.drawLine(xPos, xyPadBounds.getY(), xPos, xyPadBounds.getBottom(), 1.0f);
    
    // Bright center dot
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.fillEllipse(xPos - 4, yPos - 4, 8, 8);

    // Title text
    g.setColour(juce::Colour(0xFF00FFFF));
    g.setFont(juce::Font("Arial", 32.0f, juce::Font::bold));
    g.drawText("VOID TEXTURE SYNTH", bounds.removeFromTop(60), juce::Justification::centred);

    // Control labels
    g.setColour(juce::Colour(0xFFCCCCCC));
    g.setFont(juce::Font("Arial", 12.0f, juce::Font::plain));
    
    // Performance pad label
    g.drawText("PERFORMANCE XY", xyPadBounds.removeFromBottom(20), juce::Justification::centred);
}

void MainDisplay::resized()
{
    auto bounds = getLocalBounds();
    
    // Reserve space for title
    bounds.removeFromTop(80);
    
    // Performance XY Pad in center
    auto xyPadArea = juce::Rectangle<int>(bounds.getCentreX() - 150, bounds.getCentreY() - 100, 300, 200);
    performanceX->setBounds(xyPadArea.getX(), xyPadArea.getBottom() + 10, xyPadArea.getWidth(), 20);
    performanceY->setBounds(xyPadArea.getRight() + 10, xyPadArea.getY(), 20, xyPadArea.getHeight());

    // Control knobs arranged around the performance area
    int knobSize = 100;
    int spacing = 140;
    
    // Top row - Filter controls
    filterCutoff->setBounds(bounds.getCentreX() - spacing - knobSize/2, 120, knobSize, knobSize);
    filterResonance->setBounds(bounds.getCentreX() + spacing - knobSize/2, 120, knobSize, knobSize);
    
    // Bottom row - Envelope controls  
    attackTime->setBounds(bounds.getCentreX() - spacing - knobSize/2, bounds.getHeight() - 180, knobSize, knobSize);
    releaseTime->setBounds(bounds.getCentreX() + spacing - knobSize/2, bounds.getHeight() - 180, knobSize, knobSize);
    
    // Master volume - Large center bottom
    masterVolume->setBounds(bounds.getCentreX() - 60, bounds.getHeight() - 140, 120, 120);
    
    // Preset selector - Top right
    presetSelector->setBounds(bounds.getWidth() - 200, 20, 180, 30);
}
