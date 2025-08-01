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
    sectionLabel.setText("SYNTH ENGINE 1 - LAYER CONFIGURATION", juce::dontSendNotification);
    sectionLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    sectionLabel.setColour(juce::Label::textColourId, juce::Colours::cyan);
    sectionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(sectionLabel);
    
    // Column headers
    enableLabel.setText("ENABLE", juce::dontSendNotification);
    enableLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    enableLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    enableLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(enableLabel);
    
    levelLabel.setText("LEVEL", juce::dontSendNotification);
    levelLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    levelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    levelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(levelLabel);
    
    panLabel.setText("PAN", juce::dontSendNotification);
    panLabel.setFont(juce::Font(10.0f, juce::Font::bold));
    panLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    panLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(panLabel);
    
    // Layer labels
    oscillatorLabel.setText("OSCILLATOR", juce::dontSendNotification);
    oscillatorLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    oscillatorLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    oscillatorLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(oscillatorLabel);
    
    subLabel.setText("SUB OSC", juce::dontSendNotification);
    subLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    subLabel.setColour(juce::Label::textColourId, juce::Colours::red);
    subLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(subLabel);
    
    noiseLabel.setText("NOISE", juce::dontSendNotification);
    noiseLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    noiseLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    noiseLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(noiseLabel);
    
    samplerLabel.setText("SAMPLER", juce::dontSendNotification);
    samplerLabel.setFont(juce::Font(12.0f, juce::Font::bold));
    samplerLabel.setColour(juce::Label::textColourId, juce::Colours::lime);
    samplerLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(samplerLabel);
}

void SynthEngine1Panel::setupLayerControls()
{
    // Oscillator Layer Controls
    oscillatorEnable.setButtonText("");
    oscillatorEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorEnable);
    
    oscillatorLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    oscillatorLevel.setRange(0.0, 1.0, 0.01);
    oscillatorLevel.setValue(0.7);
    oscillatorLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorLevel);
    
    oscillatorPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    oscillatorPan.setRange(-1.0, 1.0, 0.01);
    oscillatorPan.setValue(0.0);
    oscillatorPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorPan);
    
    oscillatorWaveform.addItem("Saw", 1);
    oscillatorWaveform.addItem("Square", 2);
    oscillatorWaveform.addItem("Triangle", 3);
    oscillatorWaveform.addItem("Sine", 4);
    oscillatorWaveform.setSelectedId(1);
    oscillatorWaveform.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2A2A2A));
    oscillatorWaveform.setColour(juce::ComboBox::textColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorWaveform);
    
    oscillatorDetune.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorDetune.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    oscillatorDetune.setRange(-50.0, 50.0, 1.0);
    oscillatorDetune.setValue(0.0);
    oscillatorDetune.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorDetune);
    
    oscillatorOctave.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    oscillatorOctave.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    oscillatorOctave.setRange(-3.0, 3.0, 1.0);
    oscillatorOctave.setValue(0.0);
    oscillatorOctave.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    addAndMakeVisible(oscillatorOctave);
    
    // Sub Oscillator Controls
    subEnable.setButtonText("");
    subEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::red);
    addAndMakeVisible(subEnable);
    
    subLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    subLevel.setRange(0.0, 1.0, 0.01);
    subLevel.setValue(0.5);
    subLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
    addAndMakeVisible(subLevel);
    
    subPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    subPan.setRange(-1.0, 1.0, 0.01);
    subPan.setValue(0.0);
    subPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::red);
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
    
    noiseLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noiseLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    noiseLevel.setRange(0.0, 1.0, 0.01);
    noiseLevel.setValue(0.3);
    noiseLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    addAndMakeVisible(noiseLevel);
    
    noisePan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noisePan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    noisePan.setRange(-1.0, 1.0, 0.01);
    noisePan.setValue(0.0);
    noisePan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    addAndMakeVisible(noisePan);
    
    noiseType.addItem("White", 1);
    noiseType.addItem("Pink", 2);
    noiseType.addItem("Brown", 3);
    noiseType.addItem("Blue", 4);
    noiseType.setSelectedId(1);
    noiseType.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF2A2A2A));
    noiseType.setColour(juce::ComboBox::textColourId, juce::Colours::yellow);
    addAndMakeVisible(noiseType);
    
    noiseFilterCutoff.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    noiseFilterCutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    noiseFilterCutoff.setRange(100.0, 20000.0, 10.0);
    noiseFilterCutoff.setValue(5000.0);
    noiseFilterCutoff.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    addAndMakeVisible(noiseFilterCutoff);
    
    // Sampler Layer Controls
    samplerEnable.setButtonText("");
    samplerEnable.setColour(juce::ToggleButton::tickColourId, juce::Colours::lime);
    addAndMakeVisible(samplerEnable);
    
    samplerLevel.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    samplerLevel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    samplerLevel.setRange(0.0, 1.0, 0.01);
    samplerLevel.setValue(0.0);
    samplerLevel.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lime);
    addAndMakeVisible(samplerLevel);
    
    samplerPan.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    samplerPan.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    samplerPan.setRange(-1.0, 1.0, 0.01);
    samplerPan.setValue(0.0);
    samplerPan.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lime);
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
    // Dark background with subtle texture
    g.fillAll(juce::Colour(0xFF0A0A0F));
    
    // Draw section separators
    g.setColour(juce::Colour(0xFF333333));
    g.drawLine(20, 80, getWidth() - 20, 80, 1.0f);
    g.drawLine(20, 140, getWidth() - 20, 140, 1.0f);
    g.drawLine(20, 200, getWidth() - 20, 200, 1.0f);
    g.drawLine(20, 260, getWidth() - 20, 260, 1.0f);
    g.drawLine(20, 320, getWidth() - 20, 320, 1.0f);
    
    // Draw vertical separators for columns
    int columnWidth = (getWidth() - 160) / 5;
    for (int i = 1; i < 5; ++i) {
        int x = 120 + (i * columnWidth);
        g.drawLine(x, 60, x, 320, 1.0f);
    }
}

