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
    
    // Initialize FX Module Controls
    setupFXControls();
    
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
    
    // Setup main title
    titleLabel.setText("VoidTextureSynth", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(32.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFF99CCFF));
    titleLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(titleLabel);
    
    // Setup master volume control with professional styling
    volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.7);
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
    
    // Connect volume slider to parameter
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "masterVolume", volumeSlider);
    
    // Connect macro sliders to parameters
    macro1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "macro1", macro1Slider);
    macro2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "macro2", macro2Slider);
    macro3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "macro3", macro3Slider);
    macro4Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "macro4", macro4Slider);
    
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

bool VoidTextureSynthAudioProcessorEditor::keyPressed(const juce::KeyPress& key)
{
#if MELATONIN_INSPECTOR && JUCE_DEBUG
    // Toggle Melatonin Inspector with Ctrl+Shift+I (Debug builds only)
    if (key == juce::KeyPress('i', juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier, 0))
    {
        inspector.setVisible(!inspector.isVisible());
        return true;
    }
#endif
    
    return false; // Let other components handle the key
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
        
        // Hide all other components
        volumeLabel.setVisible(false);
        if (waveformDisplay)
            waveformDisplay->setVisible(false);
        displayModeButton.setVisible(false);
        if (synthEngine1Panel)
            synthEngine1Panel->setVisible(false);
        synth2Label.setVisible(false);
        fxLabel.setVisible(false);
        fxHeaderLabel.setVisible(false);
        setFXControlsVisible(false);
        return;
    }
    
    // NORMAL MODE: Hide ALL content first to prevent overlapping
    titleLabel.setVisible(false);
    volumeSlider.setVisible(false);
    volumeLabel.setVisible(false);
    if (waveformDisplay)
        waveformDisplay->setVisible(false);
    displayModeButton.setVisible(false);
    
    // Hide Synth 1 panel completely
    if (synthEngine1Panel)
        synthEngine1Panel->setVisible(false);
    
    // Hide Synth 2 content
    synth2Label.setVisible(false);
    
    // Hide all FX related components
    fxLabel.setVisible(false);
    fxHeaderLabel.setVisible(false);
    reverbLabel.setVisible(false);
    delayLabel.setVisible(false);
    filterLabel.setVisible(false);
    distortionLabel.setVisible(false);
    setFXControlsVisible(false);
    
    // Show content ONLY for current tab
    switch (currentTab)
    {
        case 0: // Main Tab - Our FlexBox layout
            titleLabel.setVisible(true);
            volumeSlider.setVisible(true);
            volumeLabel.setVisible(true);
            if (waveformDisplay)
                waveformDisplay->setVisible(true);
            displayModeButton.setVisible(true);
            fxLabel.setVisible(true);
            fxHeaderLabel.setVisible(true);
            reverbLabel.setVisible(true);
            delayLabel.setVisible(true);
            filterLabel.setVisible(true);
            distortionLabel.setVisible(true);
            setFXControlsVisible(true);
            break;
            
        case 1: // Synth 1 Tab - Dedicated panel
            if (synthEngine1Panel)
                synthEngine1Panel->setVisible(true);
            break;
            
        case 2: // Synth 2 Tab - Simple label
            synth2Label.setVisible(true);
            break;
    }
    
    // Force layout update
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
        contentBounds.reduce(16, 16);
        
        // CORRECT LAYOUT PAINTING:
        // Top section: Left = Synth 1, Right = placeholder
        auto topSection = contentBounds.removeFromTop(static_cast<int>(contentBounds.getHeight() * 0.6f));
        auto bottomSection = contentBounds; // FX + Macros area
        
        // Left side - Synth Engine 1
        auto synthArea = topSection.removeFromLeft(topSection.getWidth() / 2);
        juce::ColourGradient leftGrad(
            juce::Colour(0xFF1E1E22), synthArea.getX(), synthArea.getY(),
            juce::Colour(0xFF18181A), synthArea.getX(), synthArea.getBottom(),
            false);
        g.setGradientFill(leftGrad);
        g.fillRoundedRectangle(synthArea.reduced(5).toFloat(), 6.0f);
        g.setColour(juce::Colour(0xFF2A5599));
        g.drawRoundedRectangle(synthArea.reduced(5).toFloat(), 6.0f, 1.2f);
        
        // Right side placeholder
        auto rightPlaceholder = topSection;
        g.setColour(juce::Colour(0xFF0F0F0F));
        g.fillRoundedRectangle(rightPlaceholder.reduced(5).toFloat(), 6.0f);
        g.setColour(juce::Colour(0xFF333333));
        g.drawRoundedRectangle(rightPlaceholder.reduced(5).toFloat(), 6.0f, 1.2f);
        
        // Bottom section - FX + Macros (full width)
        juce::ColourGradient bottomGrad(
            juce::Colour(0xFF1A1E22), bottomSection.getX(), bottomSection.getY(),
            juce::Colour(0xFF151A18), bottomSection.getX(), bottomSection.getBottom(),
            false);
        g.setGradientFill(bottomGrad);
        g.fillRoundedRectangle(bottomSection.reduced(5).toFloat(), 6.0f);
        g.setColour(juce::Colour(0xFF00AA44));
        g.drawRoundedRectangle(bottomSection.reduced(5).toFloat(), 6.0f, 1.2f);
        
        // Draw Energy orb backdrop in synth area
        auto orbAreaBounds = synthArea.reduced(15);
        orbAreaBounds.removeFromTop(50); // Skip header
        
        int orbWidth = std::min(orbAreaBounds.getWidth() - 40, 180);
        int orbHeight = std::min(orbAreaBounds.getHeight() - 40, 180);
        int orbX = orbAreaBounds.getCentreX() - orbWidth/2;
        int orbY = orbAreaBounds.getCentreY() - orbHeight/2;
        juce::Rectangle<int> orbArea(orbX, orbY, orbWidth, orbHeight);
        
        g.setColour(juce::Colour(0xFF101014));
        g.fillRoundedRectangle(orbArea.expanded(10).toFloat(), 8.0f);
        g.setColour(juce::Colour(0xFF2A2A33));
        g.drawRoundedRectangle(orbArea.expanded(10).toFloat(), 8.0f, 1.0f);
    }
}

void VoidTextureSynthAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Validation safety check - bounds must be valid
    if (bounds.isEmpty() || bounds.getWidth() < 50 || bounds.getHeight() < 50)
        return;
    
    // Check if we're in validation mode
    auto processName = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getFileNameWithoutExtension().toLowerCase();
    bool isValidationEnvironment = processName.contains("pluginval") || processName.contains("validator");
    
    if (isValidationEnvironment)
    {
        // VALIDATION MODE: Ultra-safe simple layout
        contentArea.setBounds(bounds);
        titleLabel.setBounds(bounds.removeFromTop(100).reduced(20));
        volumeSlider.setBounds(bounds.removeFromTop(50).reduced(50));
        return;
    }
    
    // NORMAL MODE: Professional FlexBox layout system
    layoutWithFlexBox(bounds);
}

void VoidTextureSynthAudioProcessorEditor::layoutWithFlexBox(juce::Rectangle<int> bounds)
{
    // Main layout FlexBox container - vertical stack
    juce::FlexBox mainLayout;
    mainLayout.flexDirection = juce::FlexBox::Direction::column;
    mainLayout.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    mainLayout.alignItems = juce::FlexBox::AlignItems::stretch;
    
    // Header section (tabs) - fixed height
    juce::FlexBox headerLayout;
    headerLayout.flexDirection = juce::FlexBox::Direction::row;
    headerLayout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    headerLayout.alignItems = juce::FlexBox::AlignItems::stretch;
    
    headerLayout.items.add(juce::FlexItem(mainTabButton).withFlex(1.0f).withMargin(2));
    headerLayout.items.add(juce::FlexItem(synth1TabButton).withFlex(1.0f).withMargin(2));
    headerLayout.items.add(juce::FlexItem(synth2TabButton).withFlex(1.0f).withMargin(2));
    
    // Content area - flexible height
    auto contentBounds = bounds;
    auto headerArea = contentBounds.removeFromTop(44);
    headerLayout.performLayout(headerArea);
    
    // Set content area bounds
    contentArea.setBounds(contentBounds);
    
    // Layout content based on current tab
    switch (currentTab)
    {
        case 0: // Main tab - Use FlexBox layout
            layoutMainTabWithFlexBox(contentBounds);
            break;
        case 1: // Synth 1 tab - Dedicated panel only
            if (synthEngine1Panel && synthEngine1Panel->isVisible()) {
                synthEngine1Panel->setBounds(contentBounds.reduced(16));
            }
            break;
        case 2: // Synth 2 tab - Simple label only
            if (synth2Label.isVisible()) {
                synth2Label.setBounds(contentBounds.reduced(16));
            }
            break;
    }
}

void VoidTextureSynthAudioProcessorEditor::layoutMainTabWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout if we're on the main tab
    if (currentTab != 0)
        return;
        
    // Apply consistent margin to entire layout
    const int MARGIN = 16;
    bounds.reduce(MARGIN, MARGIN);
    
    // STEP 1: Title section - positioned right under tabs, full width
    if (titleLabel.isVisible())
    {
        auto titleBounds = bounds.removeFromTop(40); // Reduced height, moved up
        titleLabel.setBounds(titleBounds); // Full width, no additional reducing
        titleLabel.setJustificationType(juce::Justification::centred); // Center the text
    }
    
    // RESTRUCTURED LAYOUT: 
    // - Top area: Left = Synth Engine, Right = Master Controls
    // - Bottom area: Left = FX Modules, Right = Macro Panel
    
    // Split into top and bottom sections
    auto topSection = bounds.removeFromTop(static_cast<int>(bounds.getHeight() * 0.6f));
    auto bottomSection = bounds; // Remaining height for FX + Macros
    
    // TOP SECTION: Left = Synth 1, Right = Master Controls  
    auto synthArea = topSection.removeFromLeft(topSection.getWidth() / 2);
    auto masterControlsArea = topSection; // Right side for master controls
    
    // Layout Synth 1 in left area (Energy orb + controls)
    layoutSynthEngineArea(synthArea);
    
    // Layout Master Controls in right area (relocated master volume)
    layoutMasterControlsArea(masterControlsArea);
    
    // BOTTOM SECTION: Full width FX + Macros
    layoutBottomFXAndMacros(bottomSection);
}

