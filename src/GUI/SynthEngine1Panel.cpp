#include "SynthEngine1Panel.h"

SynthEngine1Panel::SynthEngine1Panel(juce::AudioProcessorValueTreeState& apvts, SynthEngine1& engine)
    : apvts(apvts), synthEngine(engine)
{
    setupLabels();
    setupLayerControls();
    setupAttachments();
}

SynthEngine1Panel::~SynthEngine1Panel()
{
}

void SynthEngine1Panel::setupLabels()
{
    // Main section label
    sectionLabel.setText("VOID TEXTURE SYNTH", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    sectionLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);
    sectionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sectionLabel);
    
    // Layer labels with larger, more visible text
    oscillatorLabel.setText("OSCILLATOR LAYER", juce::dontSendNotification);
    oscillatorLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    oscillatorLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    oscillatorLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscillatorLabel);
    
    subLabel.setText("SUB OSCILLATOR", juce::dontSendNotification);
    subLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    subLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    subLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subLabel);
    
    noiseLabel.setText("NOISE GENERATOR", juce::dontSendNotification);
    noiseLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    noiseLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    noiseLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noiseLabel);
    
    samplerLabel.setText("SAMPLER", juce::dontSendNotification);
    samplerLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    samplerLabel.setColour(juce::Label::textColourId, juce::Colours::lime);
    samplerLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(samplerLabel);
    
    // Individual control labels - Oscillator
    oscLevelLabel.setText("LEVEL", juce::dontSendNotification);
    oscLevelLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    oscLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    oscLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscLevelLabel);
    
    oscPanLabel.setText("PAN", juce::dontSendNotification);
    oscPanLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    oscPanLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    oscPanLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscPanLabel);
    
    oscWaveformLabel.setText("WAVEFORM", juce::dontSendNotification);
    oscWaveformLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    oscWaveformLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    oscWaveformLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscWaveformLabel);
    
    oscDetuneLabel.setText("DETUNE", juce::dontSendNotification);
    oscDetuneLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    oscDetuneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    oscDetuneLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscDetuneLabel);
    
    oscOctaveLabel.setText("OCTAVE", juce::dontSendNotification);
    oscOctaveLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    oscOctaveLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    oscOctaveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscOctaveLabel);
    
    // Sub oscillator control labels
    subLevelLabel.setText("LEVEL", juce::dontSendNotification);
    subLevelLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    subLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    subLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subLevelLabel);
    
    subPanLabel.setText("PAN", juce::dontSendNotification);
    subPanLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    subPanLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    subPanLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subPanLabel);
    
    subWaveformLabel.setText("WAVEFORM", juce::dontSendNotification);
    subWaveformLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    subWaveformLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    subWaveformLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subWaveformLabel);
    
    subOctaveLabel.setText("OCTAVE", juce::dontSendNotification);
    subOctaveLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    subOctaveLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    subOctaveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subOctaveLabel);
    
    // Noise control labels
    noiseLevelLabel.setText("LEVEL", juce::dontSendNotification);
    noiseLevelLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    noiseLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    noiseLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noiseLevelLabel);
    
    noisePanLabel.setText("PAN", juce::dontSendNotification);
    noisePanLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    noisePanLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    noisePanLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noisePanLabel);
    
    noiseTypeLabel.setText("TYPE", juce::dontSendNotification);
    noiseTypeLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    noiseTypeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    noiseTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noiseTypeLabel);
    
    noiseFilterLabel.setText("FILTER", juce::dontSendNotification);
    noiseFilterLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    noiseFilterLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    noiseFilterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(noiseFilterLabel);
    
    // Sampler control labels
    samplerLevelLabel.setText("LEVEL", juce::dontSendNotification);
    samplerLevelLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    samplerLevelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    samplerLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(samplerLevelLabel);
    
    samplerPanLabel.setText("PAN", juce::dontSendNotification);
    samplerPanLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    samplerPanLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    samplerPanLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(samplerPanLabel);
}

