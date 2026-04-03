#pragma once

#include <QtWidgets>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>

class FileManagmentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileManagmentWidget(QWidget *parent = nullptr);

signals:
    void startAnalysisRequested(const QString &path);
    void pathChoosen(const QString &path);

private slots:
    void onBrowse();

private:
    QLineEdit *pathEdit;
    QPushButton *browseBtn;
    QPushButton *analyzeBtn;

    inline static const QString SELECT_MESSAGE = "Select Audio File";
    inline static const QString SELECT_FORMAT_INFO = "Wav Files (*.wav)";
    inline static const QString PATH_EDIT_PLACEHOLDER = "Select .wav file...";
    inline static const QString BROWSE_BTN_TEXT = "Browse";
    inline static const QString ANALYSE_BTN_TEXT = "Analyse Audios";
    inline static const QString ERROR_MSG = "Please select file from ";
};