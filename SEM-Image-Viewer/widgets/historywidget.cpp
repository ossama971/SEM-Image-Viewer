#include "HistoryWidget.h"

HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create a horizontal layout for the header and buttons
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Create header label
    headerLabel = new QLabel("History", this);
    headerLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    headerLayout->addWidget(headerLabel);

    // Add a spacer to push buttons to the right
    headerLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Create Undo button with icon
    undoButton = new QPushButton("▲",this);
    // undoButton->setIcon(QIcon(":/icons/undo_icon.png")); // Set your undo icon path here
    // undoButton->setIconSize(QSize(12, 12));
    /*undoButton->setStyleSheet("background: none; border: none;");*/ // Remove background and border
    headerLayout->addWidget(undoButton);

    // Create Redo button with icon
    redoButton = new QPushButton("▼",this);
    // redoButton->setIcon(QIcon(":/icons/redo_icon.png")); // Set your redo icon path here
    // redoButton->setIconSize(QSize(12, 12));
   // redoButton->setStyleSheet("background: none; border: none;"); // Remove background and border
    headerLayout->addWidget(redoButton);

    // Add header layout to main layout
    mainLayout->addLayout(headerLayout);


    // Create action list
    actionList = new QListWidget(this);
    mainLayout->addWidget(actionList);



    // Connect buttons to slots
    connect(undoButton, &QPushButton::clicked, this, &HistoryWidget::undoAction);
    connect(redoButton, &QPushButton::clicked, this, &HistoryWidget::redoAction);

    // Set the main layout
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}

void HistoryWidget::addAction(const QString &action)
{
    // Add action to the undo stack and clear the redo stack
    undoStack.push(action);
    redoStack.clear();
    updateActionList();
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

void HistoryWidget::undoAction(){
    emit undo();
}
void HistoryWidget::redoAction(){
    emit redo();
}
