#include "log_card_widget.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QApplication>
#include <QPalette>

LogCard::LogCard(const QString &typeOfMessage,const QString &headerText, bool progressBarFlag,bool colorFlag,const QString & path, QWidget *parent)
    : QWidget(parent)
{

    // Create a container widget
    QWidget *containerWidget = new QWidget(this);
    if(colorFlag){
        containerWidget->setStyleSheet("background-color: #dee2e6; ");
    }
    // Main vertical layout for the container


    _type = typeOfMessage;
    _path = path;

    // Header horizontal layout
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QString fullText = "["+_type+"] "+headerText;
    int maxLength = 80;

    QString displayText = fullText;
    if (fullText.length() > maxLength) {
        displayText = fullText.left(maxLength) + "...";
    }
    // Header label
    headerLabel = new QLabel(displayText, this);
    headerLabel->setToolTip(fullText);
    if(colorFlag ){
        headerLabel->setStyleSheet(R"(
        font-weight: semi-bold;
        font-size: 12px;
        background: transparent;
        color: black;
        )");
    }
    else{
        headerLabel->setStyleSheet(R"(
        font-weight: semi-bold;
        font-size: 12px;
        background: transparent;
        )");
    }
    headerLabel->setObjectName("loggerText");
    headerLayout->addWidget(headerLabel);
    headerLayout->setContentsMargins(5, 5, 5, 5);

    if(path!=""){
        pathBtn= new QPushButton(this);
        pathBtn->setObjectName("pathBtn");
        headerLayout->addWidget(pathBtn);
        connect(pathBtn, &QPushButton::clicked, this, &LogCard::onClickOnPath);
    }
    // Spacer
    headerLayout->addStretch(1);

    // Progress bar
    if(progressBarFlag){
        progressBar = new ProgressBarComponent();
        headerLayout->addWidget(progressBar);
    }

    // Copy button
    copyBtn = new QPushButton(this);
    copyBtn->setObjectName("copyBtn");
    headerLayout->addWidget(copyBtn);


    // Set the main layout for the container
    containerWidget->setLayout(headerLayout);
    containerWidget->setContentsMargins(0, 0, 0, 0);
    // Create a main layout for the entire widget
    QHBoxLayout *mainWidgetLayout = new QHBoxLayout(this);
    mainWidgetLayout->addWidget(containerWidget);
    mainWidgetLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainWidgetLayout);

    // Connect copy button
    connect(copyBtn, &QPushButton::clicked, this, &LogCard::onCopyContent);
}

ProgressBarComponent* LogCard::getProgressBar(){
    return progressBar;
}

QString LogCard::getType(){
    return _type;
}

void LogCard::onCopyContent(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(headerLabel->text());
}

void LogCard::onClickOnPath(){

    QDesktopServices::openUrl(QUrl(_path));

}
