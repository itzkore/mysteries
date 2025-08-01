#include "OrbVisualizer.h"

OrbVisualizer::OrbVisualizer() 
    : fft(fftOrder)
    , window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    // Initialize buffers
    audioBuffer.fill(0.0f);
    energyHistory.fill(0.0f);
    fftData.fill(0.0f);
    spectrumData.fill(0.0f);
    
    // Initialize particles
    for (auto& particle : particles)
    {
        particle.life = 0.0f; // Inactive
    }
    
    // Make component transparent (no background fill)
    setOpaque(false);
    
    // Important: Don't start timer in constructor (validation safety)
}

OrbVisualizer::~OrbVisualizer()
{
    // Always stop timer in destructor (validation safety)
    stopTimer();
}

void OrbVisualizer::paint(juce::Graphics& g)
{
    try
    {
        auto bounds = getLocalBounds().toFloat();
        // Define inner frame for orb placement with margin
        auto frame = bounds.reduced(10.0f);
        
        // Safety check for validation
        if (bounds.isEmpty() || bounds.getWidth() < 10 || bounds.getHeight() < 10)
        {
            g.fillAll(backgroundColor);
            return;
        }
        
        // [Background removed for transparent orb]
        // g.fillAll(backgroundColor);
        
        // Make the orb fit in the smaller dimension to stay circular
        // Calculate orb size within frame for a smaller visual
        const float size = std::min(frame.getWidth(), frame.getHeight()) * 0.70f;
        juce::Rectangle<float> orbBounds(0.0f, 0.0f, size, size);
        orbBounds.setCentre(frame.getCentre());
        
        // Draw particles first (behind orb)
        drawParticles(g);
        
        // Draw the orb itself based on current mode
        switch (currentMode)
        {
            case DisplayMode::Energy:
                drawEnergyOrb(g, orbBounds);
                break;
                
            case DisplayMode::Spectrum:
                drawSpectrumOrb(g, orbBounds);
                break;
                
            case DisplayMode::Phase:
                drawPhaseOrb(g, orbBounds);
                break;
        }
    }
    catch (...)
    {
        // Safety fallback if anything goes wrong
        g.fillAll(juce::Colours::black);
    }
}

void OrbVisualizer::resized()
{
    // Nothing specific needed here, as we calculate bounds in paint()
}

void OrbVisualizer::setVisible(bool shouldBeVisible)
{
    Component::setVisible(shouldBeVisible);
    
    // Ultra-conservative timer management for validation safety
    if (shouldBeVisible && getParentComponent() != nullptr)
    {
        auto bounds = getLocalBounds();
        if (!bounds.isEmpty() && bounds.getWidth() > 20 && bounds.getHeight() > 20)
        {
            if (!isTimerRunning())
                startTimerHz(30); // Animation at 30 FPS
        }
    }
    else
    {
        if (isTimerRunning())
            stopTimer();
    }
}

void OrbVisualizer::timerCallback()
{
    try 
    {
        // Safety checks first
        if (!isVisible() || getParentComponent() == nullptr)
        {
            stopTimer();
            return;
        }
        
        auto bounds = getLocalBounds();
        if (bounds.isEmpty() || bounds.getWidth() <= 20 || bounds.getHeight() <= 20)
        {
            stopTimer();
            return;
        }
        
        // Update animation state
        rotationPhase += rotationSpeed * 0.01f;
        if (rotationPhase > 1.0f) 
            rotationPhase -= 1.0f;
        
        // Decay the current energy value for smooth animation
        currentEnergy *= 0.95f;
        
        // Slowly decay the peak energy
        peakEnergy *= 0.99f;
        
        // Update MIDI velocity with decay
        if (midiActive)
        {
            // When MIDI is active, decay is slower
            midiVelocity *= 0.995f;
        }
        else
        {
            // When MIDI is inactive, decay faster
            midiVelocity *= midiDecay;
        }
        
        // If MIDI is active, ensure minimum energy level for better visualization
        if (midiActive && currentEnergy < midiVelocity * 0.3f)
            currentEnergy = midiVelocity * 0.3f;
        
        // Clamp activeParticles range to prevent invalid indices
        activeParticles = juce::jlimit(0, maxParticles, activeParticles);
        // Update particles
        updateParticles();
        
        // Trigger a repaint for animation
        repaint();
    }
    catch (...)
    {
        // If anything goes wrong, stop the timer
        stopTimer();
    }
}

