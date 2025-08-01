// VoidLookAndFeel.cpp - Modernized for Voidwalker
#include "VoidLookAndFeel.h"
#include <random>

// Define static color constants
const juce::Colour VoidLookAndFeel::CosmicColors::backgroundDarkest  = juce::Colour(0xFF050508);
const juce::Colour VoidLookAndFeel::CosmicColors::backgroundDark     = juce::Colour(0xFF0A0A10);
const juce::Colour VoidLookAndFeel::CosmicColors::backgroundMedium   = juce::Colour(0xFF12121A);
const juce::Colour VoidLookAndFeel::CosmicColors::backgroundLight    = juce::Colour(0xFF1A1A25);

const juce::Colour VoidLookAndFeel::CosmicColors::accentBlue         = juce::Colour(0xFF1E90FF);
const juce::Colour VoidLookAndFeel::CosmicColors::accentCyan         = juce::Colour(0xFF00FFFF);
const juce::Colour VoidLookAndFeel::CosmicColors::accentPurple       = juce::Colour(0xFF9370DB);
const juce::Colour VoidLookAndFeel::CosmicColors::accentGreen        = juce::Colour(0xFF00FF9C);

const juce::Colour VoidLookAndFeel::CosmicColors::textPrimary        = juce::Colour(0xFFFFFFFF);
const juce::Colour VoidLookAndFeel::CosmicColors::textSecondary      = juce::Colour(0xFFB0B0C0);
const juce::Colour VoidLookAndFeel::CosmicColors::controlOutline     = juce::Colour(0xFF2A2A35);
const juce::Colour VoidLookAndFeel::CosmicColors::controlFill        = juce::Colour(0xFF151520);

const juce::Colour VoidLookAndFeel::CosmicColors::tabInactive        = juce::Colour(0xFF0F0F18);
const juce::Colour VoidLookAndFeel::CosmicColors::tabActive          = juce::Colour(0xFF1E90FF);

const juce::Colour VoidLookAndFeel::CosmicColors::starDim            = juce::Colour(0xFF342056);
const juce::Colour VoidLookAndFeel::CosmicColors::starBright         = juce::Colour(0xFF6236B2);
const juce::Colour VoidLookAndFeel::CosmicColors::starHighlight      = juce::Colour(0xFF9A6CE0);

VoidLookAndFeel::VoidLookAndFeel() {
    setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    setColour(juce::Label::textColourId, juce::Colours::white);
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::cyan);
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF222222));
    setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
    setColour(juce::ComboBox::textColourId, juce::Colours::white);
    setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
}

VoidLookAndFeel::~VoidLookAndFeel() {}

void VoidLookAndFeel::drawCosmicBackground(juce::Graphics& g, juce::Rectangle<int> bounds, float starDensity)
{
    // Draw the cosmic gradient background first
    juce::Rectangle<float> floatBounds = bounds.toFloat();
    drawCosmicGradientBackground(g, floatBounds);
    
    // Setup random generator with consistent seed for deterministic star pattern
    std::mt19937 rng(42); // Fixed seed for consistent stars
    std::uniform_real_distribution<float> xDist(0, bounds.getWidth());
    std::uniform_real_distribution<float> yDist(0, bounds.getHeight());
    std::uniform_real_distribution<float> sizeDist(0.5f, 3.0f);
    std::uniform_real_distribution<float> brightnessDist(0.0f, 1.0f);
    
    // Calculate number of stars based on area and density
    int totalStars = static_cast<int>(bounds.getWidth() * bounds.getHeight() * starDensity);
    
    // Draw stars with different sizes and colors
    for (int i = 0; i < totalStars; ++i)
    {
        float x = xDist(rng);
        float y = yDist(rng);
        float size = sizeDist(rng);
        float brightness = brightnessDist(rng);
        
        // Choose star color based on brightness
        juce::Colour starColor;
        if (brightness < 0.7f)
            starColor = CosmicColors::starDim;
        else if (brightness < 0.93f)
            starColor = CosmicColors::starBright;
        else
            starColor = CosmicColors::starHighlight;
        
        // Add slight random alpha variation for twinkling effect
        starColor = starColor.withAlpha(0.7f + brightness * 0.3f);
        
        // Draw the star
        g.setColour(starColor);
        
        // Small stars are just points, larger stars get glow
        if (size < 1.0f)
        {
            g.fillEllipse(x, y, size, size);
        }
        else
        {
            // Draw larger stars with a subtle glow effect
            g.fillEllipse(x - size/2, y - size/2, size, size);
            
            // Add a subtle glow for brighter stars
            if (brightness > 0.8f)
            {
                float glowSize = size * 3.0f;
                juce::ColourGradient glow(
                    starColor.withAlpha(0.4f), x, y,
                    starColor.withAlpha(0.0f), x - glowSize/2, y - glowSize/2,
                    true
                );
                g.setGradientFill(glow);
                g.fillEllipse(x - glowSize/2, y - glowSize/2, glowSize, glowSize);
            }
        }
    }
}

