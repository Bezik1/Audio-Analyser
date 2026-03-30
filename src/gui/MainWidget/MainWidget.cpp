#include "MainWidget.hpp"

#include "../../core/AudioAnalyser/AudioAnalyser.hpp"

#include "../widgets/SpectrumChartWidget/SpectrumChartWidget.hpp"
#include "../widgets/WaveformChartWidget/WaveformChartWidget.hpp"

MainWidget::MainWidget(const std::string &wavPath, QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    statusLabel = new QLabel(MainWidget::MSG_INIT, this);
    statusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(statusLabel);

    connect(this, &MainWidget::statusChanged, statusLabel, &QLabel::setText);

    startAsyncAnalysis(wavPath);
}

void MainWidget::startAsyncAnalysis(const std::string &wavPath)
{
    connect(&watcher, &QFutureWatcher<void>::finished, this, &MainWidget::onAnalysisFinished);

    QFuture<void> future = QtConcurrent::run([this, wavPath]()
    {
        emit statusChanged(MainWidget::MSG_READING_WAV);
        this->audioData = AudioUtils::readWav(wavPath);
        int numSamples = audioData.getNumSamples();

        emit statusChanged(MainWidget::MSG_CALC_DFT);
        this->spectrum = AudioAnalyser::discreteFourierTransform(audioData.data.samples, audioData.fmt.sampleRate);

        emit statusChanged(MainWidget::MSG_SORTING_SPECTRUM);
        auto sortedSpectrum = this->spectrum;
        std::sort(sortedSpectrum.begin(), sortedSpectrum.end(), [](const auto &a, const auto &b) 
        {
            return a.amplitude > b.amplitude;
        });

        for (int N : MainWidget::THRESHOLDS) {
            emit statusChanged(QString(MainWidget::MSG_RECONSTRUCTING_TOP).arg(N));

            int count = std::min(N, static_cast<int>(sortedSpectrum.size()));
            std::vector<AudioAnalyser::FrequencyData> topN(sortedSpectrum.begin(), sortedSpectrum.begin() + count);
            
            auto recN = AudioAnalyser::reconstruct(topN, numSamples, audioData.fmt.sampleRate);

            AudioUtils::AudioData nData = AudioUtils::prepareSamplesToBeSaved(
                recN, audioData.fmt.numChannels, audioData.fmt.sampleRate, audioData.fmt.bitsPerSample);

            AudioUtils::saveWav(nData, MainWidget::SPECTRUM_FILE_DIR + std::to_string(N) + ".wav");
        }

        emit statusChanged(MainWidget::MSG_FINALIZING);
        this->reconstructedSamples = AudioAnalyser::reconstruct(this->spectrum, numSamples, audioData.fmt.sampleRate);
        
        AudioUtils::AudioData fullData = AudioUtils::prepareSamplesToBeSaved(
            this->reconstructedSamples, audioData.fmt.numChannels, audioData.fmt.sampleRate, audioData.fmt.bitsPerSample);
        AudioUtils::saveWav(fullData, MainWidget::OUTPUT_FILE_DIR);

        emit statusChanged(MainWidget::MSG_DONE);
    });

    watcher.setFuture(future);
}

void MainWidget::onAnalysisFinished()
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    QHBoxLayout *topChartsLayout = new QHBoxLayout();
    auto *originalChart = new WaveformChartWidget(audioData.data.samples, audioData.fmt.sampleRate, "Input Audio File Spectrum");
    auto *reconstructedChart = new WaveformChartWidget(reconstructedSamples, audioData.fmt.sampleRate, "Output Audio File Spectrum");

    topChartsLayout->addWidget(originalChart, 1);
    topChartsLayout->addWidget(reconstructedChart, 1);
    layout->addLayout(topChartsLayout, 1);
    layout->addWidget(new SpectrumChartWidget(spectrum), 1);
}