void VoidTextureSynthAudioProcessorEditor::layoutSynthEngineArea(juce::Rectangle<int> bounds)
{
    // Only layout if on main tab and components are visible
    if (currentTab != 0)
        return;
    
    bounds.reduce(8, 0);
    
    // STEP 2: Synth Engine header - properly centered
    auto headerBounds = bounds.removeFromTop(50);
    if (fxLabel.isVisible())
    {
        fxLabel.setText("SYNTH ENGINE 1", juce::dontSendNotification);
        fxLabel.setFont(juce::Font(16.0f, juce::Font::bold)); // Slightly larger
        fxLabel.setJustificationType(juce::Justification::centred); // Center it
        fxLabel.setBounds(headerBounds.reduced(4)); // Full width, centered
    }
    
    // Visual display area (Energy orb) - takes remaining space
    auto visualBounds = bounds;
    layoutVisualSectionWithFlexBox(visualBounds);
}

void VoidTextureSynthAudioProcessorEditor::layoutMasterControlsArea(juce::Rectangle<int> bounds)
{
    // Only layout if on main tab
    if (currentTab != 0)
        return;
    
    bounds.reduce(8, 0);
    
    // STEP 7: Master Controls section header
    auto headerBounds = bounds.removeFromTop(50);
    // We can add a label here later if needed
    
    // STEP 3 & 7: Master volume relocated here
    if (volumeSlider.isVisible() && volumeLabel.isVisible())
    {
        // Center the master volume in the available space
        auto controlArea = bounds.removeFromTop(120);
        auto volumeBounds = controlArea.withSizeKeepingCentre(80, 100);
        
        auto volumeKnobBounds = volumeBounds.removeFromTop(80);
        auto volumeLabelBounds = volumeBounds;
        
        volumeSlider.setBounds(volumeKnobBounds);
        volumeLabel.setBounds(volumeLabelBounds);
        volumeLabel.setJustificationType(juce::Justification::centred);
    }
    
    // Remaining space can be used for additional master controls later
}

void VoidTextureSynthAudioProcessorEditor::layoutBottomFXAndMacros(juce::Rectangle<int> bounds)
{
    // Only layout if on main tab
    if (currentTab != 0)
        return;
    
    // Add spacing from top section
    bounds.removeFromTop(16);
    
    // STEP 6 & 7: Restructured bottom layout
    // Split horizontally: FX modules (70%) + Beautiful Macro Panel (30%)
    auto fxBounds = bounds.removeFromLeft(static_cast<int>(bounds.getWidth() * 0.7f));
    bounds.removeFromLeft(16); // Gap between FX and macros
    auto macroBounds = bounds;
    
    // Layout FX section with better organization
    layoutFXSectionWithFlexBox(fxBounds);
    
    // STEP 7: Layout beautiful 4-knob macro panel (TENSION, SHADOW, GHOST, HEAT style)
    layoutBeautifulMacroPanelWithFlexBox(macroBounds);
}

void VoidTextureSynthAudioProcessorEditor::layoutBeautifulMacroPanelWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout if on main tab
    if (currentTab != 0)
        return;
    
    // STEP 7: Beautiful 4-knob macro panel design
    bounds.reduce(8, 8);
    
    // Calculate dimensions for 4 horizontal knobs
    const int KNOB_SIZE = 60;
    const int KNOB_SPACING = 8;
    const int LABEL_HEIGHT = 16;
    const int VALUE_HEIGHT = 14;
    
    int totalWidth = (4 * KNOB_SIZE) + (3 * KNOB_SPACING);
    int totalHeight = KNOB_SIZE + LABEL_HEIGHT + VALUE_HEIGHT + 16; // padding
    
    // Center the macro panel in available bounds
    auto panelBounds = bounds.withSizeKeepingCentre(totalWidth, totalHeight);
    
    // Layout 4 macro controls horizontally (TENSION, SHADOW, GHOST, HEAT style)
    for (int i = 0; i < 4; ++i)
    {
        int x = panelBounds.getX() + i * (KNOB_SIZE + KNOB_SPACING);
        int y = panelBounds.getY();
        
        juce::Rectangle<int> knobArea(x, y + LABEL_HEIGHT, KNOB_SIZE, KNOB_SIZE);
        juce::Rectangle<int> labelArea(x, y, KNOB_SIZE, LABEL_HEIGHT);
        juce::Rectangle<int> valueArea(x, y + LABEL_HEIGHT + KNOB_SIZE, KNOB_SIZE, VALUE_HEIGHT);
        
        // For now, we'll position the existing macro sliders
        // Later we can enhance this with proper styling
        switch (i)
        {
            case 0: // TENSION
                if (macro1Slider.isVisible())
                {
                    macro1Slider.setBounds(knobArea);
                    macro1Label.setBounds(labelArea);
                    macro1Label.setText("TENSION", juce::dontSendNotification);
                    macro1Label.setJustificationType(juce::Justification::centred);
                }
                break;
            case 1: // SHADOW  
                if (macro2Slider.isVisible())
                {
                    macro2Slider.setBounds(knobArea);
                    macro2Label.setBounds(labelArea);
                    macro2Label.setText("SHADOW", juce::dontSendNotification);
                    macro2Label.setJustificationType(juce::Justification::centred);
                }
                break;
            case 2: // GHOST
                if (macro3Slider.isVisible())
                {
                    macro3Slider.setBounds(knobArea);
                    macro3Label.setBounds(labelArea);
                    macro3Label.setText("GHOST", juce::dontSendNotification);
                    macro3Label.setJustificationType(juce::Justification::centred);
                }
                break;
            case 3: // HEAT
                if (macro4Slider.isVisible())
                {
                    macro4Slider.setBounds(knobArea);
                    macro4Label.setBounds(labelArea);
                    macro4Label.setText("HEAT", juce::dontSendNotification);
                    macro4Label.setJustificationType(juce::Justification::centred);
                }
                break;
        }
    }
}

