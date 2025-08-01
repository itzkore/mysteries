#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * VoidLookAndFeel - Custom styling for the VoidTextureSynth plugin
 * Implements the Cosmic Dark Void Aesthetic with deep space gradients,
 * electric accent glows, and particle effects.
 */
class VoidLookAndFeel : public juce::LookAndFeel_V4
{
public:
    //==============================================================================
    // Color palette for the Cosmic Dark Void Aesthetic
    struct CosmicColors {
        // Deep space backgrounds
        static const juce::Colour backgroundDarkest;  // Deepest void black
        static const juce::Colour backgroundDark;     // Dark void
        static const juce::Colour backgroundMedium;   // Dark space
        static const juce::Colour backgroundLight;    // Lighter space
        
        // Electric accents
        static const juce::Colour accentBlue;         // Electric blue
        static const juce::Colour accentCyan;         // Cosmic cyan
        static const juce::Colour accentPurple;       // Nebula purple
        static const juce::Colour accentGreen;        // Alien green
        
        // Utility colors
        static const juce::Colour textPrimary;        // Bright white
        static const juce::Colour textSecondary;      // Soft blue-grey
        static const juce::Colour controlOutline;     // Subtle control border
        static const juce::Colour controlFill;        // Control background
        
        // Tab colors
        static const juce::Colour tabInactive;        // Inactive tab
        static const juce::Colour tabActive;          // Active tab
        
        // Star colors for cosmic background
        static const juce::Colour starDim;            // Dim purple star
        static const juce::Colour starBright;         // Bright purple star
        static const juce::Colour starHighlight;      // Highlight purple star
    };
    
    //==============================================================================
    VoidLookAndFeel();
    virtual ~VoidLookAndFeel();

    //==============================================================================
    // Button styling
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted, 
                              bool shouldDrawButtonAsDown) override;
    
    //==============================================================================
    // Background styling methods
    void drawCosmicBackground(juce::Graphics& g, juce::Rectangle<int> bounds, float starDensity = 0.0003f);
    
    //==============================================================================
    // Slider styling for all types
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider& slider) override;
                           
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override;
    
    //==============================================================================
    // ComboBox styling
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override;
                       
    //==============================================================================
    // Toggle button styling (checkboxes)
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, 
                           bool shouldDrawButtonAsDown) override;
    
    //==============================================================================
    // Utility drawing methods
    void drawGlowEffect(juce::Graphics& g, juce::Rectangle<float> bounds, 
                         juce::Colour color, float intensity);
                       
    void drawCosmicGradientBackground(juce::Graphics& g, juce::Rectangle<float> bounds, 
                                      float intensity = 1.0f);
                                      
    juce::Path createStarPath(float outerRadius, float innerRadius, int numPoints);
};
