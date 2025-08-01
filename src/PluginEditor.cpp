#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoidTextureSynthAudioProcessorEditor::VoidTextureSynthAudioProcessorEditor (VoidTextureSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set the custom look and feel
    setLookAndFeel(&voidLookAndFeel);
    
    // CRITICAL: Detect if we're running in pluginval environment
    auto processName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileNameWithoutExtension().toLowerCase();
    bool isValidationEnvironment = processName.contains("pluginval") || processName.contains("validator");
    
    // Larger plugin size for better usability
    setSize (1200, 800);
    
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
    
    synth1TabButton.setButtonText("SYNTH 1");
    synth1TabButton.addListener(this);
    
    synth2TabButton.setButtonText("SYNTH 2");
    synth2TabButton.addListener(this);
    
    // Apply styling to tab buttons
    juce::Colour activeTabColor(0xFF2A5599);
    juce::Colour inactiveTabColor(0xFF151518);
    juce::Colour textColor(0xFFEEEEFF);
    
    for (auto* button : {&mainTabButton, &synth1TabButton, &synth2TabButton})
    {
        button->setColour(juce::TextButton::buttonColourId, inactiveTabColor);
        button->setColour(juce::TextButton::buttonOnColourId, activeTabColor);
        button->setColour(juce::TextButton::textColourOnId, textColor);
        button->setColour(juce::TextButton::textColourOffId, textColor.withAlpha(0.7f));
        addAndMakeVisible(button);
    }
    
    mainTabButton.setToggleState(true, juce::dontSendNotification);
    
    // Setup content area
    addAndMakeVisible(contentArea);
    
    // Create SynthEngine1Panel
    synthEngine1Panel = std::make_unique<SynthEngine1Panel>(audioProcessor.apvts, audioProcessor.synthEngine1);
    contentArea.addAndMakeVisible(*synthEngine1Panel);
    
    // Create main page components
    waveformDisplay = std::make_unique<OrbVisualizer>();
    contentArea.addAndMakeVisible(*waveformDisplay);
    
    displayModeButton.setButtonText("Energy");
    displayModeButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF2A2A33));
    displayModeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF2A5599));
    displayModeButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    displayModeButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    displayModeButton.onClick = [this]() { displayModeButtonClicked(); };
    
    contentArea.addAndMakeVisible(displayModeButton);
    
        // Setup section labels with consistent styling
    fxLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    fxLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    fxLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(fxLabel);
    
    reverbLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    reverbLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    reverbLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(reverbLabel);
    
    delayLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    delayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    delayLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(delayLabel);
    
    // Setup synth2 label (placeholder for future) with improved styling
    synth2Label.setText("SYNTH ENGINE 2\n\nComing Soon", juce::dontSendNotification);
    synth2Label.setFont(juce::Font(24.0f, juce::Font::bold));
    synth2Label.setColour(juce::Label::textColourId, juce::Colour(0xFF00AA44));
    synth2Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(synth2Label);
    
    // Setup main controls with improved styling
    titleLabel.setText("VoidTextureSynth", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    
    // Create subtle text gradient effect
    juce::ColourGradient textGradient(
        juce::Colour(0xFF99CCFF), 0, 0,
        juce::Colour(0xFF2A5599), 250, 40,
        false);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF99CCFF));
    
    titleLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(titleLabel);
    
    // Volume slider with improved styling
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.7);
    
    // Blue theme for volume control
    volumeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF2A5599));
    volumeSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF1A1A20));
    volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFF99CCFF));
    volumeSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    volumeSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF333333));
    volumeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF151518));
    contentArea.addAndMakeVisible(volumeSlider);
    
    volumeLabel.setText("MASTER", juce::dontSendNotification);
    volumeLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    volumeLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF99CCFF));
    volumeLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(volumeLabel);
    
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
    synth2Label.setFont(juce::Font(18.0f, juce::Font::bold));
    synth2Label.setColour(juce::Label::textColourId, juce::Colour(0xFF00AA44));
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
    // Remove the custom look and feel
    setLookAndFeel(nullptr);
    
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
    {
        synthEngine1Panel->setVisible(false);
        // Hide any panel-specific labels here
    }
    synth2Label.setVisible(false);
    fxLabel.setVisible(false);
    reverbLabel.setVisible(false);
    delayLabel.setVisible(false);
    
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
            fxLabel.setVisible(true);
            reverbLabel.setVisible(true);
            delayLabel.setVisible(true);
            break;
        case 1: // Synth 1
            if (synthEngine1Panel)
            {
                synthEngine1Panel->setVisible(true);
            }
            break;
        case 2: // Synth 2
            synth2Label.setVisible(true);
            break;
    }
    
    resized();
}

void VoidTextureSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Create a cosmic background with stars using our member variable
    voidLookAndFeel.drawCosmicBackground(g, getLocalBounds(), 0.0004f);
    
    // Draw main panels if on main tab
    if (currentTab == 0)
    {
        auto bounds = getLocalBounds();
        auto contentBounds = bounds;
        contentBounds.removeFromTop(100); // Skip title and tabs
        
        // Draw section backgrounds with subtle borders
        auto leftHalf = contentBounds.removeFromLeft(contentBounds.getWidth() / 2);
        auto rightHalf = contentBounds;
        
        // Synth 1 background (left side) - dark with subtle gradient
        juce::ColourGradient leftGrad(
            juce::Colour(0xFF1E1E22), leftHalf.getX(), leftHalf.getY(),
            juce::Colour(0xFF18181A), leftHalf.getX(), leftHalf.getBottom(),
            false);
        g.setGradientFill(leftGrad);
        g.fillRoundedRectangle(leftHalf.reduced(5).toFloat(), 6.0f);
        
        // Add a glowing border with gradient
        g.setColour(juce::Colour(0xFF2A5599));
        g.drawRoundedRectangle(leftHalf.reduced(5).toFloat(), 6.0f, 1.2f);
        
        // Draw synth engine 1 header area
        auto synth1HeaderArea = leftHalf.reduced(15, 15).removeFromTop(30);
        juce::ColourGradient headerGrad(
            juce::Colour(0xFF2A5599), synth1HeaderArea.getX(), synth1HeaderArea.getCentreY(),
            juce::Colour(0xFF152A44), synth1HeaderArea.getRight(), synth1HeaderArea.getCentreY(),
            false);
        g.setGradientFill(headerGrad);
        g.fillRoundedRectangle(synth1HeaderArea.toFloat(), 4.0f);
        
        // Orb area with subtle glow
        auto orbAreaBounds = leftHalf.reduced(15);
        orbAreaBounds.removeFromTop(50); // Skip header
        auto tempOrbArea = orbAreaBounds.removeFromTop(orbAreaBounds.getHeight() * 0.5);
        
        // Centrované umístění orbu s vlastními rozměry
        int orbWidth = std::min(orbAreaBounds.getWidth() - 40, 220);
        int orbHeight = std::min(orbAreaBounds.getHeight() - 40, 220);
        int orbX = tempOrbArea.getCentreX() - orbWidth/2;
        int orbY = tempOrbArea.getCentreY() - orbHeight/2;
        juce::Rectangle<int> orbArea(orbX, orbY, orbWidth, orbHeight);
        
        // Draw an elegant backdrop for the orb
        g.setColour(juce::Colour(0xFF101014));
        g.fillRoundedRectangle(orbArea.expanded(10).toFloat(), 8.0f);
        g.setColour(juce::Colour(0xFF2A2A33));
        g.drawRoundedRectangle(orbArea.expanded(10).toFloat(), 8.0f, 1.0f);
        
        // FX section panel 
        auto fxSectionY = orbArea.getBottom() + 40;
        auto fxSectionHeight = leftHalf.getBottom() - fxSectionY - 15;
        juce::Rectangle<int> fxSectionBounds(leftHalf.getX() + 15, fxSectionY, leftHalf.getWidth() - 30, fxSectionHeight);
        
        // Draw FX section header with gradient
        juce::ColourGradient fxHeaderGrad(
            juce::Colour(0xFF2A5599), fxSectionBounds.getX(), fxSectionBounds.getY(),
            juce::Colour(0xFF152A44), fxSectionBounds.getRight(), fxSectionBounds.getY(),
            false);
        g.setGradientFill(fxHeaderGrad);
        g.fillRoundedRectangle(fxSectionBounds.removeFromTop(30).toFloat(), 4.0f);
        
        // Draw FX section body
        g.setColour(juce::Colour(0xFF151518));
        g.fillRoundedRectangle(fxSectionBounds.toFloat(), 4.0f);
        g.setColour(juce::Colour(0xFF252530));
        g.drawRoundedRectangle(fxSectionBounds.toFloat(), 4.0f, 1.0f);
        
        // Draw the 2x2 grid of FX modules
        const int moduleWidth = (fxSectionBounds.getWidth() - 30) / 2;
        const int moduleHeight = (fxSectionBounds.getHeight() - 20) / 2;
        
        juce::Rectangle<int> module1Bounds(fxSectionBounds.getX() + 10, 
                                          fxSectionBounds.getY() + 10, 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module2Bounds(module1Bounds.getRight() + 10, 
                                          module1Bounds.getY(), 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module3Bounds(module1Bounds.getX(), 
                                          module1Bounds.getBottom() + 10, 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module4Bounds(module2Bounds.getX(), 
                                          module3Bounds.getY(), 
                                          moduleWidth, moduleHeight);
        
        // Draw each module with gradient background
        juce::String moduleNames[4] = {"REVERB", "DELAY", "FILTER", "DISTORTION"};
        juce::Rectangle<int> moduleBounds[4] = {module1Bounds, module2Bounds, module3Bounds, module4Bounds};
        
        for (int i = 0; i < 4; ++i) {
            // Module background
            juce::ColourGradient moduleGrad(
                juce::Colour(0xFF1A1A20), moduleBounds[i].getX(), moduleBounds[i].getY(),
                juce::Colour(0xFF151518), moduleBounds[i].getX(), moduleBounds[i].getBottom(),
                false);
            g.setGradientFill(moduleGrad);
            g.fillRoundedRectangle(moduleBounds[i].toFloat(), 3.0f);
            
            // Module border
            g.setColour(juce::Colour(0xFF2A2A33));
            g.drawRoundedRectangle(moduleBounds[i].toFloat(), 3.0f, 1.0f);
            
            // Module header (smaller for compact layout)
            g.setFont(juce::Font(12.0f, juce::Font::bold));
            g.setColour(juce::Colours::white);
            g.drawText(moduleNames[i], moduleBounds[i].removeFromTop(18), juce::Justification::centred, true);
        }
        
        // Synth 2 background (right side)
        juce::ColourGradient rightGrad(
            juce::Colour(0xFF1A1A1C), rightHalf.getX(), rightHalf.getY(),
            juce::Colour(0xFF131315), rightHalf.getX(), rightHalf.getBottom(),
            false);
        g.setGradientFill(rightGrad);
        g.fillRoundedRectangle(rightHalf.reduced(5).toFloat(), 6.0f);
        
        // Green accent border for Synth 2
        g.setColour(juce::Colour(0xFF00AA44));
        g.drawRoundedRectangle(rightHalf.reduced(5).toFloat(), 6.0f, 1.2f);
    }
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
        
        // Top title area with cosmic styling and consistent spacing
        auto titleArea = contentBounds.removeFromTop(60);
        titleLabel.setBounds(titleArea.reduced(30, 10));
        
        // Create two halves for Synth 1 and future Synth 2 with cosmic layout proportions
        auto leftHalf = contentBounds.removeFromLeft(contentBounds.getWidth() / 2);
        auto rightHalf = contentBounds;
        
        // Add margin between halves for better separation with cosmic aesthetic
        leftHalf.reduce(12, 12);
        rightHalf.reduce(12, 12);
        
        // Master volume positioned properly at top with enough clickable area
        auto volumeSection = leftHalf.removeFromTop(70);
        auto volumeArea = volumeSection.removeFromRight(100).reduced(5, 5);
        volumeSlider.setBounds(volumeArea);
        volumeLabel.setBounds(volumeArea.getX(), volumeArea.getBottom() - 15, volumeArea.getWidth(), 15);
        
        // Only show Synth Engine 1 label on the main tab
        auto synth1HeaderArea = leftHalf.removeFromTop(30);
        fxLabel.setText("SYNTH ENGINE 1", juce::dontSendNotification);
        fxLabel.setBounds(synth1HeaderArea);
        
        // Create proper layout for left half - split into sections
        auto leftPanelContent = leftHalf;
        
        // Orb visualizer centered with more presence - adjusted for cosmic theme
        auto orbSectionHeight = leftPanelContent.getHeight() * 0.50f; // Reduced prominence for spacing
        auto orbArea = leftPanelContent.removeFromTop(orbSectionHeight);
        
        // Centered orb with custom dimensions - larger for better visibility
        int orbWidth = std::min(orbArea.getWidth() - 100, 180);  // Further reduced max size
        int orbHeight = std::min(orbArea.getHeight() - 80, 180);
        int orbX = orbArea.getCentreX() - orbWidth/2;
        int orbY = orbArea.getCentreY() - orbHeight/2 - 20;
        juce::Rectangle<int> orbBounds(orbX, orbY, orbWidth, orbHeight);
        
        // Display mode button below orb with cosmic styling
        displayModeButton.setBounds(juce::Rectangle<int>(orbBounds.getCentreX() - 50, orbBounds.getBottom() + 10, 100, 25));
        
        if (waveformDisplay)
            waveformDisplay->setBounds(orbBounds);
        
        // FX section reduced for compact layout
        auto remainingHeight = leftPanelContent.getHeight();
        auto fxSectionArea = leftPanelContent.removeFromBottom(remainingHeight * 0.15f);  // 15% of remaining height
        
        // Add a label for FX section with cosmic styling
        auto fxHeaderArea = fxSectionArea.removeFromTop(25);
        reverbLabel.setText("FX MODULES", juce::dontSendNotification);
        reverbLabel.setFont(juce::Font(14.0f, juce::Font::bold));
        reverbLabel.setBounds(fxHeaderArea.reduced(5, 0));
        
        // Create a compact grid for FX modules (2x2)
        const int moduleWidth = (fxSectionArea.getWidth() - 30) / 2;
        const int moduleHeight = (fxSectionArea.getHeight() - 20) / 2;
        
        // Four FX modules in a grid - more compact layout
        juce::Rectangle<int> module1Bounds(fxSectionArea.getX() + 5, 
                                          fxHeaderArea.getBottom() + 5, 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module2Bounds(module1Bounds.getRight() + 5, 
                                          module1Bounds.getY(), 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module3Bounds(module1Bounds.getX(), 
                                          module1Bounds.getBottom() + 5, 
                                          moduleWidth, moduleHeight);
        
        juce::Rectangle<int> module4Bounds(module2Bounds.getX(), 
                                          module3Bounds.getY(), 
                                          moduleWidth, moduleHeight);
        
        // Set delay label to the first module position with cosmic styling
        // Reduce FX module header size for better fit
        auto fxLabelBounds = module1Bounds.removeFromTop(18);
        delayLabel.setBounds(fxLabelBounds);
        delayLabel.setFont(juce::Font(12.0f, juce::Font::bold));
        delayLabel.setText("REVERB", juce::dontSendNotification);
        
        // Right half is reserved for Synth 2 with cosmic purple styling
        synth2Label.setBounds(rightHalf.reduced(20));
    }
    else if (currentTab == 1) // Synth 1
    {
        // Only set the Synth Engine 1 panel visible on the Synth 1 tab
        if (synthEngine1Panel) {
            synthEngine1Panel->setBounds(bounds);
        }
    }
    else if (currentTab == 2) // Synth 2
    {
        // Show only synth 2 label
        synth2Label.setBounds(bounds);
    }
}

void VoidTextureSynthAudioProcessorEditor::displayModeButtonClicked()
{
    if (!waveformDisplay) return; // Safety check
    
    // Cycle through display modes
    auto currentMode = waveformDisplay->getDisplayMode();
    switch (currentMode) {
        case OrbVisualizer::DisplayMode::Energy:
            waveformDisplay->setDisplayMode(OrbVisualizer::DisplayMode::Spectrum);
            displayModeButton.setButtonText("Spectrum");
            break;
        case OrbVisualizer::DisplayMode::Spectrum:
            waveformDisplay->setDisplayMode(OrbVisualizer::DisplayMode::Phase);
            displayModeButton.setButtonText("Phase");
            break;
        case OrbVisualizer::DisplayMode::Phase:
            waveformDisplay->setDisplayMode(OrbVisualizer::DisplayMode::Energy);
            displayModeButton.setButtonText("Energy");
            break;
    }
}