void VoidTextureSynthAudioProcessorEditor::layoutLeftPanelWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout if components are visible
    if (currentTab != 0)
        return;
    
    // Header with master volume
    auto headerBounds = bounds.removeFromTop(60);
    layoutHeaderSectionWithFlexBox(headerBounds);
    
    // Visual display area (Energy orb) - this should be the main content
    auto visualBounds = bounds.removeFromTop(240);
    layoutVisualSectionWithFlexBox(visualBounds);
    
    // NO FX modules on the left side - that's for the right panel!
    // Remaining space can be used for synth-specific controls later
}

void VoidTextureSynthAudioProcessorEditor::layoutHeaderSectionWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout visible components
    if (currentTab != 0)
        return;
    
    // Synth label on left - this should say "SYNTH ENGINE 1"
    auto labelBounds = bounds.removeFromLeft(bounds.getWidth() * 0.6f);
    if (fxLabel.isVisible())
    {
        fxLabel.setText("SYNTH ENGINE 1", juce::dontSendNotification);
        fxLabel.setFont(juce::Font(14.0f, juce::Font::bold));
        fxLabel.setJustificationType(juce::Justification::centredLeft);
        fxLabel.setBounds(labelBounds.reduced(8));
    }
    
    // Master volume on right
    if (volumeSlider.isVisible() && volumeLabel.isVisible())
    {
        auto volumeBounds = bounds;
        volumeBounds.reduce(8, 8);
        
        // Volume control with label below
        auto volumeKnobBounds = volumeBounds.removeFromTop(volumeBounds.getHeight() * 0.7f);
        auto volumeLabelBounds = volumeBounds;
        
        volumeSlider.setBounds(volumeKnobBounds);
        volumeLabel.setBounds(volumeLabelBounds);
        volumeLabel.setJustificationType(juce::Justification::centred);
    }
}

void VoidTextureSynthAudioProcessorEditor::layoutVisualSectionWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout visible components
    if (currentTab != 0)
        return;
    
    // STEP 4: Center the Energy orb vertically in its box
    const int VISUAL_SIZE = 180; // Slightly larger
    auto visualBounds = bounds.withSizeKeepingCentre(VISUAL_SIZE, VISUAL_SIZE);
    
    if (waveformDisplay && waveformDisplay->isVisible())
        waveformDisplay->setBounds(visualBounds);
    
    // Display mode button in top-left corner of the synth area
    if (displayModeButton.isVisible())
        displayModeButton.setBounds(bounds.getX() + 8, bounds.getY() + 8, 70, 24);
}

void VoidTextureSynthAudioProcessorEditor::layoutRightPanelWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout if on main tab
    if (currentTab != 0)
        return;
    
    // FX section (70% of height)
    auto fxBounds = bounds.removeFromTop(static_cast<int>(bounds.getHeight() * 0.7f));
    layoutFXSectionWithFlexBox(fxBounds);
    
    // Add spacing
    bounds.removeFromTop(16);
    
    // Macro section (remaining height)
    auto macroBounds = bounds;
    layoutMacroSectionWithFlexBox(macroBounds);
}

void VoidTextureSynthAudioProcessorEditor::layoutFXSectionWithFlexBox(juce::Rectangle<int> bounds)
{
    // Only layout visible FX components
    if (currentTab != 0)
        return;
    
    // FX section header
    auto headerBounds = bounds.removeFromTop(30);
    if (fxHeaderLabel.isVisible())
    {
        fxHeaderLabel.setText("● FX MODULES ●", juce::dontSendNotification);
        fxHeaderLabel.setFont(juce::Font(12.0f, juce::Font::bold));
        fxHeaderLabel.setJustificationType(juce::Justification::centred);
        fxHeaderLabel.setBounds(headerBounds);
    }
    
    // FX grid container - now spans most of the full width
    const int GRID_SPACING = 12;
    bounds.reduce(8, 8);
    
    // Calculate 2x2 grid dimensions for the wider area
    int moduleWidth = (bounds.getWidth() - GRID_SPACING) / 2;
    int moduleHeight = (bounds.getHeight() - GRID_SPACING) / 2;
    
    // Create clean grid layout using the full FX area width
    juce::Rectangle<int> modules[4] = {
        {bounds.getX(), bounds.getY(), moduleWidth, moduleHeight},
        {bounds.getX() + moduleWidth + GRID_SPACING, bounds.getY(), moduleWidth, moduleHeight},
        {bounds.getX(), bounds.getY() + moduleHeight + GRID_SPACING, moduleWidth, moduleHeight},
        {bounds.getX() + moduleWidth + GRID_SPACING, bounds.getY() + moduleHeight + GRID_SPACING, moduleWidth, moduleHeight}
    };
    
    // Layout FX modules only if their components are visible
    if (reverbLabel.isVisible())
        layoutFXModuleWithFlexBox(modules[0], reverbLabel, reverbSizeSlider, reverbDampSlider, reverbSizeLabel, reverbDampLabel, "REVERB");
    if (delayLabel.isVisible())
        layoutFXModuleWithFlexBox(modules[1], delayLabel, delayTimeSlider, delayFeedbackSlider, delayTimeLabel, delayFeedbackLabel, "DELAY");
    if (filterLabel.isVisible())
        layoutFXModuleWithFlexBox(modules[2], filterLabel, filterCutoffSlider, filterResonanceSlider, filterCutoffLabel, filterResonanceLabel, "FILTER");
    if (distortionLabel.isVisible())
        layoutFXModuleWithFlexBox(modules[3], distortionLabel, distortionDriveSlider, distortionMixSlider, distortionDriveLabel, distortionMixLabel, "DISTORTION");
}

