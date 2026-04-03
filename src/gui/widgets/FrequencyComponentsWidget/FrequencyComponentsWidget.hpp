#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDir>
#include <vector>
#include <algorithm>

#include "../IndividualFrequency/IndividualFrequency.hpp"
#include "../../../core/AudioAnalyser/AudioAnalyser.hpp"

class FrequencyComponentsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrequencyComponentsWidget(QWidget *parent = nullptr);
    void showAnalyzingStatus();
    void setFrequencyData(
        const std::vector<AudioAnalyser::FrequencyData> &topFrequencies,
        int numSamples,
        int sampleRate);

signals:
    void topXChanged(int newX);
    void saveRequested(const std::vector<AudioAnalyser::FrequencyData> &data);

private:
    inline static QString STATUS_LABEL_TEXT = "Please select an audio file first.";
    inline static QString COUNT_LABEL_TEXT = "Number of components:";
    inline static QString VISIBLE_COUNT_LABEL_TEXT = "Number of visible components:";
    inline static QString SAVE_BTN_TEXT = "Save Modified Audio";
    inline static QString PLAY_BTN_TEXT = "Play Modified Audio";
    inline static QString SUMMARY_CHART_TEXT = "Frequency Composition (Modified)";
    inline static QString ANALYZING_TEXT = "Analyzing Audio Data...";

    void
    updateSummaryChart();
    void updateVisibleWidgets();

    std::vector<IndividualFrequency *> freqWidgets;

    QMediaPlayer *mediaPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;

    QVBoxLayout *mainLayout = nullptr;
    QScrollArea *scrollArea = nullptr;

    QWidget *scrollContainer = nullptr;
    QWidget *contentWidget = nullptr;

    QPushButton *saveButton = nullptr;
    QPushButton *playButton = nullptr;

    QLabel *statusLabel = nullptr;
    QVBoxLayout *scrollLayout = nullptr;
    QChartView *summaryChartView = nullptr;

    QSpinBox *visibleCountSpinner = nullptr;
    QSpinBox *countSpinner = nullptr;

    std::vector<AudioAnalyser::FrequencyData> localFrequencies;
    int currentNumSamples = 0;
    int currentSampleRate = 0;
};