void OrbVisualizer::pushAudioData(const float* audioData, int numSamples)
{
    if (numSamples <= 0 || !audioData) 
        return;
    
    // Calculate energy from this audio chunk
    float energy = 0.0f;
    for (int i = 0; i < numSamples; ++i)
    {
        energy += audioData[i] * audioData[i];
    }
    energy = std::sqrt(energy / static_cast<float>(numSamples));
    
    // Update the current energy value (smoothly)
    currentEnergy = currentEnergy * 0.7f + energy * 0.3f;
    
    // Update peak if needed
    if (currentEnergy > peakEnergy)
        peakEnergy = currentEnergy;
    
    // Copy data to the buffer with bounds checking
    const int samplesToUse = std::min(numSamples, bufferSize);
    for (int i = 0; i < samplesToUse; ++i)
    {
        const int writeIndex = (writePos + i) % bufferSize;
        if (writeIndex >= 0 && writeIndex < bufferSize)
        {
            audioBuffer[writeIndex] = audioData[i];
            
            // Also store energy history with bounds check
            if (writeIndex < static_cast<int>(energyHistory.size()))
                energyHistory[writeIndex] = energy;
        }
    }
    writePos = (writePos + samplesToUse) % bufferSize;
    
    // Update FFT data if in spectrum mode
    if (currentMode == DisplayMode::Spectrum)
        calculateSpectrum();
    
    // Emit particles based on energy
    if (energy > 0.05f)
    {
        const int particlesToEmit = static_cast<int>(energy * 10.0f);
        emitParticles(particlesToEmit);
    }
}

void OrbVisualizer::pushAudioData(const juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() == 0 || buffer.getNumSamples() == 0)
        return;
    
    // Mix down to mono if needed
    if (buffer.getNumChannels() == 1)
    {
        pushAudioData(buffer.getReadPointer(0), buffer.getNumSamples());
    }
    else
    {
        // Mix all channels to mono
        std::vector<const float*> channels;
        channels.reserve(buffer.getNumChannels());
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            channels.push_back(buffer.getReadPointer(ch));
        
        const int numSamples = buffer.getNumSamples();
        std::vector<float> mixedData(numSamples, 0.0f);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float sum = 0.0f;
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
                sum += channels[ch][i];
            
            mixedData[i] = sum / static_cast<float>(buffer.getNumChannels());
        }
        
        pushAudioData(mixedData.data(), numSamples);
    }
}

void OrbVisualizer::setDisplayMode(DisplayMode mode)
{
    currentMode = mode;
    
    // Reset data when changing modes
    audioBuffer.fill(0.0f);
    energyHistory.fill(0.0f);
    fftData.fill(0.0f);
    spectrumData.fill(0.0f);
    
    repaint();
}

void OrbVisualizer::setMidiActivity(bool isActive, float velocity)
{
    midiActive = isActive;
    
    // Update velocity with some smoothing if active
    if (isActive)
        midiVelocity = juce::jmax(midiVelocity, juce::jlimit(0.0f, 1.0f, velocity));
    
    // Will be reflected in next animation frame
}

void OrbVisualizer::setOrbColor(juce::Colour baseColor)
{
    orbColor = baseColor;
    repaint();
}

void OrbVisualizer::setGlowColor(juce::Colour color)
{
    glowColor = color;
    repaint();
}

void OrbVisualizer::setBackgroundColor(juce::Colour bgColor)
{
    backgroundColor = bgColor;
    repaint();
}

void OrbVisualizer::calculateSpectrum()
{
    // Safety check for buffer sizes
    if (writePos < 0 || writePos >= bufferSize)
        writePos = 0;
    
    // Copy audio data to FFT buffer with bounds checking
    int readPos = (writePos - fftSize + bufferSize) % bufferSize;
    for (int i = 0; i < fftSize; ++i)
    {
        int bufferIndex = (readPos + i) % bufferSize;
        if (bufferIndex >= 0 && bufferIndex < bufferSize)
        {
            fftData[i * 2] = audioBuffer[bufferIndex];
            fftData[i * 2 + 1] = 0.0f; // Imaginary part
        }
        else
        {
            fftData[i * 2] = 0.0f;
            fftData[i * 2 + 1] = 0.0f;
        }
    }
    
    // Apply windowing function with safety check
    if (fftData.size() >= fftSize * 2)
        window.multiplyWithWindowingTable(fftData.data(), fftSize);
    
    // Perform FFT
    fft.performFrequencyOnlyForwardTransform(fftData.data());
    
    // Convert to spectrum data with some smoothing and bounds checking
    for (int i = 0; i < numBins && i < static_cast<int>(spectrumData.size()) && i < static_cast<int>(fftData.size()); ++i)
    {
        const float newValue = fftData[i];
        spectrumData[i] = spectrumData[i] * 0.7f + newValue * 0.3f;
    }
}

