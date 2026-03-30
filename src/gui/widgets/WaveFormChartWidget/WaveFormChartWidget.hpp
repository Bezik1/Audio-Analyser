#pragma once

#include <string>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QBrush>
#include <QColor>

class WaveformChartWidget : public QWidget
{
    Q_OBJECT
public:
    WaveformChartWidget(const std::vector<float> &samples, int sampleRate, const std::string &title, QWidget *parent = nullptr);

private:
    QChart *chart;
    QLineSeries *series;
};