void SynthEngine1Panel::setupLayerControls()
{
    // Oscillator Layer Controls
    oscillatorEnable.setButtonText("ON");
    oscillatorEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
    oscillatorEnable.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    oscillatorEnable.setTooltip("Enable/Disable Oscillator Layer");
    addAndMakeVisible(oscillatorEnable);
    
    oscillatorLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    oscillatorLevel.setRange(0.0, 1.0, 0.01);
    oscillatorLevel.setValue(0.7);
    oscillatorLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    oscillatorLevel.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    oscillatorLevel.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    oscillatorLevel.setTooltip("Oscillator Level");
    addAndMakeVisible(oscillatorLevel);
    // Tools button for oscillator layer
    oscillatorSettingsButton = std::make_unique<juce::TextButton>("⚙");
    oscillatorSettingsButton->setTooltip("Oscillator settings");
    oscillatorSettingsButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*oscillatorSettingsButton);
    // Randomize button for oscillator layer
    oscillatorRandomizeButton = std::make_unique<juce::TextButton>("R");
    oscillatorRandomizeButton->setTooltip("Randomize oscillator parameters");
    oscillatorRandomizeButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*oscillatorRandomizeButton);
    
    oscillatorPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    oscillatorPan.setRange(-1.0, 1.0, 0.01);
    oscillatorPan.setValue(0.0);
    oscillatorPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    oscillatorPan.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    oscillatorPan.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    oscillatorPan.setTooltip("Oscillator Pan Position");
    addAndMakeVisible(oscillatorPan);
    
    oscillatorWaveform.addItem("Saw", 1);
    oscillatorWaveform.addItem("Square", 2);
    oscillatorWaveform.addItem("Triangle", 3);
    oscillatorWaveform.addItem("Sine", 4);
    oscillatorWaveform.setSelectedId(1);
    oscillatorWaveform.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2A2A2A));
    oscillatorWaveform.setColour(juce::ComboBox::textColourId, juce::Colours::orange);
    oscillatorWaveform.setColour(juce::ComboBox::outlineColourId, juce::Colour(0x33FFFFFF));
    oscillatorWaveform.setTooltip("Oscillator Waveform");
    addAndMakeVisible(oscillatorWaveform);
    
    oscillatorDetune.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorDetune.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    oscillatorDetune.setRange(-50.0, 50.0, 1.0);
    oscillatorDetune.setValue(0.0);
    oscillatorDetune.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    oscillatorDetune.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    oscillatorDetune.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    oscillatorDetune.setTooltip("Oscillator Detune Amount (cents)");
    addAndMakeVisible(oscillatorDetune);
    
    oscillatorOctave.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorOctave.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    oscillatorOctave.setRange(-3.0, 3.0, 1.0);
    oscillatorOctave.setValue(0.0);
    oscillatorOctave.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    oscillatorOctave.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    oscillatorOctave.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    oscillatorOctave.setTooltip("Oscillator Octave Shift");
    addAndMakeVisible(oscillatorOctave);
    
    // Sub Oscillator Controls
    subEnable.setButtonText("ON");
    subEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::red);
    subEnable.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    subEnable.setTooltip("Enable/Disable Sub Oscillator Layer");
    addAndMakeVisible(subEnable);
    // Tools button for sub oscillator layer
    subSettingsButton = std::make_unique<juce::TextButton>("⚙");
    subSettingsButton->setTooltip("Sub oscillator settings");
    subSettingsButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*subSettingsButton);
    // Randomize button for sub oscillator layer
    subRandomizeButton = std::make_unique<juce::TextButton>("R");
    subRandomizeButton->setTooltip("Randomize sub oscillator parameters");
    subRandomizeButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*subRandomizeButton);
    
    subLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    subLevel.setRange(0.0, 1.0, 0.01);
    subLevel.setValue(0.5);
    subLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    subLevel.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    subLevel.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    subLevel.setTooltip("Sub Oscillator Level");
    addAndMakeVisible(subLevel);
    
    subPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    subPan.setRange(-1.0, 1.0, 0.01);
    subPan.setValue(0.0);
    subPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    subPan.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    subPan.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    subPan.setTooltip("Sub Oscillator Pan Position");
    addAndMakeVisible(subPan);
    
    subWaveform.addItem("Saw", 1);
    subWaveform.addItem("Square", 2);
    subWaveform.addItem("Triangle", 3);
    subWaveform.addItem("Sine", 4);
    subWaveform.setSelectedId(4);
    subWaveform.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2A2A2A));
    subWaveform.setColour(juce::ComboBox::textColourId, juce::Colours::red);
    addAndMakeVisible(subWaveform);
    
    subOctaveOffset.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subOctaveOffset.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    subOctaveOffset.setRange(-2.0, 1.0, 1.0);
    subOctaveOffset.setValue(-1.0);
    subOctaveOffset.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    addAndMakeVisible(subOctaveOffset);
    
    // Noise Layer Controls
    noiseEnable.setButtonText("");
    noiseEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::yellow);
    addAndMakeVisible(noiseEnable);
    // Tools button for noise generator layer
    noiseSettingsButton = std::make_unique<juce::TextButton>("⚙");
    noiseSettingsButton->setTooltip("Noise generator settings");
    noiseSettingsButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*noiseSettingsButton);
    // Randomize button for noise generator layer
    noiseRandomizeButton = std::make_unique<juce::TextButton>("R");
    noiseRandomizeButton->setTooltip("Randomize noise generator parameters");
    noiseRandomizeButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*noiseRandomizeButton);
    
    noiseLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noiseLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    noiseLevel.setRange(0.0, 1.0, 0.01);
    noiseLevel.setValue(0.3);
    noiseLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    noiseLevel.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    noiseLevel.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    noiseLevel.setTooltip("Noise Level");
    addAndMakeVisible(noiseLevel);
    
    noisePan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noisePan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    noisePan.setRange(-1.0, 1.0, 0.01);
    noisePan.setValue(0.0);
    noisePan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    noisePan.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    noisePan.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    noisePan.setTooltip("Noise Pan Position");
    addAndMakeVisible(noisePan);
    
    noiseType.addItem("White", 1);
    noiseType.addItem("Pink", 2);
    noiseType.addItem("Brown", 3);
    noiseType.addItem("Blue", 4);
    noiseType.setSelectedId(2);  // Default to Pink for ambient textures
    noiseType.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2A2A2A));
    noiseType.setColour(juce::ComboBox::textColourId, juce::Colours::yellow);
    noiseType.setColour(juce::ComboBox::outlineColourId, juce::Colour(0x33FFFFFF));
    noiseType.setTooltip("Noise Type");
    addAndMakeVisible(noiseType);
    
    noiseFilterCutoff.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noiseFilterCutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    noiseFilterCutoff.setRange(100.0, 20000.0, 10.0);
    noiseFilterCutoff.setValue(5000.0);
    noiseFilterCutoff.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    noiseFilterCutoff.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    noiseFilterCutoff.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    noiseFilterCutoff.setSkewFactorFromMidPoint(1000.0); // More natural filter cutoff response
    noiseFilterCutoff.setTooltip("Noise Filter Cutoff Frequency");
    addAndMakeVisible(noiseFilterCutoff);
    
    // Sampler Layer Controls
    samplerEnable.setButtonText("ON");
    samplerEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::lime);
    samplerEnable.setColour(juce::ToggleButton::textColourId, juce::Colours::white);
    samplerEnable.setTooltip("Enable/Disable Sampler Layer");
    addAndMakeVisible(samplerEnable);
    // Tools button for sampler layer
    samplerSettingsButton = std::make_unique<juce::TextButton>("⚙");
    samplerSettingsButton->setTooltip("Sampler settings");
    samplerSettingsButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*samplerSettingsButton);
    // Randomize button for sampler layer
    samplerRandomizeButton = std::make_unique<juce::TextButton>("R");
    samplerRandomizeButton->setTooltip("Randomize sampler parameters");
    samplerRandomizeButton->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    addAndMakeVisible(*samplerRandomizeButton);
    
    samplerLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    samplerLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    samplerLevel.setRange(0.0, 1.0, 0.01);
    samplerLevel.setValue(0.0);
    samplerLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lime);
    samplerLevel.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    samplerLevel.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    samplerLevel.setTooltip("Sampler Level");
    addAndMakeVisible(samplerLevel);
    
    samplerPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    samplerPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    samplerPan.setRange(-1.0, 1.0, 0.01);
    samplerPan.setValue(0.0);
    samplerPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lime);
    samplerPan.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    samplerPan.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x33FFFFFF));
    samplerPan.setTooltip("Sampler Pan Position");
    addAndMakeVisible(samplerPan);
}

