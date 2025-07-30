#pragma once
class WavetableEngine {
public:
    WavetableEngine();
    void setWaveform(int oscIndex, int waveType);
    void process(float* outBuffer, int numSamples);
};
