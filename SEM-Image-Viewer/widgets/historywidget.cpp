#include "HistoryWidget.h"

HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
{

    QWidget *line = new QWidget(this);
    line->setStyleSheet("background-color: #aaaaaa;");
    line->setFixedHeight(1);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QString buttonsStyle = "QPushButton {"
                           "border: none;"
                           "font-family: 'Roboto';"
                           "padding:5px;"
                           "font:  14px;"
                           "text-align: center;"
                           "}"
                           "QPushButton:hover {"
                           "background-color: #d3d3d3;"
                           "}";
    // Create main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create a horizontal layout for the header and buttons
    QHBoxLayout *topLayout = new QHBoxLayout();

    // Create header label
    headerLabel = new QLabel("History", this);
    headerLabel->setAlignment(Qt::AlignCenter);

    QWidget *buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);

    topLayout->addWidget(headerLabel);

    // Create buttons
    undoButton = new QPushButton("▼", this);
    redoButton = new QPushButton("▲", this);

    undoButton->setStyleSheet(buttonsStyle);
    redoButton->setStyleSheet(buttonsStyle);

    undoButton->setFixedSize(50, 30);
    redoButton->setFixedSize(50, 30);

    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);

    // Set the button layout to the button widget
    buttonWidget->setLayout(buttonLayout);
    // Add buttons to the horizontal layout
    topLayout->addWidget(buttonWidget);

    // Add top layout to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(line);
    // Create action list
    actionList = new QListWidget(this);
    mainLayout->addWidget(actionList);

    // Connect buttons to slots
    connect(undoButton, &QPushButton::clicked, this, &HistoryWidget::undoAction);
    connect(redoButton, &QPushButton::clicked, this, &HistoryWidget::redoAction);

    // Set the main layout
    setLayout(mainLayout);
}

void HistoryWidget::addAction(const QString &action)
{
    // Add action to the undo stack and clear the redo stack
    undoStack.push(action);
    redoStack.clear();
    updateActionList();
}

void HistoryWidget::undoAction()
{
    if (!undoStack.isEmpty())
    {
        // Move the last action from undo stack to redo stack
        QString lastAction = undoStack.pop();
        redoStack.push(lastAction);
        updateActionList();
    }
}

void HistoryWidget::redoAction()
{
    if (!redoStack.isEmpty())
    {
        // Move the last action from redo stack to undo stack
        QString lastRedoAction = redoStack.pop();
        undoStack.push(lastRedoAction);
        updateActionList();
    }
}

void HistoryWidget::updateActionList()
{
    actionList->clear();
    // Display current actions in the undo stack
    QList<QString> actions = undoStack.toList();
    for (const QString &action : actions)
    {
        actionList->addItem(action);
    }
}