void SynthEngine1Panel::setupAttachments()
{
    // Level attachments
    oscillatorLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "osc1Level", oscillatorLevel);
    subLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "subLevel", subLevel);
    noiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "noiseLevel", noiseLevel);
    samplerLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "samplerLevel", samplerLevel);
    
    // Pan attachments
    oscillatorPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "osc1Pan", oscillatorPan);
    subPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "subPan", subPan);
    noisePanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "noisePan", noisePan);
    samplerPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "samplerPan", samplerPan);
    
    // Oscillator specific attachments
    oscillatorDetuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "osc1Detune", oscillatorDetune);
    oscillatorOctaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "osc1Octave", oscillatorOctave);
    subOctaveOffsetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "subOctaveOffset", subOctaveOffset);
    noiseFilterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "noiseFilterCutoff", noiseFilterCutoff);
    
    // Waveform/type attachments
    oscillatorWaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "osc1Waveform", oscillatorWaveform);
    subWaveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "subWaveform", subWaveform);
    noiseTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "noiseType", noiseType);
    
    // Enable attachments
    oscillatorEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "osc1Enable", oscillatorEnable);
    subEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "subEnable", subEnable);
    noiseEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "noiseEnable", noiseEnable);
    samplerEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "samplerEnable", samplerEnable);
}