void SynthEngine1Panel::resized()
{
    auto bounds = getLocalBounds();
    
    // Main section label
    sectionLabel.setBounds(bounds.removeFromTop(40).reduced(10));
    
    // Column headers
    auto headerArea = bounds.removeFromTop(20);
    enableLabel.setBounds(120, headerArea.getY(), 60, 20);
    levelLabel.setBounds(190, headerArea.getY(), 80, 20);
    panLabel.setBounds(280, headerArea.getY(), 80, 20);
    
    auto layerHeight = 60;
    auto rowY = headerArea.getBottom();
    
    // Oscillator layer
    oscillatorLabel.setBounds(20, rowY, 90, layerHeight);
    oscillatorEnable.setBounds(130, rowY + 20, 40, 20);
    oscillatorLevel.setBounds(190, rowY + 10, 60, 40);
    oscillatorPan.setBounds(280, rowY + 10, 60, 40);
    oscillatorWaveform.setBounds(370, rowY + 10, 80, 20);
    oscillatorDetune.setBounds(460, rowY + 10, 60, 40);
    oscillatorOctave.setBounds(530, rowY + 10, 60, 40);
    rowY += layerHeight;
    
    // Sub oscillator layer
    subLabel.setBounds(20, rowY, 90, layerHeight);
    subEnable.setBounds(130, rowY + 20, 40, 20);
    subLevel.setBounds(190, rowY + 10, 60, 40);
    subPan.setBounds(280, rowY + 10, 60, 40);
    subWaveform.setBounds(370, rowY + 10, 80, 20);
    subOctaveOffset.setBounds(460, rowY + 10, 60, 40);
    rowY += layerHeight;
    
    // Noise layer
    noiseLabel.setBounds(20, rowY, 90, layerHeight);
    noiseEnable.setBounds(130, rowY + 20, 40, 20);
    noiseLevel.setBounds(190, rowY + 10, 60, 40);
    noisePan.setBounds(280, rowY + 10, 60, 40);
    noiseType.setBounds(370, rowY + 10, 80, 20);
    noiseFilterCutoff.setBounds(460, rowY + 10, 60, 40);
    rowY += layerHeight;
    
    // Sampler layer
    samplerLabel.setBounds(20, rowY, 90, layerHeight);
    samplerEnable.setBounds(130, rowY + 20, 40, 20);
    samplerLevel.setBounds(190, rowY + 10, 60, 40);
    samplerPan.setBounds(280, rowY + 10, 60, 40);
}
