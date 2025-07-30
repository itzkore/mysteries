#pragma once
class ReverbFX {
public:
    ReverbFX();
    void process(float* buffer, int numSamples);
};