void SynthEngine1Panel::paint(juce::Graphics& g)
{
    // Dark modern background with subtle gradient
    juce::ColourGradient gradient(juce::Colour(0xFF121218), 0.0f, 0.0f, 
                                 juce::Colour(0xFF1A1A24), getWidth(), getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();
    
    // Add subtle texture
    g.setColour(juce::Colour(0x08FFFFFF));
    for (int i = 0; i < getHeight(); i += 3)
        g.drawHorizontalLine(i, 0, getWidth());
    
    // Draw section backgrounds with rounded corners
    g.setColour(juce::Colour(0xFF1E1E28));
    
    // Oscillator section
    juce::Rectangle<int> oscSection(20, 50, getWidth() - 40, 140);
    g.fillRoundedRectangle(oscSection.toFloat(), 10.0f);
    
    // Sub section
    juce::Rectangle<int> subSection(20, 200, getWidth() - 40, 140);
    g.fillRoundedRectangle(subSection.toFloat(), 10.0f);
    
    // Noise section
    juce::Rectangle<int> noiseSection(20, 350, getWidth() - 40, 140);
    g.fillRoundedRectangle(noiseSection.toFloat(), 10.0f);
    
    // Sampler section
    juce::Rectangle<int> samplerSection(20, 500, getWidth() - 40, 140);
    g.fillRoundedRectangle(samplerSection.toFloat(), 10.0f);
    
    // Add some subtle decorative lines
    g.setColour(juce::Colour(0xFF333344));
    for (int y : {190, 340, 490, 640}) {
        g.drawLine(40, y, getWidth() - 40, y, 1.0f);
    }
}

void SynthEngine1Panel::resized()
{
    auto bounds = getLocalBounds();
    
    // Main section label at top
    sectionLabel.setBounds(bounds.removeFromTop(50).reduced(10));
    
    // Layout for each layer - vertical stack with improved control spacing
    const int layerHeight = 140;
    const int controlHeight = 100;
    const int controlWidth = 80;
    const int padding = 15; // Increased padding for better spacing
    const int labelHeight = 20;
    const int enableWidth = 50;
    
    // OSCILLATOR LAYER SECTION
    auto oscSectionBounds = bounds.removeFromTop(layerHeight).reduced(20, 5);
    oscillatorLabel.setBounds(oscSectionBounds.removeFromTop(30));
    
    auto oscRowBounds = oscSectionBounds.reduced(padding, 0);
    
    // Position each control with spacing
    auto enableBounds = oscRowBounds.removeFromLeft(enableWidth);
    enableBounds = enableBounds.withY(oscillatorLabel.getBottom() + 10);
    oscillatorEnable.setBounds(enableBounds);
    // Settings and randomize buttons for oscillator
    if (oscillatorSettingsButton)
        oscillatorSettingsButton->setBounds(enableBounds.getRight() + 5,
                                           enableBounds.getY(),
                                           18, 18);
    if (oscillatorRandomizeButton)
        oscillatorRandomizeButton->setBounds(enableBounds.getRight() + 25,
                                             enableBounds.getY(),
                                             18, 18);
    
    // Level control with label - ensure there's enough room for interaction
    auto levelBounds = oscRowBounds.removeFromLeft(controlWidth).reduced(2);
    oscLevelLabel.setBounds(levelBounds.removeFromTop(labelHeight));
    oscillatorLevel.setBounds(levelBounds.reduced(0, 2));
    
    // Add spacing
    oscRowBounds.removeFromLeft(padding);
    
    // Pan control with label - ensure there's enough room for interaction
    auto panBounds = oscRowBounds.removeFromLeft(controlWidth).reduced(2);
    oscPanLabel.setBounds(panBounds.removeFromTop(labelHeight));
    oscillatorPan.setBounds(panBounds.reduced(0, 2));
    
    // Add spacing
    oscRowBounds.removeFromLeft(padding);
    
    // Waveform combo box with label
    auto waveformBounds = oscRowBounds.removeFromLeft(controlWidth + 20).reduced(2);
    oscWaveformLabel.setBounds(waveformBounds.removeFromTop(labelHeight));
    oscillatorWaveform.setBounds(waveformBounds.removeFromTop(30));
    
    // Add spacing
    oscRowBounds.removeFromLeft(padding);
    
    // Detune control with label
    auto detuneBounds = oscRowBounds.removeFromLeft(controlWidth).reduced(2);
    oscDetuneLabel.setBounds(detuneBounds.removeFromTop(labelHeight));
    oscillatorDetune.setBounds(detuneBounds.reduced(0, 2));
    
    // Add spacing
    oscRowBounds.removeFromLeft(padding);
    
    // Octave control with label
    auto octaveBounds = oscRowBounds.removeFromLeft(controlWidth).reduced(2);
    oscOctaveLabel.setBounds(octaveBounds.removeFromTop(labelHeight));
    oscillatorOctave.setBounds(octaveBounds.reduced(0, 2));
    
    // SUB OSCILLATOR LAYER SECTION
    bounds.removeFromTop(10); // spacing between sections
    auto subSectionBounds = bounds.removeFromTop(layerHeight).reduced(20, 5);
    subLabel.setBounds(subSectionBounds.removeFromTop(30));
    
    auto subRowBounds = subSectionBounds.reduced(padding, 0);
    
    // Position each control with spacing
    auto subEnableBounds = subRowBounds.removeFromLeft(enableWidth);
    subEnableBounds = subEnableBounds.withY(subLabel.getBottom() + 10);
    subEnable.setBounds(subEnableBounds);
    
    // Level control with label
    auto subLevelBounds = subRowBounds.removeFromLeft(controlWidth).reduced(2);
    subLevelLabel.setBounds(subLevelBounds.removeFromTop(labelHeight));
    subLevel.setBounds(subLevelBounds.reduced(0, 2));
    
    // Add spacing
    subRowBounds.removeFromLeft(padding);
    
    // Pan control with label
    auto subPanBounds = subRowBounds.removeFromLeft(controlWidth).reduced(2);
    subPanLabel.setBounds(subPanBounds.removeFromTop(labelHeight));
    subPan.setBounds(subPanBounds.reduced(0, 2));
    
    // Add spacing
    subRowBounds.removeFromLeft(padding);
    
    // Waveform combo box with label
    auto subWaveformBounds = subRowBounds.removeFromLeft(controlWidth + 20).reduced(2);
    subWaveformLabel.setBounds(subWaveformBounds.removeFromTop(labelHeight));
    subWaveform.setBounds(subWaveformBounds.removeFromTop(30));
    
    // Add spacing
    subRowBounds.removeFromLeft(padding);
    
    // Octave control with label
    auto subOctaveBounds = subRowBounds.removeFromLeft(controlWidth).reduced(2);
    subOctaveLabel.setBounds(subOctaveBounds.removeFromTop(labelHeight));
    subOctaveOffset.setBounds(subOctaveBounds.reduced(0, 2));
    
    // NOISE LAYER SECTION
    bounds.removeFromTop(10); // spacing between sections
    auto noiseSectionBounds = bounds.removeFromTop(layerHeight).reduced(20, 5);
    noiseLabel.setBounds(noiseSectionBounds.removeFromTop(30));
    
    auto noiseRowBounds = noiseSectionBounds.reduced(padding, 0);
    
    // Position each control with spacing
    auto noiseEnableBounds = noiseRowBounds.removeFromLeft(enableWidth);
    noiseEnableBounds = noiseEnableBounds.withY(noiseLabel.getBottom() + 10);
    noiseEnable.setBounds(noiseEnableBounds);
    
    // Level control with label
    auto noiseLevelBounds = noiseRowBounds.removeFromLeft(controlWidth).reduced(2);
    noiseLevelLabel.setBounds(noiseLevelBounds.removeFromTop(labelHeight));
    noiseLevel.setBounds(noiseLevelBounds.reduced(0, 2));
    
    // Add spacing
    noiseRowBounds.removeFromLeft(padding);
    
    // Pan control with label
    auto noisePanBounds = noiseRowBounds.removeFromLeft(controlWidth).reduced(2);
    noisePanLabel.setBounds(noisePanBounds.removeFromTop(labelHeight));
    noisePan.setBounds(noisePanBounds.reduced(0, 2));
    
    // Add spacing
    noiseRowBounds.removeFromLeft(padding);
    
    // Type combo box with label
    auto noiseTypeBounds = noiseRowBounds.removeFromLeft(controlWidth + 20).reduced(2);
    noiseTypeLabel.setBounds(noiseTypeBounds.removeFromTop(labelHeight));
    noiseType.setBounds(noiseTypeBounds.removeFromTop(30));
    
    // Add spacing
    noiseRowBounds.removeFromLeft(padding);
    
    // Filter control with label
    auto noiseFilterBounds = noiseRowBounds.removeFromLeft(controlWidth).reduced(2);
    noiseFilterLabel.setBounds(noiseFilterBounds.removeFromTop(labelHeight));
    noiseFilterCutoff.setBounds(noiseFilterBounds.reduced(0, 2));
    
    // SAMPLER LAYER SECTION
    bounds.removeFromTop(10); // spacing between sections
    auto samplerSectionBounds = bounds.removeFromTop(layerHeight).reduced(20, 5);
    samplerLabel.setBounds(samplerSectionBounds.removeFromTop(30));
    
    auto samplerRowBounds = samplerSectionBounds.reduced(padding, 0);
    
    // Position each control with spacing
    auto samplerEnableBounds = samplerRowBounds.removeFromLeft(enableWidth);
    samplerEnableBounds = samplerEnableBounds.withY(samplerLabel.getBottom() + 10);
    samplerEnable.setBounds(samplerEnableBounds);
    
    // Level control with label
    auto samplerLevelBounds = samplerRowBounds.removeFromLeft(controlWidth).reduced(2);
    samplerLevelLabel.setBounds(samplerLevelBounds.removeFromTop(labelHeight));
    samplerLevel.setBounds(samplerLevelBounds.reduced(0, 2));
    
    // Add spacing
    samplerRowBounds.removeFromLeft(padding);
    
    // Pan control with label
    auto samplerPanBounds = samplerRowBounds.removeFromLeft(controlWidth).reduced(2);
    samplerPanLabel.setBounds(samplerPanBounds.removeFromTop(labelHeight));
    samplerPan.setBounds(samplerPanBounds.reduced(0, 2));
}
