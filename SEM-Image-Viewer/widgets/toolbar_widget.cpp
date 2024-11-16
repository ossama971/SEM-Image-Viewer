#include "toolbar_widget.h"

ToolbarWidget::ToolbarWidget(QWidget *parent)
    : QWidget(parent) {
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#2C2C2C"));
    setPalette(palette);
    setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->setSpacing(0);

    // Create QToolButtons with icons
    imageViewButton = new QToolButton(this);
    diffViewButton = new QToolButton(this);
    gridViewButton = new QToolButton(this);
    seperatorIcon = new QToolButton(this);
    saveButton = new QToolButton(this);
    undoButton = new QToolButton(this);
    redoButton = new QToolButton(this);

    // Set icons and remove button padding
    imageViewButton->setIcon(QIcon(":/icons/image-icon.svg"));
    diffViewButton->setIcon(QIcon(":/icons/diff-icon.svg"));
    gridViewButton->setIcon(QIcon(":/icons/grid-icon.svg"));
    seperatorIcon->setIcon(QIcon(":/icons/toolbar-seperator-icon.svg"));
    saveButton->setIcon(QIcon(":/icons/save.svg"));
    undoButton->setIcon(QIcon(":/icons/undo.svg"));
    redoButton->setIcon(QIcon(":/icons/redo.svg"));

    imageViewButton->setIconSize(QSize(17, 17));
    diffViewButton->setIconSize(QSize(20, 20));
    gridViewButton->setIconSize(QSize(18, 18));
    seperatorIcon->setIconSize(QSize(23, 23));
    saveButton->setIconSize(QSize(17, 17));
    undoButton->setIconSize(QSize(16, 16));
    redoButton->setIconSize(QSize(16, 16));

    auto buttonStyle1 = "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
                        "QToolButton:checked { background-color: #555555; border-radius: 4px; }";

    auto buttonStyle2 = "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
                        "QToolButton:checked { background-color: #555555; border-radius: 4px; }"
                        "QToolButton:hover { background-color: #444444; border-radius: 4px; }";

    imageViewButton->setStyleSheet(buttonStyle1);
    diffViewButton->setStyleSheet(buttonStyle1);
    gridViewButton->setStyleSheet(buttonStyle1);
    seperatorIcon->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");
    saveButton->setStyleSheet(buttonStyle2);
    undoButton->setStyleSheet(buttonStyle2);
    redoButton->setStyleSheet(buttonStyle2);

    // Set the buttons to be checkable
    imageViewButton->setCheckable(true);
    diffViewButton->setCheckable(true);
    gridViewButton->setCheckable(true);

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
    setLayout(toolbarLayout);

    // Set the first button as checked by default
    imageViewButton->setChecked(true);

    // Disable save button by default
    saveButton->setEnabled(imageViewButton->isChecked());

    // Connections
    connect(imageViewButton, &QToolButton::toggled, saveButton, &QToolButton::setEnabled);
    connect(undoButton, &QToolButton::clicked, this, &ToolbarWidget::onUndoClicked);
    connect(redoButton, &QToolButton::clicked, this, &ToolbarWidget::onRedoClicked);
    connect(saveButton, &QToolButton::clicked, this, &ToolbarWidget::onSaveClicked);
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
