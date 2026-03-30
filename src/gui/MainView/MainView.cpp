#include "MainView.hpp"

#include "../../core/AudioAnalyser/AudioAnalyser.hpp"

MainView::MainView(const std::string &wavPath, QWidget *parent)
    : QWidget(parent)
{
    AudioUtils::AudioData audioData = AudioUtils::readWav(wavPath);
    layout = new QVBoxLayout(this);
    QHBoxLayout *topChartsLayout = new QHBoxLayout();

    int displayWidth = this->screen()->geometry().width();
    int numSamples = audioData.getNumSamples();
    int step = std::max(1, numSamples / displayWidth);
    double durationInSeconds = static_cast<double>(numSamples) / audioData.fmt.sampleRate;

    auto *originalChart = new QChart();
    originalChart->setBackgroundBrush(QBrush(QColor(53, 53, 53)));
    auto *originalSeries = new QLineSeries();

    for (int i = 0; i < numSamples; i += step)
    {
        originalSeries->append(static_cast<double>(i) / audioData.fmt.sampleRate, audioData.data.samples[i]);
    }

    originalChart->addSeries(originalSeries);
    originalChart->createDefaultAxes();
    originalChart->setTitle("Original Audio Waveform");
    originalChart->setTitleBrush(QBrush(Qt::white));
    originalChart->legend()->hide();

    if (!originalChart->axes().isEmpty())
    {
        auto *axisX = qobject_cast<QValueAxis *>(originalChart->axes(Qt::Horizontal).at(0));
        auto *axisY = qobject_cast<QValueAxis *>(originalChart->axes(Qt::Vertical).at(0));
        if (axisX)
        {
            axisX->setRange(0, durationInSeconds);
            axisX->setLabelsColor(Qt::white);
            axisX->setTitleText("Time (s)");
            axisX->setTitleBrush(QBrush(Qt::white));
        }
        if (axisY)
        {
            axisY->setRange(-1.0, 1.0);
            axisY->setLabelsColor(Qt::white);
            axisY->setTitleText("Amplitude");
            axisY->setTitleBrush(QBrush(Qt::white));
        }
    }

    auto *originalChartView = new QChartView(originalChart);
    originalChartView->setRenderHint(QPainter::Antialiasing);
    topChartsLayout->addWidget(originalChartView, 1);

    auto spectrum = AudioAnalyser::discreteFourierTransform(audioData.data.samples, audioData.fmt.sampleRate);

    auto sortedSpectrum = spectrum;
    std::sort(sortedSpectrum.begin(), sortedSpectrum.end(), [](const auto &a, const auto &b)
              { return a.amplitude > b.amplitude; });

    std::vector<int> thresholds = {10, 100, 1000, 2000, 5000, 10000, 50000};
    for (int N : thresholds)
    {
        int count = std::min(N, static_cast<int>(sortedSpectrum.size()));
        std::vector<AudioAnalyser::FrequencyData> topN(sortedSpectrum.begin(), sortedSpectrum.begin() + count);
        auto recN = AudioAnalyser::reconstruct(topN, numSamples, audioData.fmt.sampleRate);

        AudioUtils::AudioData nData = AudioUtils::prepareSamplesToBeSaved(
            recN, audioData.fmt.numChannels, audioData.fmt.sampleRate, audioData.fmt.bitsPerSample);
        AudioUtils::saveWav(nData, "eval/spectrums/spectrum_top_" + std::to_string(N) + ".wav");
    }

    auto reconstructedSamples = AudioAnalyser::reconstruct(spectrum, numSamples, audioData.fmt.sampleRate);
    AudioUtils::AudioData fullData = AudioUtils::prepareSamplesToBeSaved(
        reconstructedSamples, audioData.fmt.numChannels, audioData.fmt.sampleRate, audioData.fmt.bitsPerSample);
    AudioUtils::saveWav(fullData, "eval/output_file.wav");

    auto *reconstructedChart = new QChart();
    reconstructedChart->setBackgroundBrush(QBrush(QColor(53, 53, 53)));
    auto *reconstructedSeries = new QLineSeries();

    for (int i = 0; i < numSamples; i += step)
    {
        reconstructedSeries->append(static_cast<double>(i) / audioData.fmt.sampleRate, reconstructedSamples[i]);
    }

    reconstructedChart->addSeries(reconstructedSeries);
    reconstructedChart->createDefaultAxes();
    reconstructedChart->setTitle("Reconstructed (Full)");
    reconstructedChart->setTitleBrush(QBrush(Qt::white));
    reconstructedChart->legend()->hide();

    if (!reconstructedChart->axes().isEmpty())
    {
        auto *axisX = qobject_cast<QValueAxis *>(reconstructedChart->axes(Qt::Horizontal).at(0));
        auto *axisY = qobject_cast<QValueAxis *>(reconstructedChart->axes(Qt::Vertical).at(0));
        if (axisX)
        {
            axisX->setRange(0, durationInSeconds);
            axisX->setLabelsColor(Qt::white);
        }
        if (axisY)
        {
            axisY->setRange(-1.0, 1.0);
            axisY->setLabelsColor(Qt::white);
        }
    }

    auto *reconstructedChartView = new QChartView(reconstructedChart);
    reconstructedChartView->setRenderHint(QPainter::Antialiasing);
    topChartsLayout->addWidget(reconstructedChartView, 1);

    auto *spectrumChart = new QChart();
    spectrumChart->setBackgroundBrush(QBrush(QColor(53, 53, 53)));
    auto *spectrumSeries = new QLineSeries();

    int spectrumSizeLimit = std::min(5512, static_cast<int>(spectrum.size()));
    for (int i = 0; i < spectrumSizeLimit; i++)
    {
        spectrumSeries->append(spectrum[i].frequency, spectrum[i].amplitude);
    }

    spectrumChart->addSeries(spectrumSeries);
    spectrumChart->createDefaultAxes();
    spectrumChart->setTitle("Frequency Spectrum");
    spectrumChart->setTitleBrush(QBrush(Qt::white));
    spectrumChart->legend()->hide();

    if (!spectrumChart->axes().isEmpty())
    {
        auto *axisX = qobject_cast<QValueAxis *>(spectrumChart->axes(Qt::Horizontal).at(0));
        auto *axisY = qobject_cast<QValueAxis *>(spectrumChart->axes(Qt::Vertical).at(0));
        if (axisX)
        {
            axisX->setLabelsColor(Qt::white);
            axisX->setTitleText("Hz");
        }
        if (axisY)
        {
            axisY->setLabelsColor(Qt::white);
        }
    }

    auto *spectrumChartView = new QChartView(spectrumChart);
    spectrumChartView->setRenderHint(QPainter::Antialiasing);

    layout->addLayout(topChartsLayout, 1);
    layout->addWidget(spectrumChartView, 1);
}