void OrbVisualizer::updateParticles()
{
    const auto bounds = getLocalBounds().toFloat();
    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();
    
    // Safety clamp for activeParticles
    activeParticles = juce::jlimit(0, maxParticles, activeParticles);
    
    // Update existing particles
    for (int i = 0; i < activeParticles && i < maxParticles; ++i)
    {
        auto& p = particles[i];
        
        // Update position
        p.x += p.vx;
        p.y += p.vy;
        
        // Apply upward drift for smoke effect instead of gravity
        p.vy -= 0.015f; // Upward drift
        
        // Horizontal dispersion and dampening
        p.vx *= 0.985f;
        p.vy *= 0.99f;
        
        // Decay life slower for more visible smoke
        p.life -= 0.008f;
        
        // Update color alpha based on life with softer fade
        p.color = p.color.withAlpha(p.life * 0.6f);
        
        // Grow slightly as life depletes (smoke expansion)
        p.size = p.size * 1.002f;
    }
    
    // Remove dead particles with safety checks
    for (int i = 0; i < activeParticles && i < maxParticles; ++i)
    {
        if (particles[i].life <= 0.0f)
        {
            // Replace with the last active particle
            if (i < activeParticles - 1 && activeParticles > 1 && (activeParticles - 1) < maxParticles)
                particles[i] = particles[activeParticles - 1];
            
            // Decrease active count
            --activeParticles;
            --i; // Check the same index again
            
            // Safety clamp
            activeParticles = juce::jlimit(0, maxParticles, activeParticles);
        }
    }
}

void OrbVisualizer::emitParticles(int count)
{
    if (activeParticles >= maxParticles || count <= 0)
        return;
    
    const auto bounds = getLocalBounds().toFloat();
    if (bounds.isEmpty())
        return;
        
    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();
    const float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.35f;
    
    const int numToEmit = juce::jlimit(0, maxParticles - activeParticles, count);
    
    for (int i = 0; i < numToEmit; ++i)
    {
        // Find an inactive particle with bounds checking
        if (activeParticles < maxParticles)
        {
            auto& p = particles[activeParticles++];
            
            // Random angle and distance from center
            const float angle = juce::Random::getSystemRandom().nextFloat() * juce::MathConstants<float>::twoPi;
            const float distance = radius * (0.8f + 0.2f * juce::Random::getSystemRandom().nextFloat());
            
            // Set position - start from the orb's edge
            p.x = centerX + std::cos(angle) * distance;
            p.y = centerY + std::sin(angle) * distance;
            
            // Random velocity away from center - upward bias for smoke effect
            const float speed = 0.3f + juce::Random::getSystemRandom().nextFloat() * 0.8f;
            p.vx = std::cos(angle) * speed * 0.3f; // Reduced horizontal drift
            p.vy = -speed * (0.8f + juce::Random::getSystemRandom().nextFloat() * 0.4f); // Upward motion
            
            // Random life and size for smoke particles
            p.life = 0.8f + juce::Random::getSystemRandom().nextFloat() * 0.4f;
            p.size = 1.5f + juce::Random::getSystemRandom().nextFloat() * 2.5f;
            
            // Color based on the orb color with variation
            const float hue = orbColor.getHue();
            const float sat = orbColor.getSaturation();
            const float bright = orbColor.getBrightness();
            
            const float hueVariation = 0.05f;
            const float newHue = juce::jlimit(0.0f, 1.0f, hue + (juce::Random::getSystemRandom().nextFloat() - 0.5f) * hueVariation);
            
            p.color = juce::Colour::fromHSV(newHue, sat, bright, 0.8f);
        }
    }
}

void OrbVisualizer::drawParticles(juce::Graphics& g)
{
    // Ensure we do not exceed particle array bounds
    const int particleCount = juce::jlimit(0, maxParticles, activeParticles);
    for (int i = 0; i < particleCount; ++i)
    {
        const auto& p = particles[i];
        
        const float size = p.size;
        const float halfSize = size * 0.5f;
        
        // Draw particle with glow
        g.setColour(p.color.withAlpha(p.life * 0.3f));
        g.fillEllipse(p.x - halfSize * 2, p.y - halfSize * 2, size * 4, size * 4);
        
        g.setColour(p.color);
        g.fillEllipse(p.x - halfSize, p.y - halfSize, size, size);
    }
}