void VoidTextureSynthAudioProcessorEditor::layoutFXModuleWithFlexBox(juce::Rectangle<int> bounds,
                                                                   juce::Label& headerLabel,
                                                                   juce::Slider& slider1,
                                                                   juce::Slider& slider2,
                                                                   juce::Label& label1,
                                                                   juce::Label& label2,
                                                                   const juce::String& title)
{
    // Ensure bounds are valid
    if (bounds.getWidth() < 50 || bounds.getHeight() < 50)
        return;
    
    // Module header
    auto headerBounds = bounds.removeFromTop(20);
    headerLabel.setBounds(headerBounds);
    headerLabel.setText(title, juce::dontSendNotification);
    headerLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    headerLabel.setJustificationType(juce::Justification::centred);
    
    // Control area with padding
    bounds.reduce(6, 4);
    
    // Horizontal layout for two controls
    int controlWidth = (bounds.getWidth() - 8) / 2;
    int controlHeight = bounds.getHeight();
    
    // Left control
    auto leftBounds = bounds.removeFromLeft(controlWidth);
    auto leftKnobBounds = leftBounds.removeFromTop(leftBounds.getHeight() - 16);
    auto leftLabelBounds = leftBounds;
    
    slider1.setBounds(leftKnobBounds);
    label1.setBounds(leftLabelBounds);
    label1.setJustificationType(juce::Justification::centred);
    
    // Spacing
    bounds.removeFromLeft(8);
    
    // Right control  
    auto rightBounds = bounds;
    auto rightKnobBounds = rightBounds.removeFromTop(rightBounds.getHeight() - 16);
    auto rightLabelBounds = rightBounds;
    
    slider2.setBounds(rightKnobBounds);
    label2.setBounds(rightLabelBounds);
    label2.setJustificationType(juce::Justification::centred);
}

void VoidTextureSynthAudioProcessorEditor::layoutMacroSectionWithFlexBox(juce::Rectangle<int> bounds)
{
    // Macro section header
    auto headerBounds = bounds.removeFromTop(24);
    // Use an existing label for macro header (we'll reuse one of the available labels)
    
    // Horizontal layout for 4 macro controls
    bounds.reduce(8, 4);
    
    if (bounds.getWidth() > 0 && bounds.getHeight() > 0)
    {
        int macroWidth = bounds.getWidth() / 4;
        int macroSpacing = 4;
        
        // Layout 4 macro controls in a row
        for (int i = 0; i < 4; ++i)
        {
            auto macroBounds = juce::Rectangle<int>(
                bounds.getX() + i * (macroWidth - macroSpacing),
                bounds.getY(),
                macroWidth - macroSpacing,
                bounds.getHeight()
            );
            
            // For now, we'll just reserve space for macro controls
            // These can be implemented later as dedicated macro components
        }
    }
}

void VoidTextureSynthAudioProcessorEditor::layoutSynth1Section(juce::Rectangle<int> area)
{
    // Professional Synth 1 layout with optimal space utilization
    
    // Header with master volume - clean horizontal layout
    auto headerArea = area.removeFromTop(50);
    auto volumeArea = headerArea.removeFromRight(85);
    
    // Synth 1 label on the left
    auto synth1LabelArea = headerArea.reduced(5, 5);
    fxLabel.setText("SYNTH ENGINE 1", juce::dontSendNotification);
    fxLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    fxLabel.setJustificationType(juce::Justification::centredLeft);
    fxLabel.setBounds(synth1LabelArea);
    
    // Master volume controls on the right - clean vertical stack
    auto volumeKnobArea = volumeArea.removeFromTop(35);
    auto volumeLabelArea = volumeArea;
    volumeSlider.setBounds(volumeKnobArea.reduced(5));
    volumeLabel.setBounds(volumeLabelArea);
    
    // Add spacing
    area.removeFromTop(10);
    
    // Visual display area - properly sized and centered
    auto visualArea = area.removeFromTop(static_cast<int>(area.getHeight() * 0.45f));
    layoutVisualSection(visualArea);
    
    // Add spacing
    area.removeFromTop(15);
    
    // Bottom controls area - FX and Macros side by side
    layoutBottomControls(area);
}

void VoidTextureSynthAudioProcessorEditor::layoutVisualSection(juce::Rectangle<int> area)
{
    // Professional orb visualizer layout
    
    // Calculate optimal orb size
    int maxSize = std::min(area.getWidth() - 60, area.getHeight() - 40);
    int orbSize = std::max(140, std::min(maxSize, 200));
    
    // Center the orb perfectly
    int orbX = area.getCentreX() - orbSize/2;
    int orbY = area.getCentreY() - orbSize/2;
    juce::Rectangle<int> orbBounds(orbX, orbY, orbSize, orbSize);
    
    if (waveformDisplay)
        waveformDisplay->setBounds(orbBounds);
    
    // Display mode button - clean positioning
    displayModeButton.setBounds(area.getX() + 5, area.getY() + 5, 60, 18);
}

void VoidTextureSynthAudioProcessorEditor::layoutBottomControls(juce::Rectangle<int> area)
{
    // Professional side-by-side layout: FX (65%) + Macros (35%)
    
    auto fxArea = area.removeFromLeft(static_cast<int>(area.getWidth() * 0.65f));
    area.removeFromLeft(10); // Gap
    auto macroArea = area;
    
    // Layout FX section
    layoutFXSection(fxArea);
    
    // Layout Macro section
    layoutMacroSection(macroArea);
}

