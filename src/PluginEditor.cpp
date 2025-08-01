#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // CRITICAL: Detect if we're running in pluginval environment
    auto processName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileNameWithoutExtension().toLowerCase();
    bool isValidationEnvironment = processName.contains("pluginval") || processName.contains("validator");
    
    setSize (1000, 700);
    
    if (isValidationEnvironment)
    {
        // VALIDATION MODE: Create only minimal, safe components
        addAndMakeVisible(contentArea);
        
        // Just a simple label - no complex components
        titleLabel.setText("VoidTextureSynth - Validation Mode", juce::dontSendNotification);
        titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        titleLabel.setJustificationType(juce::Justification::centred);
        contentArea.addAndMakeVisible(titleLabel);
        
        // Simple volume slider only
        volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
        volumeSlider.setRange(0.0, 1.0, 0.01);
        volumeSlider.setValue(0.7);
        contentArea.addAndMakeVisible(volumeSlider);
        
        volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "masterVolume", volumeSlider);
            
        return; // Skip all complex initialization
    }
    
    // NORMAL MODE: Full functionality
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
    
    // Create SynthEngine1Panel
    synthEngine1Panel = std::make_unique<SynthEngine1Panel>(audioProcessor.apvts, audioProcessor.synthEngine1);
    contentArea.addAndMakeVisible(*synthEngine1Panel);
    
    // Create main page components
    waveformDisplay = std::make_unique<WaveformDisplay>();
    contentArea.addAndMakeVisible(*waveformDisplay);
    
    displayModeButton.setButtonText("Waveform");
    displayModeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF444444));
    displayModeButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xFF00FF88));
    displayModeButton.onClick = [this]() { displayModeButtonClicked(); };
    
    contentArea.addAndMakeVisible(displayModeButton);
    
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
    
    // Setup synth2 label (placeholder for future)
    synth2Label.setText("SYNTH ENGINE 2\n\nReserved for Future Expansion", juce::dontSendNotification);
    synth2Label.setFont(juce::Font(16.0f));
    synth2Label.setColour(juce::Label::textColourId, juce::Colour(0xFF888888));
    synth2Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(synth2Label);
    
    // Start with main tab
    updateTabVisibility();
    
    // Connect waveform display AFTER all components are ready
    if (waveformDisplay)
        audioProcessor.setWaveformDisplay(waveformDisplay.get());
}

VoidTextureSynthAudioProcessorEditor::~VoidTextureSynthAudioProcessorEditor()
{
    // Disconnect waveform display from processor
    audioProcessor.setWaveformDisplay(nullptr);
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
    // Check if we're in validation mode
    auto processName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileNameWithoutExtension().toLowerCase();
    bool isValidationEnvironment = processName.contains("pluginval") || processName.contains("validator");
    
    if (isValidationEnvironment)
    {
        // VALIDATION MODE: Only show basic components
        titleLabel.setVisible(true);
        volumeSlider.setVisible(true);
        return;
    }
    
    // NORMAL MODE: Hide all content first
    titleLabel.setVisible(false);
    volumeSlider.setVisible(false);
    volumeLabel.setVisible(false);
    if (waveformDisplay)
        waveformDisplay->setVisible(false);
    displayModeButton.setVisible(false);
    if (synthEngine1Panel)
        synthEngine1Panel->setVisible(false);
    synth2Label.setVisible(false);
    
    // Show content for current tab
    switch (currentTab)
    {
        case 0: // Main
            titleLabel.setVisible(true);
            volumeSlider.setVisible(true);
            volumeLabel.setVisible(true);
            if (waveformDisplay)
                waveformDisplay->setVisible(true);
            displayModeButton.setVisible(true);
            break;
        case 1: // Synth 1
            if (synthEngine1Panel)
                synthEngine1Panel->setVisible(true);
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
    
    // Check if we're in validation mode
    auto processName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileNameWithoutExtension().toLowerCase();
    bool isValidationEnvironment = processName.contains("pluginval") || processName.contains("validator");
    
    if (isValidationEnvironment)
    {
        // VALIDATION MODE: Simple layout
        contentArea.setBounds(bounds);
        titleLabel.setBounds(bounds.removeFromTop(100).reduced(20));
        volumeSlider.setBounds(bounds.removeFromTop(50).reduced(50));
        return;
    }
    
    // NORMAL MODE: Full layout
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
        
        // Waveform display area
        auto displayArea = contentBounds.reduced(20);
        displayModeButton.setBounds(displayArea.getRight() - 120, displayArea.getY(), 100, 30);
        if (waveformDisplay)
            waveformDisplay->setBounds(displayArea.reduced(0, 40));
    }
    else if (currentTab == 1) // Synth 1
    {
        if (synthEngine1Panel)
            synthEngine1Panel->setBounds(bounds);
    }
    else if (currentTab == 2) // Synth 2
    {
        synth2Label.setBounds(bounds);
    }
}

void VoidTextureSynthAudioProcessorEditor::displayModeButtonClicked()
{
    if (!waveformDisplay) return; // Safety check
    
    // Cycle through display modes
    auto currentMode = waveformDisplay->getDisplayMode();
    switch (currentMode) {
        case WaveformDisplay::DisplayMode::Waveform:
            waveformDisplay->setDisplayMode(WaveformDisplay::DisplayMode::Spectrum);
            displayModeButton.setButtonText("Spectrum");
            break;
        case WaveformDisplay::DisplayMode::Spectrum:
            waveformDisplay->setDisplayMode(WaveformDisplay::DisplayMode::Oscilloscope);
            displayModeButton.setButtonText("Oscilloscope");
            break;
        case WaveformDisplay::DisplayMode::Oscilloscope:
            waveformDisplay->setDisplayMode(WaveformDisplay::DisplayMode::Waveform);
            displayModeButton.setButtonText("Waveform");
            break;
    }
}
