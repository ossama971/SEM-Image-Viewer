#include "toolbar_widget.h"

ToolbarWidget::ToolbarWidget(QWidget *parent)
    : QWidget(parent)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor("#2C2C2C")); // Use Window instead of Background
    setPalette(palette);
    setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setContentsMargins(0, 0, 0, 0);
    toolbarLayout->setSpacing(0);

    // Create QToolButtons with icons
    button1 = new QToolButton(this);
    button2 = new QToolButton(this);
    button3 = new QToolButton(this);
    QToolButton *seperatorIcon = new QToolButton(this);

    // Set icons and remove button padding
    button1->setIcon(QIcon(":/icons/image-icon.svg"));
    button2->setIcon(QIcon(":/icons/diff-icon.svg"));
    button3->setIcon(QIcon(":/icons/grid-icon.svg"));
    seperatorIcon->setIcon(QIcon(":/icons/toolbar-seperator-icon.svg"));

    button1->setIconSize(QSize(17, 17));
    button2->setIconSize(QSize(20, 20));
    button3->setIconSize(QSize(18, 18));
    seperatorIcon->setIconSize(QSize(23, 23));

    button1->setStyleSheet(
        "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
        "QToolButton:checked { background-color: #555555; border-radius: 4px; }");
    button2->setStyleSheet(
        "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
        "QToolButton:checked { background-color: #555555; border-radius: 4px; }");
    button3->setStyleSheet(
        "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
        "QToolButton:checked { background-color: #555555; border-radius: 4px; }");
    seperatorIcon->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");

    // Set the buttons to be checkable
    button1->setCheckable(true);
    button2->setCheckable(true);
    button3->setCheckable(true);
    // Create a button group to manage exclusive selection
    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(button1);
    buttonGroup->addButton(button2);
    buttonGroup->addButton(button3);

    // Add buttons to the toolbar layout
    toolbarLayout->addWidget(button1);
    toolbarLayout->addWidget(button2);
    toolbarLayout->addWidget(button3);
    toolbarLayout->addWidget(seperatorIcon);

    // Add a stretchable spacer to push the buttons to the left
    toolbarLayout->addStretch();
    setLayout(toolbarLayout);

    // Set the first button as checked by default
    button1->setChecked(true);

}
