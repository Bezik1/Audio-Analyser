#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <vector>
#include "../../../core/AudioAnalyser/AudioAnalyser.hpp"

class IndividualFrequency : public QWidget
{
    Q_OBJECT
public:
    explicit IndividualFrequency(
        const AudioAnalyser::FrequencyData &data,
        int numSamples,
        int sampleRate,
        QWidget *parent = nullptr
    );

    AudioAnalyser::FrequencyData getData() const;
    void setData(const AudioAnalyser::FrequencyData &newData);

signals:
    void frequencyUpdated();

private:
    inline static QString FREQUENCY_LABEL_TEXT = "Freq: %1 Hz";
    inline static QString PHASE_TEXT = "Phase: ";
    inline static QString AMPLITUDE_TEXT = "Amp: ";

    void
    setupUi();
    void updateChart();
    QChartView *createChart();

    AudioAnalyser::FrequencyData data;
    int numSamples;
    int sampleRate;

    QLabel *frequencyLabel = nullptr;
    QDoubleSpinBox *ampSpin = nullptr;
    QDoubleSpinBox *phaseSpin = nullptr;
    QChartView *chartView= nullptr;
    QLineSeries *series = nullptr;
};