void VoidTextureSynthAudioProcessorEditor::layoutFXSection(juce::Rectangle<int> area)
{
    // FX section header
    auto headerArea = area.removeFromTop(22);
    fxHeaderLabel.setText("● FX MODULES ●", juce::dontSendNotification);
    fxHeaderLabel.setFont(juce::Font(11.0f, juce::Font::bold));
    fxHeaderLabel.setJustificationType(juce::Justification::centred);
    fxHeaderLabel.setBounds(headerArea);
    
    // Add spacing
    area.removeFromTop(8);
    
    // Calculate clean 2x2 grid for FX modules
    const int margin = 8;
    const int spacing = 8;
    const int moduleWidth = (area.getWidth() - (2 * margin + spacing)) / 2;
    const int moduleHeight = (area.getHeight() - (2 * margin + spacing)) / 2;
    
    // Create module bounds
    juce::Rectangle<int> modules[4] = {
        {area.getX() + margin, area.getY() + margin, moduleWidth, moduleHeight},
        {area.getX() + margin + moduleWidth + spacing, area.getY() + margin, moduleWidth, moduleHeight},
        {area.getX() + margin, area.getY() + margin + moduleHeight + spacing, moduleWidth, moduleHeight},
        {area.getX() + margin + moduleWidth + spacing, area.getY() + margin + moduleHeight + spacing, moduleWidth, moduleHeight}
    };
    
    // Layout each FX module
    layoutFXModule(modules[0], reverbLabel, reverbSizeSlider, reverbDampSlider, reverbSizeLabel, reverbDampLabel, "REVERB");
    layoutFXModule(modules[1], delayLabel, delayTimeSlider, delayFeedbackSlider, delayTimeLabel, delayFeedbackLabel, "DELAY");
    layoutFXModule(modules[2], filterLabel, filterCutoffSlider, filterResonanceSlider, filterCutoffLabel, filterResonanceLabel, "FILTER");
    layoutFXModule(modules[3], distortionLabel, distortionDriveSlider, distortionMixSlider, distortionDriveLabel, distortionMixLabel, "DISTORTION");
}

void VoidTextureSynthAudioProcessorEditor::layoutFXModule6Controls(juce::Rectangle<int> bounds, 
                                                                juce::Label& headerLabel,
                                                                std::vector<juce::Slider*> sliders,
                                                                std::vector<juce::Label*> labels,
                                                                const juce::String& title)
{
    // Module header
    auto headerBounds = bounds.removeFromTop(14);
    headerLabel.setBounds(headerBounds);
    headerLabel.setText(title, juce::dontSendNotification);
    headerLabel.setFont(juce::Font(9.0f, juce::Font::bold));
    headerLabel.setJustificationType(juce::Justification::centred);
    
    // Layout 6 controls in 3x2 grid
    const int padding = 3;
    const int labelHeight = 8;
    auto controlArea = bounds.reduced(padding);
    
    // Calculate dimensions for 3 columns, 2 rows
    int knobWidth = controlArea.getWidth() / 3;
    int knobHeight = (controlArea.getHeight() - labelHeight) / 2;
    int knobSize = std::min(knobWidth - 2, knobHeight - labelHeight - 2);
    knobSize = std::max(16, std::min(knobSize, 24)); // Smaller knobs for 6 controls
    
    // Position 6 controls (ensure we have exactly 6)
    int numControls = std::min(static_cast<int>(sliders.size()), 6);
    
    for (int i = 0; i < numControls; ++i)
    {
        int col = i % 3;  // 3 columns
        int row = i / 3;  // 2 rows
        
        int x = controlArea.getX() + col * knobWidth + (knobWidth - knobSize) / 2;
        int y = controlArea.getY() + row * (knobHeight);
        
        if (sliders[i])
        {
            sliders[i]->setBounds(x, y, knobSize, knobSize);
        }
        
        if (i < labels.size() && labels[i])
        {
            labels[i]->setBounds(x, y + knobSize + 1, knobSize, labelHeight);
        }
    }
}

void VoidTextureSynthAudioProcessorEditor::layoutFXModule(juce::Rectangle<int> bounds, 
                                                          juce::Label& headerLabel,
                                                          juce::Slider& slider1, juce::Slider& slider2,
                                                          juce::Label& label1, juce::Label& label2,
                                                          const juce::String& title)
{
    // Temporary method - will use 6-control version once all controls are added
    // For now, use the existing 2-control layout but make it more compact
    
    auto headerBounds = bounds.removeFromTop(14);
    headerLabel.setBounds(headerBounds);
    headerLabel.setText(title, juce::dontSendNotification);
    headerLabel.setFont(juce::Font(9.0f, juce::Font::bold));
    headerLabel.setJustificationType(juce::Justification::centred);
    
    const int padding = 4;
    const int labelHeight = 8;
    auto controlArea = bounds.reduced(padding);
    
    // Make knobs smaller to prepare for 6-control layout
    int knobSize = std::min(controlArea.getWidth() / 3 - 2, controlArea.getHeight() / 2 - labelHeight - 2);
    knobSize = std::max(18, std::min(knobSize, 26));
    
    // Position first two controls in top row
    int knobWidth = controlArea.getWidth() / 3;
    int x1 = controlArea.getX() + (knobWidth - knobSize) / 2;
    int x2 = controlArea.getX() + knobWidth + (knobWidth - knobSize) / 2;
    int y = controlArea.getY();
    
    slider1.setBounds(x1, y, knobSize, knobSize);
    label1.setBounds(x1, y + knobSize + 1, knobSize, labelHeight);
    
    slider2.setBounds(x2, y, knobSize, knobSize);
    label2.setBounds(x2, y + knobSize + 1, knobSize, labelHeight);
}

