#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>

class FileStreamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileStreamWidget(QWidget *parent = nullptr);
    void refreshFileList(const QString &selectedPath = "");

private slots:
    void playSelectedFile();

private:
    QVBoxLayout *layout = nullptr;
    QLabel *statusLabel = nullptr;
    QWidget *contentWidget = nullptr;
    QListWidget *fileListWidget = nullptr;
    QPushButton *playButton = nullptr;

    QMediaPlayer *mediaPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;
};