#include "TabsWidget.h"
#include <QDebug>

TabsWidget::TabsWidget(QWidget *parent) : QWidget(parent)
{
    layout_ = new QVBoxLayout;
}

TabsWidget::~TabsWidget()
{
    deleteButtons();
    delete layout_;
}

void TabsWidget::deleteButtons()
{
    foreach (QPushButton *button, buttons_) {
        delete button;
    }
}

void TabsWidget::setTabs(int tabsCount)
{
    deleteButtons();
    buttons_.clear();
    for (int i = 0; i < tabsCount; i++) {
        QPushButton *button = new QPushButton("Узел " + QString::number(i+1), this);
        buttons_.append(button);
        layout_->addWidget(button);
        QObject::connect(button, &QPushButton::clicked, [this, i] {
            emit this->buttonClicked(i);
            qDebug() << "tabClicked" << i+1;
        });
    }
    setLayout(layout_);
}