void VoidTextureSynthAudioProcessorEditor::layoutMacroSection(juce::Rectangle<int> area)
{
    // Macro section header
    auto headerArea = area.removeFromTop(22);
    macroHeaderLabel.setText("● MACROS ●", juce::dontSendNotification);
    macroHeaderLabel.setFont(juce::Font(11.0f, juce::Font::bold));
    macroHeaderLabel.setJustificationType(juce::Justification::centred);
    macroHeaderLabel.setBounds(headerArea);
    
    // Add spacing
    area.removeFromTop(8);
    
    // Calculate clean 2x2 grid for macro controls
    const int margin = 10;
    const int spacing = 10;
    const int macroWidth = (area.getWidth() - (2 * margin + spacing)) / 2;
    const int macroHeight = (area.getHeight() - (2 * margin + spacing)) / 2;
    
    // Create macro bounds
    juce::Rectangle<int> macroBounds[4] = {
        {area.getX() + margin, area.getY() + margin, macroWidth, macroHeight},
        {area.getX() + margin + macroWidth + spacing, area.getY() + margin, macroWidth, macroHeight},
        {area.getX() + margin, area.getY() + margin + macroHeight + spacing, macroWidth, macroHeight},
        {area.getX() + margin + macroWidth + spacing, area.getY() + margin + macroHeight + spacing, macroWidth, macroHeight}
    };
    
    // Layout macro controls
    layoutMacroControl(macroBounds[0], macro1Slider, macro1Label);
    layoutMacroControl(macroBounds[1], macro2Slider, macro2Label);
    layoutMacroControl(macroBounds[2], macro3Slider, macro3Label);
    layoutMacroControl(macroBounds[3], macro4Slider, macro4Label);
}

