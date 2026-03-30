#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include <string>

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include "../../core/AudioAnalyser/AudioAnalyser.hpp"
#include "../../utils/AudioUtils/AudioUtils.hpp"

class MainWidget : public QWidget
{
Q_OBJECT

    public : explicit MainWidget(const std::string &wavPath, QWidget *parent = nullptr);

signals:
    void statusChanged(const QString &message);

private slots:
    void onAnalysisFinished();

private:
    inline static const QString MSG_INIT = "Initializing...";
    inline static const QString MSG_READING_WAV = "Reading WAV file...";
    inline static const QString MSG_CALC_DFT = "Calculating Fourier Transform (DFT)...";
    inline static const QString MSG_SORTING_SPECTRUM = "Sorting spectrum data...";
    inline static const QString MSG_RECONSTRUCTING_TOP = "Reconstructing top frequencies...";
    inline static const QString MSG_FINALIZING = "Finalizing full reconstruction...";
    inline static const QString MSG_DONE = "Done! Rendering charts...";

    inline static const std::string SPECTRUM_FILE_DIR = "eval/spectrums/spectrum_top_";
    inline static const std::string OUTPUT_FILE_DIR = "eval/output_file.wav";

    inline static const std::vector<int> THRESHOLDS = {10, 100, 1000, 2000, 5000, 10000};

    void
    startAsyncAnalysis(const std::string &wavPath);

    QVBoxLayout *layout = nullptr;
    QLabel *statusLabel = nullptr;
    QFutureWatcher<void> watcher;

    AudioUtils::AudioData audioData;
    std::vector<AudioAnalyser::FrequencyData> spectrum;
    std::vector<float> reconstructedSamples;
};