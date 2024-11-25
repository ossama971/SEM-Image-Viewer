#include "log_card_widget.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QVBoxLayout>
LogCard::LogCard(const QString &typeOfMessage,const QString &headerText, const QString &detailsText, bool progressBarFlag,const QString & path, QWidget *parent)
    : QWidget(parent), detailsVisible(false)
{

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // auto line = new QFrame(this);
    // line->setFrameShape(QFrame::HLine);
    // line->setFrameShadow(QFrame::Plain);
    // line->setStyleSheet("color: #404040;");

    _type=typeOfMessage;
    _path=path;
    // Header horizontal layout
    QHBoxLayout *headerLayout = new QHBoxLayout();


    // Header label
    headerLabel = new QLabel("["+_type+"] "+headerText, this);
    headerLabel->setStyleSheet(R"(
    font-weight: semi-bold;
    font-size: 12px;
    background: transparent;
    )");

    headerLayout->addWidget(headerLabel);

    // Spacer
    headerLayout->addStretch(1);

    // Details button


    if(progressBarFlag){
        progressBar=new ProgressBarComponent();
        headerLayout->addWidget(progressBar);
    }
    if(detailsText!=""){
        detailsBtn = new QPushButton("Show Details", this);
        detailsBtn->setStyleSheet(R"(
        QPushButton {
            font-weight: semi-bold;
            font-size: 10px;
            background: transparent;
            color: #3C99D8;
        }
        QPushButton:hover {
            text-decoration: underline;
        }
        )");
        headerLayout->addWidget(detailsBtn);
        connect(detailsBtn, &QPushButton::clicked, this, &LogCard::toggleDetails);
    }
    copyBtn=new QPushButton(this);
    copyBtn->setObjectName("copyBtn");
    headerLayout->addWidget(copyBtn);

    // Add header layout to main layout
    mainLayout->addLayout(headerLayout);
   // mainLayout->addWidget(line);



    if(detailsText!=""){
        detailsContainer = new QWidget(this);
        detailsContainer->setStyleSheet(R"(
        background-color: #f5f5f5;
        )");
    // Create vertical layout for container
        QVBoxLayout* containerLayout = new QVBoxLayout(detailsContainer);


        // Add the details text
        QLabel* detailsLabel = new QLabel(detailsText, detailsContainer);
        detailsLabel->setWordWrap(true);
        detailsLabel->setStyleSheet("color: #333; font-size: 10px;");
        containerLayout->addWidget(detailsLabel);

        // Add the link
        QLabel* linkLabel = new QLabel("More Information →", detailsContainer);
        linkLabel->setStyleSheet("color: #0066cc; ");
        linkLabel->setCursor(Qt::PointingHandCursor);
        containerLayout->addWidget(linkLabel);

        detailsContainer->setVisible(false);

        // Add to main layout
        mainLayout->addWidget(detailsContainer);


        connect(linkLabel, &QLabel::linkActivated, this, &LogCard::onClickOnPath);

    }
    // Set the main layout
    setLayout(mainLayout);

    // Connect details button

    connect(copyBtn, &QPushButton::clicked, this, &LogCard::onCopyContent);
}
void LogCard::toggleDetails()
{
    // Toggle visibility
    detailsVisible = !detailsVisible;
    detailsContainer->setVisible(detailsVisible);

    // Update button text
    detailsBtn->setText(detailsVisible ? "Hide Details" : "Show Details");
    emit sizeChanged();
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
