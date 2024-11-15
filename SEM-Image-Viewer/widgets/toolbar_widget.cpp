#include "toolbar_widget.h"

ToolbarWidget::ToolbarWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #333333; border-radius: 8px;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *toolbarLayout = new QHBoxLayout(this);
    toolbarLayout->setContentsMargins(0, 0, 0, 0); // No margins around layout
    toolbarLayout->setSpacing(-1);

    // Create QToolButtons with icons
    button1 = new QToolButton(this);
    button2 = new QToolButton(this);
    button3 = new QToolButton(this);

    // Set icons and remove button padding
    button1->setIcon(QIcon(":/icons/edit-view.png"));
    button2->setIcon(QIcon(":/icons/edit-view.png"));
    button3->setIcon(QIcon(":/icons/edit-view.png"));
    button1->setIconSize(QSize(24, 24));
    button2->setIconSize(QSize(24, 24));
    button3->setIconSize(QSize(24, 24));

    button1->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");
    button2->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");
    button3->setStyleSheet("QToolButton { margin: 0px; padding: 0px; border: none; }");

    // Add buttons to the toolbar layout
    toolbarLayout->addWidget(button1);
    toolbarLayout->addWidget(button2);
    toolbarLayout->addWidget(button3);

    setLayout(toolbarLayout);
}
