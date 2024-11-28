#include "logger_widget.h"
#include <QLabel>
#include <QListWidgetItem>
#include <QToolButton>

LoggerWidget::LoggerWidget(QWidget *parent)
    : QWidget(parent)
{

    // Layout setup
    // Create the top horizontal layout for compact layout
    createButtons();
    createLayouts();
    createConnections();
    // Set initial state
    isExpanded = true;
    stackedWidget->setCurrentIndex(0);

    // LogCard *logCard = new LogCard("Hello", "World", false, this);
    //

    // logListWidget->addWidget(logCard);
}

void LoggerWidget::createButtons()
{
    QString buttonsStyle = "QToolButton {"
                           "border: none;"
                           "font-family: 'Roboto';"
                           "padding:5px;"
                           "padding-right: 20px;"
                           "font:  12px;"
                           "text-align: center;"
                           "}";

    // Buttons for filtering logs by type
    allShowButton = new QToolButton( this);
    allShowButton->setText("All");
    allShowButton->setObjectName("all");

    infoShowButton = new QToolButton(this);
    infoShowButton->setObjectName("info");
    infoShowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    infoShowButton->setText("Info");
    infoShowButton->setStyleSheet(buttonsStyle);

    errorsShowButton = new QToolButton( this);
    errorsShowButton->setObjectName("error");
    errorsShowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    errorsShowButton->setText("Errors");
    errorsShowButton->setStyleSheet(buttonsStyle);

    warningsShowButton = new QToolButton(this);
    warningsShowButton->setObjectName("warning");
    warningsShowButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    warningsShowButton->setText("Warnings");
    warningsShowButton->setStyleSheet(buttonsStyle);
    allShowButton->setCheckable(true);
    allShowButton->setChecked(true);
    infoShowButton->setCheckable(true);
    errorsShowButton->setCheckable(true);
    warningsShowButton->setCheckable(true);
    switchLayoutButtonFull = new QPushButton("   ▼", this);
    switchLayoutButtonFull->setStyleSheet(buttonsStyle);

    switchLayoutButtonCompact = new QPushButton("   ▲", this);
    switchLayoutButtonCompact->setStyleSheet(buttonsStyle);
    // switchLayoutButtonCompact->setFixedWidth(50);
    //  Search line edit for filtering logs by text
    QWidget *searchWidget = new QWidget(this);
    QHBoxLayout *searchContainerLayout = new QHBoxLayout(searchWidget);
    searchContainerLayout->setContentsMargins(2, 2, 2, 2); // Padding inside the border
    searchContainerLayout->setSpacing(0);
    layout = new QHBoxLayout(searchWidget);
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("Search log message");
    searchWidget->setMinimumWidth(300);
    searchWidget->setMaximumWidth(400);
    // Set a fixed width (e.g., 200 pixels)
    // searchLineEdit->setFixedWidth(300);
    // Set the style sheet for rounded borders and other properties
    searchLineEdit->setStyleSheet(""
                                  "QLineEdit {"
                                  "    border: none;"
                                  "    padding: 2px;"
                                  "}");
    QPushButton *searchIcon = new QPushButton(this);
    searchIcon->setObjectName("searchButton2");

    searchContainerLayout->addWidget(searchLineEdit);
    searchContainerLayout->addWidget(searchIcon);
    searchWidget->setStyleSheet(
        "QWidget {"
        "    border: 2px solid #aaaaaa;"
        "    border-radius: 8px;"
        "    padding: 2px;"
        "}"
        "QWidget:focus-within {"
        "    border: 2px solid #0078d7;"
        "}"
        "QPushButton {"
        "    border: 0;"
        "}"
        "QPushButton:hover {"
        "    border-style: inset;"
        "}"
        "QPushButton:pressed {"
        "    border-style: inset;"
        "}");
    layout->addWidget(searchWidget);
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
    QWidget *layoutContainer = new QWidget();
    layoutContainer->setLayout(layout);
    QWidget *line = new QWidget(this);
    line->setStyleSheet("background-color: #aaaaaa;");
    line->setFixedHeight(2);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *topLayoutCompact = new QHBoxLayout;
    logShortInfo = new QLabel(this);
    updateLogs(0, 0, 0);

    topLayoutCompact->addWidget(logShortInfo);
    topLayoutCompact->addStretch(9);
    topLayoutCompact->addWidget(switchLayoutButtonCompact);
    topLayoutCompact->addStretch(0);

    QHBoxLayout *topLayoutFull = new QHBoxLayout;
    topLayoutFull->addWidget(allShowButton);
    topLayoutFull->addWidget(infoShowButton);
    topLayoutFull->addWidget(warningsShowButton);
    topLayoutFull->addWidget(errorsShowButton);
    topLayoutFull->addStretch(1);
    topLayoutFull->addWidget(layoutContainer);
    topLayoutFull->addWidget(switchLayoutButtonFull);

    // Why i canot use the same topLayout

    // Create the compact vertical layout without logListWidget
    compactLayout = new QVBoxLayout();

    compactLayout->addLayout(topLayoutCompact);
    // Create the full vertical layout
    fullLayout = new QVBoxLayout();
    fullLayout->addLayout(topLayoutFull);
    fullLayout->addWidget(line);

    logListLayout = new QListWidget();
    logListLayout->setSpacing(0);
    logListLayout->setStyleSheet(
        "QListWidget {"
        "   padding: 0px; "
        "   margin: 0px; "
        "   border: none; "
        "}"
        );


    fullLayout->addWidget(logListLayout);


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
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

void LoggerWidget::updateLogs(int info, int warning, int error) {
    infoCount = info;
    warningCount = warning;
    errorCount = error;

    QString labelText = QString("Info: %1   Warning: %2   Error: %3")
                            .arg(infoCount)
                            .arg(warningCount)
                            .arg(errorCount);

    logShortInfo->setText(labelText);
}

void LoggerWidget::filterLogs()
{
    QString filterText = searchLineEdit->text();
    QString selectedType = "";
    QToolButton *clickedButton = qobject_cast<QToolButton *>(sender());


    if (clickedButton == infoShowButton)
    {
        selectedType = "Info";
        clickedButton->setChecked(true);
        allShowButton->setChecked(false);
        warningsShowButton->setChecked(false);
        errorsShowButton->setChecked(false);
    }
    else if (clickedButton == warningsShowButton)
    {
        selectedType = "Warning";
        clickedButton->setChecked(true);
        allShowButton->setChecked(false);
        infoShowButton->setChecked(false);
        errorsShowButton->setChecked(false);
    }
    else if (clickedButton == errorsShowButton)
    {
        selectedType = "Error";
        clickedButton->setChecked(true);
        allShowButton->setChecked(false);
        warningsShowButton->setChecked(false);
        infoShowButton->setChecked(false);
    }
    else if(clickedButton == allShowButton){
        clickedButton->setChecked(true);
        infoShowButton->setChecked(false);
        warningsShowButton->setChecked(false);
        errorsShowButton->setChecked(false);
    }


    logListLayout->clear();
    emit showSelectedType(selectedType ,filterText);
}

void LoggerWidget::switchLayout()
{
    if (isExpanded)
    {
        emit getLogStats();
        stackedWidget->setCurrentIndex(1); // Show compact view
    }
    else
    {
        stackedWidget->setCurrentIndex(0); // Show full view
    }

    isExpanded = !isExpanded; // Toggle state
    emit layoutSwitched(isExpanded); // Notify the state change
}

void LoggerWidget::addLogCard(LogCard *card)
{

    QListWidgetItem *item = new QListWidgetItem(logListLayout);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);  // Disable selection
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    item->setSizeHint(QSize(card->sizeHint().width(), card->sizeHint().height()));
    logListLayout->addItem(item);
    logListLayout->setItemWidget(item, card);
    logListLayout->scrollToItem(item);

    if(card->getType()=="error"){
        errorCount++;
    }
    else if(card->getType()=="info"){
        infoCount++;
    }
    else{
        warningCount++;
    }
}