void OrbVisualizer::drawEnergyOrb(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();
    const float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    
    // Calculate pulsation based on energy and MIDI activity
    float pulseScale = 1.0f + pulsationAmount * currentEnergy;
    
    // When MIDI is active, make the orb larger based on velocity
    if (midiVelocity > 0.05f) 
    {
        pulseScale += midiVelocity * 0.3f; // Increased MIDI response
    }
    
    const float baseRadius = radius * 0.85f * pulseScale;
    
    // Calculate glow intensity based on MIDI and audio activity
    float glowIntensity = 0.5f * currentEnergy; // Increased base glow
    if (midiVelocity > 0.05f)
    {
        glowIntensity = juce::jmax(glowIntensity, midiVelocity * 0.7f); // Increased MIDI glow
    }
    
    // Draw multiple outer glow layers for better visual depth
    for (int i = 0; i < 3; i++) {
        const float glowRadius = baseRadius * (1.2f + i * 0.15f);
        float alphaFactor = 1.0f - (i * 0.25f);
        
        juce::ColourGradient glowGradient(
            glowColor.withAlpha(glowIntensity * alphaFactor),
            centerX, centerY,
            glowColor.withAlpha(0.0f),
            centerX + glowRadius, centerY + glowRadius, 
            true);
        
        g.setGradientFill(glowGradient);
        g.fillEllipse(centerX - glowRadius, centerY - glowRadius, glowRadius * 2, glowRadius * 2);
    }
    
    // Adjust orb color based on MIDI activity
    juce::Colour baseCoreColor = orbColor;
    if (midiVelocity > 0.05f)
    {
        // Shift hue slightly and make brighter when MIDI is active
        float hue = orbColor.getHue();
        float saturation = orbColor.getSaturation();
        float brightness = orbColor.getBrightness();
        
        // Increase brightness based on velocity
        brightness = juce::jmin(1.0f, brightness + midiVelocity * 0.3f);
        
        // Shift hue slightly based on velocity
        hue = fmodf(hue + midiVelocity * 0.05f, 1.0f);
        
        baseCoreColor = juce::Colour::fromHSV(hue, saturation, brightness, 1.0f);
    }
    
    // Draw base orb with improved gradient
    juce::ColourGradient gradient(
        baseCoreColor.brighter(0.4f), // Brighter center
        centerX - baseRadius * 0.3f, centerY - baseRadius * 0.3f,
        baseCoreColor.darker(0.3f), // Less dark edges
        centerX + baseRadius * 0.6f, centerY + baseRadius * 0.6f,
        true);
    
    g.setGradientFill(gradient);
    g.fillEllipse(centerX - baseRadius, centerY - baseRadius, baseRadius * 2, baseRadius * 2);
    
    // Draw inner highlight with better positioning
    const float highlightRadius = baseRadius * 0.6f;
    
    // Create more realistic highlight with gradient
    juce::ColourGradient highlightGradient(
        orbColor.brighter(1.0f).withAlpha(0.5f),
        centerX - highlightRadius * 0.5f, 
        centerY - highlightRadius * 0.5f,
        orbColor.brighter(0.2f).withAlpha(0.0f),
        centerX, centerY,
        true);
        
    g.setGradientFill(highlightGradient);
    g.fillEllipse(
        centerX - highlightRadius * 0.8f, 
        centerY - highlightRadius * 0.8f,
        highlightRadius * 1.6f, 
        highlightRadius * 1.6f);
    
    // Energy ring with improved styling
    const float ringWidth = radius * 0.06f; // Slightly thicker
    const float ringRadius = baseRadius * 0.95f; // Larger ring
    
    // Outer ring glow
    g.setColour(glowColor.withAlpha(0.3f));
    juce::Path outerRingPath;
    outerRingPath.addEllipse(centerX - ringRadius - 2, centerY - ringRadius - 2, 
                            (ringRadius + 2) * 2, (ringRadius + 2) * 2);
    g.strokePath(outerRingPath, juce::PathStrokeType(ringWidth + 4, 
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Main ring
    g.setColour(glowColor.withAlpha(0.8f));
    juce::Path ringPath;
    ringPath.addEllipse(centerX - ringRadius, centerY - ringRadius, ringRadius * 2, ringRadius * 2);
    g.strokePath(ringPath, juce::PathStrokeType(ringWidth, 
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Energy level indicator with animated color
    const float maxAngle = juce::MathConstants<float>::twoPi * (0.7f + 0.3f * currentEnergy);
    const float startAngle = -juce::MathConstants<float>::halfPi + rotationPhase * juce::MathConstants<float>::twoPi;
    
    juce::Path energyPath;
    energyPath.addArc(centerX - ringRadius, centerY - ringRadius, 
                     ringRadius * 2, ringRadius * 2,
                     startAngle, startAngle + maxAngle, true);
    
    // Create a gradient color for the energy path
    juce::Colour startColor = juce::Colours::cyan.withAlpha(0.9f);
    juce::Colour endColor = juce::Colours::magenta.withAlpha(0.9f);
    float mixRatio = 0.5f + 0.5f * std::sin(rotationPhase * juce::MathConstants<float>::twoPi * 2.0f);
    juce::Colour energyColor = startColor.interpolatedWith(endColor, mixRatio);
    
    g.setColour(energyColor);
    g.strokePath(energyPath, juce::PathStrokeType(ringWidth * 0.8f, 
                juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void OrbVisualizer::drawSpectrumOrb(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();
    const float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    
    // Base orb similar to energy visualization
    drawEnergyOrb(g, bounds);
    
    // Draw spectrum analysis as lines from center
    const int numLines = 72; // Number of lines to draw
    const float baseLineLength = radius * 0.8f;
    const float rotationOffset = rotationPhase * juce::MathConstants<float>::twoPi;
    
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    
    for (int i = 0; i < numLines; ++i)
    {
        const float angle = rotationOffset + (i / static_cast<float>(numLines)) * juce::MathConstants<float>::twoPi;
        
        // Map this angle to a spectrum bin
        const int binIndex = static_cast<int>((i / static_cast<float>(numLines)) * numBins * 0.5f); // Use only lower half of spectrum
        const float amplitude = juce::jlimit(0.0f, 1.0f, spectrumData[binIndex] * 3.0f); // Scale up for visibility
        
        const float lineLength = baseLineLength * (0.2f + 0.8f * amplitude);
        
        const float x1 = centerX + std::cos(angle) * radius * 0.3f; // Start from inner radius
        const float y1 = centerY + std::sin(angle) * radius * 0.3f;
        const float x2 = centerX + std::cos(angle) * lineLength;
        const float y2 = centerY + std::sin(angle) * lineLength;
        
        const float thickness = 1.0f + amplitude * 2.0f;
        g.drawLine(x1, y1, x2, y2, thickness);
    }
}

void OrbVisualizer::drawPhaseOrb(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    const float centerX = bounds.getCentreX();
    const float centerY = bounds.getCentreY();
    const float radius = std::min(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    
    // Draw base orb (simpler than energy orb)
    const float baseRadius = radius * 0.85f;
    
    // Outer glow
    juce::ColourGradient glowGradient(
        glowColor.withAlpha(0.3f),
        centerX, centerY,
        glowColor.withAlpha(0.0f),
        centerX + radius * 1.2f, centerY + radius * 1.2f,
        true);
    
    g.setGradientFill(glowGradient);
    g.fillEllipse(centerX - radius * 1.2f, centerY - radius * 1.2f, radius * 2.4f, radius * 2.4f);
    
    // Base orb
    juce::ColourGradient gradient(
        orbColor.brighter(0.2f),
        centerX - baseRadius * 0.5f, centerY - baseRadius * 0.5f,
        orbColor.darker(0.3f),
        centerX + baseRadius * 0.5f, centerY + baseRadius * 0.5f,
        true);
    
    g.setGradientFill(gradient);
    g.fillEllipse(centerX - baseRadius, centerY - baseRadius, baseRadius * 2, baseRadius * 2);
    
    // Draw phase pattern (Lissajous-like figure)
    // This simulates a phase correlation display
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    
    juce::Path phasePath;
    const float phaseScale = baseRadius * 0.7f;
    const int numPoints = 100;
    
    bool pathStarted = false;
    
    for (int i = 0; i < numPoints; ++i)
    {
        const float t = i / static_cast<float>(numPoints - 1);
        const float angle = t * juce::MathConstants<float>::twoPi * 2.0f + rotationPhase * 10.0f;
        
        // Create a complex phase pattern
        const float phase1 = std::sin(angle);
        const float phase2 = std::sin(angle * 1.5f + currentEnergy * 2.0f);
        
        const float x = centerX + phase1 * phaseScale;
        const float y = centerY + phase2 * phaseScale;
        
        if (!pathStarted)
        {
            phasePath.startNewSubPath(x, y);
            pathStarted = true;
        }
        else
        {
            phasePath.lineTo(x, y);
        }
    }
    
    g.strokePath(phasePath, juce::PathStrokeType(2.0f));
    
    // Inner ring
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.drawEllipse(centerX - phaseScale, centerY - phaseScale, phaseScale * 2, phaseScale * 2, 1.0f);
}
