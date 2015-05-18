#ifndef GUI_CPP
#define GUI_CPP

#include "gui.h"
#include "QtWidgets/QVBoxLayout"
#include "QtWidgets/QMessageBox"
#include <QtWidgets/QApplication>
NoSql::NoSql()
{
    textEdit = new QTextEdit;
    quitButton = new QPushButton(tr("Quit"));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(quitButton);
    setLayout(layout);
    setWindowTitle(tr("NoSQL Database"));
}

void NoSql::quit()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("NoSQL Database"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}

#endif