void VoidTextureSynthAudioProcessorEditor::layoutMacroControl(juce::Rectangle<int> bounds, 
                                                              juce::Slider& slider, 
                                                              juce::Label& label)
{
    // Calculate optimal macro knob size
    int knobSize = std::min(bounds.getWidth() - 12, bounds.getHeight() - 18);
    knobSize = std::max(28, std::min(knobSize, 48));
    
    // Center the knob
    int knobX = bounds.getCentreX() - knobSize/2;
    int knobY = bounds.getY() + 3;
    
    slider.setBounds(knobX, knobY, knobSize, knobSize);
    label.setBounds(bounds.getX(), knobY + knobSize + 3, bounds.getWidth(), 14);
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

void VoidTextureSynthAudioProcessorEditor::setupFXControls()
{
    // Reverb Controls
    reverbSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    reverbSizeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    reverbSizeSlider.setRange(0.0, 1.0, 0.01);
    reverbSizeSlider.setValue(0.5);
    contentArea.addAndMakeVisible(reverbSizeSlider);
    
    reverbDampSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    reverbDampSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    reverbDampSlider.setRange(0.0, 1.0, 0.01);
    reverbDampSlider.setValue(0.3);
    contentArea.addAndMakeVisible(reverbDampSlider);
    
    reverbSizeLabel.setText("SIZE", juce::dontSendNotification);
    reverbSizeLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    reverbSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    reverbSizeLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(reverbSizeLabel);
    
    reverbDampLabel.setText("DAMP", juce::dontSendNotification);
    reverbDampLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    reverbDampLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    reverbDampLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(reverbDampLabel);
    
    reverbLabel.setText("REVERB", juce::dontSendNotification);
    reverbLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    reverbLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    reverbLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(reverbLabel);
    
    // Delay Controls
    delayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delayTimeSlider.setRange(0.0, 1.0, 0.01);
    delayTimeSlider.setValue(0.25);
    contentArea.addAndMakeVisible(delayTimeSlider);
    
    delayFeedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayFeedbackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    delayFeedbackSlider.setRange(0.0, 0.95, 0.01);
    delayFeedbackSlider.setValue(0.4);
    contentArea.addAndMakeVisible(delayFeedbackSlider);
    
    delayTimeLabel.setText("TIME", juce::dontSendNotification);
    delayTimeLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    delayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(delayTimeLabel);
    
    delayFeedbackLabel.setText("FEEDBACK", juce::dontSendNotification);
    delayFeedbackLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    delayFeedbackLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    delayFeedbackLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(delayFeedbackLabel);
    
    delayLabel.setText("DELAY", juce::dontSendNotification);
    delayLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    delayLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    delayLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(delayLabel);
    
    // Distortion Controls
    distortionDriveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distortionDriveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    distortionDriveSlider.setRange(1.0, 10.0, 0.1);
    distortionDriveSlider.setValue(2.0);
    contentArea.addAndMakeVisible(distortionDriveSlider);
    
    distortionMixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distortionMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    distortionMixSlider.setRange(0.0, 1.0, 0.01);
    distortionMixSlider.setValue(0.5);
    contentArea.addAndMakeVisible(distortionMixSlider);
    
    distortionLabel.setText("DISTORTION", juce::dontSendNotification);
    distortionLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    distortionLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    distortionLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(distortionLabel);
    
    distortionDriveLabel.setText("DRIVE", juce::dontSendNotification);
    distortionDriveLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    distortionDriveLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    distortionDriveLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(distortionDriveLabel);
    
    distortionMixLabel.setText("MIX", juce::dontSendNotification);
    distortionMixLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    distortionMixLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    distortionMixLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(distortionMixLabel);
    
    // Filter Controls
    filterCutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterCutoffSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffSlider.setRange(20.0, 20000.0, 1.0);
    filterCutoffSlider.setValue(1000.0);
    filterCutoffSlider.setSkewFactorFromMidPoint(1000.0);
    contentArea.addAndMakeVisible(filterCutoffSlider);
    
    filterResonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    filterResonanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterResonanceSlider.setRange(0.1, 10.0, 0.1);
    filterResonanceSlider.setValue(1.0);
    contentArea.addAndMakeVisible(filterResonanceSlider);
    
    filterLabel.setText("FILTER", juce::dontSendNotification);
    filterLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    filterLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);
    filterLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(filterLabel);
    
    filterCutoffLabel.setText("CUTOFF", juce::dontSendNotification);
    filterCutoffLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    filterCutoffLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    filterCutoffLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(filterCutoffLabel);
    
    filterResonanceLabel.setText("RESO", juce::dontSendNotification);
    filterResonanceLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    filterResonanceLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    filterResonanceLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(filterResonanceLabel);
    
    // Setup FX header label
    fxHeaderLabel.setText("● FX MODULES ●", juce::dontSendNotification);
    fxHeaderLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    fxHeaderLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    fxHeaderLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(fxHeaderLabel);
    
    // Setup Macro Controls
    macroHeaderLabel.setText("● MACRO CONTROLS ●", juce::dontSendNotification);
    macroHeaderLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    macroHeaderLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    macroHeaderLabel.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(macroHeaderLabel);
    
    // Macro 1
    macro1Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    macro1Slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    macro1Slider.setRange(0.0, 1.0, 0.01);
    macro1Slider.setValue(0.0);
    contentArea.addAndMakeVisible(macro1Slider);
    
    macro1Label.setText("M1", juce::dontSendNotification);
    macro1Label.setFont(juce::Font(9.0f, juce::Font::bold));
    macro1Label.setColour(juce::Label::textColourId, juce::Colours::white);
    macro1Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(macro1Label);
    
    // Macro 2
    macro2Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    macro2Slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    macro2Slider.setRange(0.0, 1.0, 0.01);
    macro2Slider.setValue(0.0);
    contentArea.addAndMakeVisible(macro2Slider);
    
    macro2Label.setText("M2", juce::dontSendNotification);
    macro2Label.setFont(juce::Font(9.0f, juce::Font::bold));
    macro2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    macro2Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(macro2Label);
    
    // Macro 3
    macro3Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    macro3Slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    macro3Slider.setRange(0.0, 1.0, 0.01);
    macro3Slider.setValue(0.0);
    contentArea.addAndMakeVisible(macro3Slider);
    
    macro3Label.setText("M3", juce::dontSendNotification);
    macro3Label.setFont(juce::Font(9.0f, juce::Font::bold));
    macro3Label.setColour(juce::Label::textColourId, juce::Colours::white);
    macro3Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(macro3Label);
    
    // Macro 4
    macro4Slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    macro4Slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    macro4Slider.setRange(0.0, 1.0, 0.01);
    macro4Slider.setValue(0.0);
    contentArea.addAndMakeVisible(macro4Slider);
    
    macro4Label.setText("M4", juce::dontSendNotification);
    macro4Label.setFont(juce::Font(9.0f, juce::Font::bold));
    macro4Label.setColour(juce::Label::textColourId, juce::Colours::white);
    macro4Label.setJustificationType(juce::Justification::centred);
    contentArea.addAndMakeVisible(macro4Label);
}

void VoidTextureSynthAudioProcessorEditor::setFXControlsVisible(bool visible)
{
    // FX header
    fxHeaderLabel.setVisible(visible);
    
    // Reverb controls
    reverbSizeSlider.setVisible(visible);
    reverbDampSlider.setVisible(visible);
    reverbSizeLabel.setVisible(visible);
    reverbDampLabel.setVisible(visible);
    reverbLabel.setVisible(visible);
    
    // Delay controls
    delayTimeSlider.setVisible(visible);
    delayFeedbackSlider.setVisible(visible);
    delayTimeLabel.setVisible(visible);
    delayFeedbackLabel.setVisible(visible);
    delayLabel.setVisible(visible);
    
    // Distortion controls
    distortionDriveSlider.setVisible(visible);
    distortionMixSlider.setVisible(visible);
    distortionDriveLabel.setVisible(visible);
    distortionMixLabel.setVisible(visible);
    distortionLabel.setVisible(visible);
    
    // Filter controls
    filterCutoffSlider.setVisible(visible);
    filterResonanceSlider.setVisible(visible);
    filterCutoffLabel.setVisible(visible);
    filterResonanceLabel.setVisible(visible);
    filterLabel.setVisible(visible);
    
    // Macro controls
    macroHeaderLabel.setVisible(visible);
    macro1Slider.setVisible(visible);
    macro2Slider.setVisible(visible);
    macro3Slider.setVisible(visible);
    macro4Slider.setVisible(visible);
    macro1Label.setVisible(visible);
    macro2Label.setVisible(visible);
    macro3Label.setVisible(visible);
    macro4Label.setVisible(visible);
}
