#include "FileManagmentWidget.hpp"

FileManagmentWidget::FileManagmentWidget(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(15, 15, 15, 15);

    pathEdit = new QLineEdit(this);
    pathEdit->setPlaceholderText(PATH_EDIT_PLACEHOLDER);
    pathEdit->setReadOnly(true);

    browseBtn = new QPushButton(BROWSE_BTN_TEXT, this);
    analyzeBtn = new QPushButton(ANALYSE_BTN_TEXT, this);
    analyzeBtn->setEnabled(false);

    browseBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    analyzeBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    pathEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    int screenWidth = 1200;

    pathEdit->setFixedWidth(screenWidth * 0.6);
    browseBtn->setFixedWidth(screenWidth * 0.6);
    analyzeBtn->setFixedWidth(screenWidth * 0.6);

    layout->addStretch();
    layout->addWidget(pathEdit, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(browseBtn, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(analyzeBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(browseBtn, &QPushButton::clicked, this, &FileManagmentWidget::onBrowse);
    connect(analyzeBtn, &QPushButton::clicked, this, [this]()
            { emit startAnalysisRequested(pathEdit->text()); });
}

void FileManagmentWidget::onBrowse()
{
    QString dataDirPath = QDir::currentPath() + "/data";

    QDir().mkpath(dataDirPath);

    QString fullPath = QFileDialog::getOpenFileName(
        this,
        SELECT_MESSAGE,
        dataDirPath,
        SELECT_FORMAT_INFO);

    if (!fullPath.isEmpty())
    {
        if (fullPath.startsWith(dataDirPath))
        {
            pathEdit->setText(fullPath);
            analyzeBtn->setEnabled(true);

            emit pathChoosen(pathEdit->text());
        }
        else
        {
            QMessageBox::warning(this, "Error", ERROR_MSG + dataDirPath);
        }
    }
}