void VoidLookAndFeel::drawCosmicGradientBackground(juce::Graphics& g, juce::Rectangle<float> bounds, float intensity)
{
    // Create a radial gradient from center to edges
    juce::Point<float> center = bounds.getCentre();
    float gradientRadius = juce::jmax(bounds.getWidth(), bounds.getHeight()) * 0.7f;
    
    juce::ColourGradient gradient(
        CosmicColors::backgroundDarkest,
        center.x, center.y,
        CosmicColors::backgroundDark,
        center.x + gradientRadius, center.y + gradientRadius,
        true
    );
    
    // Add midpoint colors to create a rich deep space effect
    gradient.addColour(0.4, CosmicColors::backgroundMedium);
    gradient.addColour(0.8, CosmicColors::backgroundLight.withAlpha(0.8f));
    
    g.setGradientFill(gradient);
    g.fillRect(bounds);
    
    // Optional: Add subtle nebula-like swirls for added depth
    if (intensity > 0.5f)
    {
        // Add very subtle colored nebula effect
        juce::Path nebulaPath;
        nebulaPath.startNewSubPath(bounds.getX(), bounds.getY() + bounds.getHeight() * 0.7f);
        nebulaPath.quadraticTo(bounds.getCentreX(), bounds.getCentreY() * 1.2f, 
                           bounds.getRight(), bounds.getY() + bounds.getHeight() * 0.6f);
        
        g.setColour(CosmicColors::accentPurple.withAlpha(0.05f));
        g.strokePath(nebulaPath, juce::PathStrokeType(bounds.getHeight() * 0.4f));
    }
}

void VoidLookAndFeel::drawGlowEffect(juce::Graphics& g, juce::Rectangle<float> bounds, 
                                   juce::Colour color, float intensity)
{
    // Create a radial gradient for the glow effect
    juce::Point<float> center = bounds.getCentre();
    float maxRadius = juce::jmax(bounds.getWidth(), bounds.getHeight()) * 0.6f;
    
    juce::ColourGradient glow(
        color.withAlpha(0.6f * intensity), center.x, center.y,
        color.withAlpha(0.0f), center.x + maxRadius, center.y + maxRadius,
        true
    );
    
    g.setGradientFill(glow);
    g.fillEllipse(bounds.expanded(maxRadius * 0.3f));
}

juce::Path VoidLookAndFeel::createStarPath(float outerRadius, float innerRadius, int numPoints)
{
    juce::Path starPath;
    
    float angle = juce::MathConstants<float>::pi * 2.0f / (numPoints * 2);
    starPath.startNewSubPath(0.0f, -outerRadius); // Top point
    
    for (int i = 1; i < numPoints * 2; ++i)
    {
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float x = std::sin(angle * i) * radius;
        float y = -std::cos(angle * i) * radius;
        starPath.lineTo(x, y);
    }
    
    starPath.closeSubPath();
    return starPath;
}

void VoidLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                                     const juce::Colour& backgroundColour,
                                     bool shouldDrawButtonAsHighlighted, 
                                     bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
    
    // Base colors from cosmic palette
    juce::Colour baseColor = CosmicColors::controlFill;
    juce::Colour borderColor = CosmicColors::controlOutline;
    
    // Adjust colors for state
    if (shouldDrawButtonAsDown)
    {
        baseColor = CosmicColors::accentPurple.withAlpha(0.7f);
        borderColor = CosmicColors::accentPurple;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        baseColor = CosmicColors::controlFill.brighter(0.1f);
        borderColor = CosmicColors::accentBlue;
    }
    
    // Draw button background
    g.setColour(baseColor);
    g.fillRoundedRectangle(bounds, 4.0f);
    
    // Draw border with glow effect for highlighted/down states
    if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
    {
        // Add subtle glow effect
        juce::Colour glowColor = shouldDrawButtonAsDown ? 
            CosmicColors::accentPurple.withAlpha(0.4f) : 
            CosmicColors::accentBlue.withAlpha(0.3f);
            
        g.setColour(glowColor);
        g.drawRoundedRectangle(bounds.expanded(1.5f), 4.0f, 2.0f);
    }
    
    // Draw border
    g.setColour(borderColor);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
}

void VoidLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                  float sliderPos, float minSliderPos, float maxSliderPos,
                                  const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Draw the slider track
    auto bounds = juce::Rectangle<float>(x, y, width, height);
    juce::Rectangle<float> trackBounds;
    
    // Adjust bounds based on slider orientation
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearBar)
    {
        trackBounds = bounds.withSizeKeepingCentre(bounds.getWidth(), 4.0f);
    }
    else // Vertical
    {
        trackBounds = bounds.withSizeKeepingCentre(4.0f, bounds.getHeight());
    }
    
    // Create a track gradient
    juce::ColourGradient trackGradient;
    
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearBar)
    {
        trackGradient = juce::ColourGradient(
            CosmicColors::controlOutline, trackBounds.getX(), trackBounds.getCentreY(),
            CosmicColors::controlOutline.brighter(0.2f), trackBounds.getRight(), trackBounds.getCentreY(),
            false
        );
    }
    else
    {
        trackGradient = juce::ColourGradient(
            CosmicColors::controlOutline, trackBounds.getCentreX(), trackBounds.getBottom(),
            CosmicColors::controlOutline.brighter(0.2f), trackBounds.getCentreX(), trackBounds.getY(),
            false
        );
    }
    
    g.setGradientFill(trackGradient);
    g.fillRoundedRectangle(trackBounds, 2.0f);
    
    // Draw the filled part of the track
    juce::Rectangle<float> filledTrack;
    juce::ColourGradient filledGradient;
    
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearBar)
    {
        filledTrack = trackBounds.withWidth(sliderPos - trackBounds.getX());
        filledGradient = juce::ColourGradient(
            CosmicColors::accentBlue, trackBounds.getX(), trackBounds.getCentreY(),
            CosmicColors::accentCyan, sliderPos, trackBounds.getCentreY(),
            false
        );
    }
    else // Vertical
    {
        float filledHeight = trackBounds.getBottom() - sliderPos;
        filledTrack = trackBounds.withTop(sliderPos).withHeight(filledHeight);
        filledGradient = juce::ColourGradient(
            CosmicColors::accentCyan, trackBounds.getCentreX(), sliderPos,
            CosmicColors::accentBlue, trackBounds.getCentreX(), trackBounds.getBottom(),
            false
        );
    }
    
    g.setGradientFill(filledGradient);
    g.fillRoundedRectangle(filledTrack, 2.0f);
    
    // Draw the thumb
    float thumbWidth = style == juce::Slider::LinearHorizontal ? 12.0f : 8.0f;
    float thumbHeight = style == juce::Slider::LinearHorizontal ? 16.0f : 12.0f;
    
    juce::Rectangle<float> thumbBounds;
    
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearBar)
    {
        thumbBounds = juce::Rectangle<float>(
            sliderPos - thumbWidth / 2, trackBounds.getCentreY() - thumbHeight / 2,
            thumbWidth, thumbHeight);
    }
    else // Vertical
    {
        thumbBounds = juce::Rectangle<float>(
            trackBounds.getCentreX() - thumbWidth / 2, sliderPos - thumbHeight / 2,
            thumbWidth, thumbHeight);
    }
    
    // Thumb fill with gradient
    juce::ColourGradient thumbGradient(
        CosmicColors::controlFill.brighter(0.1f), thumbBounds.getCentreX(), thumbBounds.getY(),
        CosmicColors::controlFill, thumbBounds.getCentreX(), thumbBounds.getBottom(),
        false
    );
    g.setGradientFill(thumbGradient);
    g.fillRoundedRectangle(thumbBounds, 3.0f);
    
    // Thumb border
    g.setColour(CosmicColors::accentCyan);
    g.drawRoundedRectangle(thumbBounds, 3.0f, 1.0f);
    
    // Add a subtle glow for active sliders
    if (slider.isEnabled())
    {
        g.setColour(CosmicColors::accentCyan.withAlpha(0.15f));
        g.drawRoundedRectangle(thumbBounds.expanded(2.0f), 4.0f, 1.0f);
    }
}

void VoidLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                int buttonX, int buttonY, int buttonW, int buttonH,
                                juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float>(0, 0, width, height).reduced(0.5f, 0.5f);
    
    // Base colors from the cosmic palette
    juce::Colour baseColor = CosmicColors::controlFill;
    juce::Colour borderColor = CosmicColors::controlOutline;
    
    // Adjust for state
    if (box.isEnabled())
    {
        if (isButtonDown)
        {
            baseColor = CosmicColors::accentBlue.withAlpha(0.2f);
            borderColor = CosmicColors::accentBlue;
        }
        else if (box.hasKeyboardFocus(true))
        {
            borderColor = CosmicColors::accentCyan;
        }
    }
    else
    {
        baseColor = baseColor.darker(0.2f).withAlpha(0.8f);
        borderColor = borderColor.darker(0.2f);
    }
    
    // Draw combo box background
    g.setColour(baseColor);
    g.fillRoundedRectangle(bounds, 4.0f);
    
    // Draw border
    g.setColour(borderColor);
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
    
    // Draw arrow
    juce::Rectangle<float> arrowBounds(buttonX + buttonW * 0.2f, 
                                     buttonY + buttonH * 0.25f, 
                                     buttonW * 0.6f, 
                                     buttonH * 0.5f);
    
    juce::Path arrow;
    arrow.startNewSubPath(arrowBounds.getX(), arrowBounds.getY());
    arrow.lineTo(arrowBounds.getCentreX(), arrowBounds.getBottom());
    arrow.lineTo(arrowBounds.getRight(), arrowBounds.getY());
    
    g.setColour(box.isEnabled() ? CosmicColors::accentCyan : CosmicColors::textSecondary);
    g.strokePath(arrow, juce::PathStrokeType(1.5f));
}

void VoidLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                    bool shouldDrawButtonAsHighlighted, 
                                    bool shouldDrawButtonAsDown)
{
    // Define the checkbox area
    auto bounds = button.getLocalBounds();
    const int boxSize = juce::jmin(16, bounds.getHeight() - 2);
    juce::Rectangle<float> boxBounds(1.0f, (bounds.getHeight() - boxSize) * 0.5f, boxSize, boxSize);
    
    // Base colors from cosmic palette
    juce::Colour fillColor = CosmicColors::controlFill;
    juce::Colour borderColor = CosmicColors::controlOutline;
    
    // Adjust for state
    if (button.getToggleState())
    {
        fillColor = CosmicColors::accentBlue.withAlpha(0.3f);
        borderColor = CosmicColors::accentBlue;
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        fillColor = fillColor.brighter(0.1f);
        borderColor = CosmicColors::accentCyan.withAlpha(0.6f);
    }
    
    if (shouldDrawButtonAsDown)
    {
        fillColor = fillColor.brighter(0.2f);
    }
    
    // Draw checkbox background
    g.setColour(fillColor);
    g.fillRoundedRectangle(boxBounds, 2.0f);
    
    // Draw checkbox border
    g.setColour(borderColor);
    g.drawRoundedRectangle(boxBounds, 2.0f, 1.0f);
    
    // Draw the tick mark if checked
    if (button.getToggleState())
    {
        juce::Path tickPath;
        float tickThickness = 2.0f;
        
        tickPath.startNewSubPath(boxBounds.getX() + boxSize * 0.2f, 
                              boxBounds.getY() + boxSize * 0.5f);
        tickPath.lineTo(boxBounds.getX() + boxSize * 0.4f, 
                      boxBounds.getY() + boxSize * 0.7f);
        tickPath.lineTo(boxBounds.getX() + boxSize * 0.8f, 
                      boxBounds.getY() + boxSize * 0.2f);
        
        g.setColour(CosmicColors::accentCyan);
        g.strokePath(tickPath, juce::PathStrokeType(tickThickness, 
                                                 juce::PathStrokeType::curved, 
                                                 juce::PathStrokeType::rounded));
        
        // Add a subtle glow
        g.setColour(CosmicColors::accentCyan.withAlpha(0.3f));
        g.strokePath(tickPath, juce::PathStrokeType(tickThickness + 1.0f, 
                                                 juce::PathStrokeType::curved, 
                                                 juce::PathStrokeType::rounded));
    }
    
    // Draw the button text
    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(juce::Font(14.0f)); // Use a fixed font size instead of getting it from the button
    
    // Place text after the checkbox with a little margin
    auto textBounds = bounds.withTrimmedLeft(boxBounds.getRight() + 5.0f);
    g.drawText(button.getButtonText(), textBounds, juce::Justification::centredLeft, true);
}

void VoidLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, const float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(8);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centre = bounds.getCentre();
    float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    // Background
    juce::ColourGradient bgGradient(
        CosmicColors::backgroundDarkest,
        centre.x, centre.y,
        CosmicColors::controlFill,
        bounds.getRight(), bounds.getBottom(),
        true
    );
    
    g.setGradientFill(bgGradient);
    g.fillEllipse(bounds);
    
    // Draw outer rim
    g.setColour(CosmicColors::controlOutline);
    g.drawEllipse(bounds, 1.5f);
    
    // Draw the value arc
    float arcRadius = radius - 2.5f;
    juce::Path valueArc;
    valueArc.addCentredArc(
        centre.x, centre.y,
        arcRadius, arcRadius,
        0.0f,
        rotaryStartAngle, angle,
        true
    );
    
    // Draw the value arc with a gradient from accent color to a brighter variant
    juce::ColourGradient arcGradient(
        CosmicColors::accentBlue,
        centre.x + arcRadius * std::cos(rotaryStartAngle), 
        centre.y + arcRadius * std::sin(rotaryStartAngle),
        CosmicColors::accentCyan,
        centre.x + arcRadius * std::cos(angle), 
        centre.y + arcRadius * std::sin(angle),
        false
    );
    
    g.setGradientFill(arcGradient);
    g.strokePath(valueArc, juce::PathStrokeType(2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Draw pointer line
    juce::Path pointer;
    float pointerLength = radius * 0.8f;
    float pointerThickness = 2.0f;
    
    pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
    
    // Draw pointer with glow effect
    juce::Colour pointerColor = CosmicColors::accentCyan;
    g.setColour(pointerColor);
    g.fillPath(pointer);
    
    // Add a subtle center dot
    float centerDotSize = radius * 0.15f;
    g.setColour(pointerColor.withAlpha(0.8f));
    g.fillEllipse(centre.x - centerDotSize, centre.y - centerDotSize, 
                 centerDotSize * 2.0f, centerDotSize * 2.0f);
    
    // Add subtle glow around the knob when it's at high values
    if (sliderPosProportional > 0.7f)
    {
        float glowIntensity = (sliderPosProportional - 0.7f) / 0.3f;
        juce::ColourGradient glow(
            pointerColor.withAlpha(0.3f * glowIntensity), centre.x, centre.y,
            pointerColor.withAlpha(0.0f), bounds.getX(), bounds.getY(),
            true
        );
        g.setGradientFill(glow);
        g.fillEllipse(bounds.expanded(5.0f * glowIntensity));
    }
}
