#include "toolbar_widget.h"

ToolbarWidget::ToolbarWidget(QWidget *parent)
    : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setObjectName("ToolbarWidget");
    setAttribute(Qt::WA_StyledBackground, true);
    QHBoxLayout *toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->setSpacing(0);
    // Create QToolButtons with icons
    imageViewButton = new QToolButton(this);
    diffViewButton = new QToolButton(this);
    gridViewButton = new QToolButton(this);
    seperatorIcon = new QToolButton(this);
    seperatorIcon2 = new QToolButton(this);
    saveButton = new QToolButton(this);
    undoButton = new QToolButton(this);
    redoButton = new QToolButton(this);
    minimizeToolbarButton = new QToolButton(this);
    minimizeLoggerButton = new QToolButton(this);

    imageViewButton->setObjectName("view");
    diffViewButton->setObjectName("diff");
    gridViewButton->setObjectName("grid");
    seperatorIcon->setObjectName("seperator");
    seperatorIcon2->setObjectName("seperator");
    saveButton->setObjectName("saveButton");
    undoButton->setObjectName("undoButton");
    redoButton->setObjectName("redoButton");
    minimizeToolbarButton->setObjectName("arrowup");
    minimizeLoggerButton->setObjectName("closelog");

    auto buttonStyle1 = "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
                        "QToolButton:checked { border-radius: 4px; }"
                        "QToolButton:hover { border-radius: 4px; }";
    auto buttonStyle2 = "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
                        "QToolButton:checked { border-radius: 4px; padding: 0px 3px 0px 3px;}"
                        "QToolButton:hover { border-radius: 4px; padding: 0px 3px 0px 3px;}";

    imageViewButton->setStyleSheet(buttonStyle1);
    diffViewButton->setStyleSheet(buttonStyle1);
    gridViewButton->setStyleSheet(buttonStyle1);
    seperatorIcon->setStyleSheet("QToolButton {margin: 0px; padding: 0px; border: none; }");
    seperatorIcon2->setStyleSheet("QToolButton {margin: 0px; padding: 0px; border: none; }");
    saveButton->setStyleSheet(buttonStyle1);
    undoButton->setStyleSheet(buttonStyle1);
    redoButton->setStyleSheet(buttonStyle1);
    minimizeLoggerButton->setStyleSheet(buttonStyle2);
    minimizeToolbarButton->setStyleSheet(buttonStyle2);

    // Set the buttons to be checkable
    imageViewButton->setCheckable(true);
    diffViewButton->setCheckable(true);
    gridViewButton->setCheckable(true);
    minimizeLoggerButton->setCheckable(true);
    minimizeToolbarButton->setCheckable(true);

    // Create a button group to manage exclusive selection
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(imageViewButton);
    buttonGroup->addButton(diffViewButton);
    buttonGroup->addButton(gridViewButton);

    // Add buttons to the toolbar layout
    toolbarLayout->addWidget(imageViewButton);
    toolbarLayout->addWidget(diffViewButton);
    toolbarLayout->addWidget(gridViewButton);
    toolbarLayout->addWidget(seperatorIcon);
    toolbarLayout->addWidget(saveButton);
    toolbarLayout->addWidget(undoButton);
    toolbarLayout->addWidget(redoButton);

    // Add a stretchable spacer to push the buttons to the left
    toolbarLayout->addStretch();

    // Add right-aligned buttons
    toolbarLayout->addWidget(seperatorIcon2);
    toolbarLayout->addWidget(minimizeLoggerButton);
    toolbarLayout->addWidget(minimizeToolbarButton);

    setLayout(toolbarLayout);

    // Set buttons as checked by default
    imageViewButton->setChecked(true);
    minimizeToolbarButton->setChecked(true);
    minimizeLoggerButton->setChecked(true);

    // Disable save button by default
    saveButton->setEnabled(imageViewButton->isChecked());

    // Connections
    connect(imageViewButton, &QToolButton::toggled, saveButton, &QToolButton::setEnabled);
    connect(undoButton, &QToolButton::clicked, this, &ToolbarWidget::onUndoClicked);
    connect(redoButton, &QToolButton::clicked, this, &ToolbarWidget::onRedoClicked);
    connect(saveButton, &QToolButton::clicked, this, &ToolbarWidget::onSaveClicked);
    connect(minimizeLoggerButton, &QToolButton::toggled, this, &ToolbarWidget::onMinimizeLoggerClicked);
    connect(minimizeToolbarButton, &QToolButton::toggled, this, &ToolbarWidget::onMinimizeToolbarClicked);
}

void ToolbarWidget::onSelectDiffView() {
    diffViewButton->setChecked(true);
}

void ToolbarWidget::onUndoClicked() {
    emit undoTriggered();
}

void ToolbarWidget::onRedoClicked() {
    emit redoTriggered();
}

void ToolbarWidget::onSaveClicked() {
    emit saveButtonClicked();
}

void ToolbarWidget::onMinimizeLoggerClicked(bool checked) {
    emit minimizeLoggerClicked(checked);
}

void ToolbarWidget::onMinimizeToolbarClicked(bool checked){
    emit minimizeToolbarClicked(checked);
}
