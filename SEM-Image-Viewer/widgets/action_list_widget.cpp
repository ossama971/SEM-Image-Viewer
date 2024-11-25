#include "action_list_widget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>

ActionListWidget::ActionListWidget(QWidget* parent)
    : QWidget(parent)
{
    // Main layout
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(5, 10, 10, 20);

    // Title
    auto titleLabel = new QLabel("Actions", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; background-color: transparent;");
    mainLayout->addWidget(titleLabel);

    // Container for items with vertical line
    auto container = new QWidget(this);
    auto containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(0, 5, 0, 0);

    // Container for items
    auto itemsContainer = new QWidget(container);
    m_itemsLayout = new QVBoxLayout(itemsContainer);
    m_itemsLayout->setContentsMargins(0, 0, 0, 0);
    m_itemsLayout->setSpacing(5);

    containerLayout->addWidget(itemsContainer);
    mainLayout->addWidget(container);

    // Set dark theme
   // setStyleSheet("QWidget { background-color: #2d2d2d; }");
}

void ActionListWidget::addWidget(QWidget* widget)
{
    m_itemsLayout->addWidget(widget);
}
