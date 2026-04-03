#include "FileStreamWidget.hpp"

#include <QDir>
#include <QCoreApplication>
#include <QFileInfo>
#include <QLabel>

#include "../../styles/GlobalStyles.cpp"

FileStreamWidget::FileStreamWidget(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 20, 40, 20);

    mainLayout->addStretch();

    statusLabel = new QLabel("Please select an audio file first.", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(GlobalStyles::StatusFontColor);
    mainLayout->addWidget(statusLabel);

    contentWidget = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(15);

    QLabel *headerLabel = new QLabel("Available Audio Stream Files", this);
    headerLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #fff;");
    headerLabel->setAlignment(Qt::AlignCenter);
    contentLayout->addWidget(headerLabel);

    fileListWidget = new QListWidget(this);
    fileListWidget->setFixedWidth(1200 * 0.8);
    fileListWidget->setMinimumHeight(400);
    contentLayout->addWidget(fileListWidget, 0, Qt::AlignCenter);

    playButton = new QPushButton("Play Selected File", this);
    playButton->setFixedWidth(1200 * 0.7);
    playButton->setCursor(Qt::PointingHandCursor);
    contentLayout->addWidget(playButton, 0, Qt::AlignCenter);

    mainLayout->addWidget(contentWidget);

    mainLayout->addStretch();

    contentWidget->setVisible(false);
    statusLabel->setVisible(true);

    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.7);

    connect(playButton, &QPushButton::clicked, this, &FileStreamWidget::playSelectedFile);
    connect(fileListWidget, &QListWidget::itemDoubleClicked, this, &FileStreamWidget::playSelectedFile);
}

void FileStreamWidget::refreshFileList(const QString &selectedPath)
{
    fileListWidget->clear();

    if (selectedPath.isEmpty())
    {
        statusLabel->setVisible(true);
        contentWidget->setVisible(false);
        return;
    }

    QFileInfo fileInfo(selectedPath);
    QString baseDir = fileInfo.absolutePath();
    QString spectrumDir = baseDir + "/spectrums";

    QDir dir(baseDir);
    QStringList wavFiles = dir.entryList({"*.wav"}, QDir::Files);

    if (wavFiles.isEmpty())
    {
        statusLabel->setVisible(true);
        contentWidget->setVisible(false);
        return;
    }

    statusLabel->setVisible(false);
    contentWidget->setVisible(true);

    QList<QPair<QString, QString>> groups = {
        {"Current Directory", baseDir},
        {"Spectrums / Thresholds", spectrumDir}};

    for (const auto &group : groups)
    {
        QDir currentDir(group.second);
        if (!currentDir.exists())
            continue;

        QStringList groupWavs = currentDir.entryList({"*.wav"}, QDir::Files | QDir::NoDotAndDotDot);

        if (!groupWavs.isEmpty())
        {
            QListWidgetItem *header = new QListWidgetItem("─── " + group.first + " ───");
            header->setFlags(Qt::NoItemFlags);
            header->setForeground(Qt::gray);
            header->setTextAlignment(Qt::AlignCenter);
            fileListWidget->addItem(header);

            for (const QString &fileName : groupWavs)
            {
                QListWidgetItem *item = new QListWidgetItem(fileName);
                item->setData(Qt::UserRole, currentDir.absoluteFilePath(fileName));
                fileListWidget->addItem(item);
            }
        }
    }
}

void FileStreamWidget::playSelectedFile()
{
    QListWidgetItem *current = fileListWidget->currentItem();
    if (!current)
        return;

    QString fullPath = current->data(Qt::UserRole).toString();

    mediaPlayer->setSource(QUrl::fromLocalFile(fullPath));
    mediaPlayer->play();
}