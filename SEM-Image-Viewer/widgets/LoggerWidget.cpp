#include "LoggerWidget.h"

LoggerWidget::LoggerWidget(QWidget *parent, std::shared_ptr<MessageDataModel> dataModel)
    : QWidget(parent), m_dataModel(std::move(dataModel))
{

    // Layout setup
    // Create the top horizontal layout for compact layout
    createButtons();
    createLayouts();
    createConnections();
    // Set initial state
    isExpanded = true;
    stackedWidget->setCurrentIndex(0);

    if (m_dataModel == nullptr) {
        m_dataModel = std::make_shared<MessageDataModel>();
    }
    Logger::instance()->setModel(m_dataModel);
    this->logListWidget->setModel(m_dataModel.get());
}

void LoggerWidget::createButtons()
{
    QString buttonsStyle = "QPushButton {"
                           "border: none;"
                           "font-family: 'Roboto';"
                           "padding:5px;"
                           "padding-right: 20px;"
                           "font:  14px;"
                           "text-align: center;"
                           "}";

    // Buttons for filtering logs by type
    allShowButton = new QPushButton("All", this);

    allShowButton->setStyleSheet(
        buttonsStyle);

    infoShowButton = new QPushButton("Info", this);
    infoShowButton->setIcon(QIcon("A:/Siemens_Academy/GP/project/sem-image-viewer/SEM-Image-Viewer/assets/information_icon.png"));
    infoShowButton->setIconSize(QSize(16, 16));
    infoShowButton->setStyleSheet(buttonsStyle);

    errorsShowButton = new QPushButton("Errors", this);
    errorsShowButton->setIcon(QIcon("A:/Siemens_Academy/GP/project/sem-image-viewer/SEM-Image-Viewer/assets/errors_icon.png"));
    errorsShowButton->setIconSize(QSize(16, 16));
    errorsShowButton->setStyleSheet(buttonsStyle);

    warningsShowButton = new QPushButton("Warnings", this);
    warningsShowButton->setIcon(QIcon("A:/Siemens_Academy/GP/project/sem-image-viewer/SEM-Image-Viewer/assets/warnings_icon.png"));
    warningsShowButton->setIconSize(QSize(16, 16));
    warningsShowButton->setStyleSheet(buttonsStyle);

    switchLayoutButtonFull = new QPushButton("   ▼", this);
    switchLayoutButtonFull->setStyleSheet(buttonsStyle);

    switchLayoutButtonCompact = new QPushButton("   ▲", this);
    switchLayoutButtonCompact->setStyleSheet(buttonsStyle);
    //switchLayoutButtonCompact->setFixedWidth(50);
    // Search line edit for filtering logs by text
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search log message");
    // Set a fixed width (e.g., 200 pixels)
    //searchLineEdit->setFixedWidth(300);

    // Set the style sheet for rounded borders and other properties
    searchLineEdit->setStyleSheet(""
        "QLineEdit {"
        "border: 2px solid #aaaaaa;"
        "border-radius: 10px;"
        "padding: 5px;"
        "}"
        "QLineEdit:focus {"
        "border: 2px solid #0078d7;"
        "}");

    // Log list widget for displaying messages
    logListWidget = new QListView(this);
    logListWidget->setStyleSheet(
        "QListWidget {"
        "border: none;"
        "background: transparent;"
        "}");
}

void LoggerWidget::createConnections()
{
    connect(allShowButton, &QPushButton::clicked, this, &LoggerWidget::filterLogs);
    connect(infoShowButton, &QPushButton::clicked, this, &LoggerWidget::filterLogs);
    connect(errorsShowButton, &QPushButton::clicked, this, &LoggerWidget::filterLogs);
    connect(warningsShowButton, &QPushButton::clicked, this, &LoggerWidget::filterLogs);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &LoggerWidget::filterLogs);
    connect(switchLayoutButtonFull, &QPushButton::clicked, this, &LoggerWidget::switchLayout);
    connect(switchLayoutButtonCompact, &QPushButton::clicked, this, &LoggerWidget::switchLayout);
}

void LoggerWidget::createLayouts()
{
    QWidget *line = new QWidget(this);
    line->setStyleSheet("background-color: #aaaaaa;");
    line->setFixedHeight(2);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *topLayoutCompact = new QHBoxLayout;
    topLayoutCompact->addWidget(new QLabel("Error : 1 Warnings : 2 Info : 3", this));
    topLayoutCompact->addStretch(9);
    topLayoutCompact->addWidget(switchLayoutButtonCompact);
    topLayoutCompact->addStretch(1);

    QHBoxLayout *topLayoutFull = new QHBoxLayout;
    topLayoutFull->addWidget(allShowButton);
    topLayoutFull->addWidget(infoShowButton);
    topLayoutFull->addWidget(warningsShowButton);
    topLayoutFull->addWidget(errorsShowButton);
    topLayoutFull->addSpacing(50);
    topLayoutFull->addWidget(searchLineEdit);
    topLayoutFull->addWidget(switchLayoutButtonFull);

    // Why i canot use the same topLayout

    // Create the compact vertical layout without logListWidget
    compactLayout = new QVBoxLayout();

    compactLayout->addLayout(topLayoutCompact);
    // Create the full vertical layout
    fullLayout = new QVBoxLayout();
    fullLayout->addLayout(topLayoutFull);
    fullLayout->addWidget(line);
    fullLayout->addWidget(logListWidget);

    // Set the initial layout
    stackedWidget = new QStackedWidget(this);

    QWidget *fullViewWidget = new QWidget();
    fullViewWidget->setLayout(fullLayout);

    QWidget *compactViewWidget = new QWidget();
    compactViewWidget->setLayout(compactLayout);

    stackedWidget->addWidget(fullViewWidget);
    stackedWidget->addWidget(compactViewWidget);

    // Set stackedWidget as the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void LoggerWidget::filterLogs()
{
    QString filterText = searchLineEdit->text().toLower();
    QString selectedType = "";
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    if (clickedButton == infoShowButton) {
        selectedType = "Info";
    }
    else if (clickedButton == warningsShowButton) {
        selectedType = "Warning";
    }
    else if (clickedButton == errorsShowButton) {
        selectedType = "Error";
    }
    std::string fmt = boost::str(boost::format("%1% %2%") % filterText.toStdString() % selectedType.toStdString());
    this->m_dataModel->setFilterText(QString::fromStdString(fmt));
}

void LoggerWidget::switchLayout()
{
    if (isExpanded)
    {
        stackedWidget->setCurrentIndex(1); // Show compact view
                                           // Change to up arrow icon
    }
    else
    {
        stackedWidget->setCurrentIndex(0); // Show full view
    }

    isExpanded = !isExpanded; // Toggle state
}
