#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (1000, 700);
    
    // Setup tab buttons
    mainTabButton.setButtonText("MAIN");
    mainTabButton.addListener(this);
    addAndMakeVisible(mainTabButton);
    
    synth1TabButton.setButtonText("SYNTH 1");
    synth1TabButton.addListener(this);
    addAndMakeVisible(synth1TabButton);
    
    synth2TabButton.setButtonText("SYNTH 2");
    synth2TabButton.addListener(this);
    addAndMakeVisible(synth2TabButton);
    
    // Setup content area
    addAndMakeVisible(contentArea);
    
    // Setup main controls
    titleLabel.setText("VoidTextureSynth", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(28.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    titleLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(titleLabel);
    
    // Volume slider
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.7);
    volumeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    contentArea.addAndMakeVisible(volumeSlider);
    
    // Connect volume slider to parameter
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "masterVolume", volumeSlider);
    
    // Volume label
    volumeLabel.setText("MASTER VOLUME", juce::dontSendNotification);
    volumeLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);
    volumeLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(volumeLabel);
    
    // Setup synth labels
    synth1Label.setText("SYNTH ENGINE 1\n\nAdvanced Configuration\nComing Soon...", juce::dontSendNotification);
    synth1Label.setFont(juce::Font(18.0f));
    synth1Label.setColour(juce::Label::textColourId, juce::Colours::white);
    synth1Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(synth1Label);
    
    synth2Label.setText("SYNTH ENGINE 2\n\nReserved for Future Expansion", juce::dontSendNotification);
    synth2Label.setFont(juce::Font(16.0f));
    synth2Label.setColour(juce::Label::textColourId, juce::Colour(0xFF888888));
    synth2Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(synth2Label);
    
    // Start with main tab
    updateTabVisibility();
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor()
{
}

void VoidTextureSynthAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &mainTabButton)
        currentTab = 0;
    else if (button == &synth1TabButton)
        currentTab = 1;
    else if (button == &synth2TabButton)
        currentTab = 2;
    
    updateTabVisibility();
}

void VoidTextureSynthAudioProcessorEditor::updateTabVisibility()
{
    // Hide all content first
    titleLabel.setVisible(false);
    volumeSlider.setVisible(false);
    volumeLabel.setVisible(false);
    synth1Label.setVisible(false);
    synth2Label.setVisible(false);
    
    // Show content for current tab
    switch (currentTab)
    {
        case 0: // Main
            titleLabel.setVisible(true);
            volumeSlider.setVisible(true);
            volumeLabel.setVisible(true);
            break;
        case 1: // Synth 1
            synth1Label.setVisible(true);
            break;
        case 2: // Synth 2
            synth2Label.setVisible(true);
            break;
    }
    
    resized();
}

void VoidTextureSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF0A0A0F));
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Tab buttons at top
    auto tabArea = bounds.removeFromTop(40);
    auto buttonWidth = tabArea.getWidth() / 3;
    mainTabButton.setBounds(0, 0, buttonWidth, 40);
    synth1TabButton.setBounds(buttonWidth, 0, buttonWidth, 40);
    synth2TabButton.setBounds(buttonWidth * 2, 0, buttonWidth, 40);
    
    // Content area
    contentArea.setBounds(bounds);
    
    // Layout content based on current tab
    if (currentTab == 0) // Main tab
    {
        auto contentBounds = bounds;
        titleLabel.setBounds(contentBounds.removeFromTop(60).reduced(20));
        
        auto volumeArea = contentBounds.removeFromTop(150);
        volumeSlider.setBounds(volumeArea.getCentreX() - 75, volumeArea.getY() + 20, 150, 150);
        volumeLabel.setBounds(volumeArea.getCentreX() - 75, volumeArea.getBottom() - 30, 150, 20);
    }
    else if (currentTab == 1) // Synth 1
    {
        synth1Label.setBounds(bounds);
    }
    else if (currentTab == 2) // Synth 2
    {
        synth2Label.setBounds(bounds);
    }
}
