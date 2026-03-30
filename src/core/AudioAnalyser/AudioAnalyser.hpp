#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

class AudioAnalyser
{
public:
    /**
     * @brief Represents the data of a single frequency component in a signal.
     *
     * @param amplitude Represents its strength in the signal.
     * @param phase The phase shift of the frequency component in radians, indicating its offset along the time axis.
     * @param frequency The frequency in Hz of this component.
     */
    struct FrequencyData
    {
        float amplitude;
        float phase;
        float frequency;
    };

    AudioAnalyser();

    /**
     * @brief Decomposes a time-domain audio signal into its frequency components using the Discrete Fourier Transform (DFT).
     *
     * @details This function takes a vector of audio samples and computes the frequency spectrum by performing a Discrete Fourier Transform.
     * It calculates the amplitude, phase, and frequency for each component, effectively breaking down the complex waveform into simpler sinusoidal components.
     * The number of frequency components returned is based on the Nyquist-Shannon sampling theorem, which states that the maximum number of unique frequencies
     * for a sampled signal is half the number of samples plus one. The DFT here operates in O(N^2) time complexity, where N is the number of samples.
     * This method is useful for analyzing the dominant frequencies present in the audio data.
     *
     * @param samples A vector of float values representing the audio samples in the time domain.
     * @param sampleRate The sampling rate of the audio signal in Hz.
     * @return std::vector<FrequencyData> A vector containing the frequency components, each with amplitude, phase, and frequency information.
     */
    static std::vector<FrequencyData>
    discreteFourierTransform(const std::vector<float> &samples, int sampleRate);

    /**
     * @brief Reconstructs a time-domain audio signal from its frequency components.
     *
     * @details Given a vector of FrequencyData representing the amplitude, phase, and frequency of each component,
     * this function synthesizes the original waveform by summing the sinusoidal waves corresponding to each frequency component.
     * The reconstruction uses the specified number of samples and the sample rate to produce a time-domain signal approximation.
     * This process effectively inverts the Discrete Fourier Transform, allowing the original or an approximated signal to be recreated from its frequency spectrum.
     *
     * @param spectrum A vector of FrequencyData representing the frequency components of the signal.
     * @param numSamples The number of samples to generate in the reconstructed signal.
     * @param sampleRate The sampling rate of the audio signal in Hz.
     * @return std::vector<float> A vector of float values representing the reconstructed audio samples in the time domain.
     */
    static std::vector<float>
    reconstruct(const std::vector<FrequencyData> &spectrum, int numSamples, int sampleRate);

private:
};