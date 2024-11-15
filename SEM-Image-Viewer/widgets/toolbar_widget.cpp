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

    // Set icons and remove button padding
    button1->setIcon(QIcon(":/icons/image-icon.svg"));
    button2->setIcon(QIcon(":/icons/diff-icon.svg"));
    button3->setIcon(QIcon(":/icons/grid-icon.svg"));
    button1->setIconSize(QSize(17, 17));
    button2->setIconSize(QSize(20, 20));
    button3->setIconSize(QSize(18, 18));

    button1->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");
    button2->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");
    button3->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");

    // Add buttons to the toolbar layout
    toolbarLayout->addWidget(button1);
    toolbarLayout->addWidget(button2);
    toolbarLayout->addWidget(button3);

    setLayout(toolbarLayout);
}
