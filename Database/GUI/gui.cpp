#ifndef GUI_CPP
#define GUI_CPP

#include "gui.h"
#include <QtGui>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMainWindow>
NoSql::NoSql()
{
    addColumnButton = new QPushButton("&Add column");
    openAction = new QAction(tr("&Open"), this);
    saveAction = new QAction(tr("&Save"), this);
    exitAction = new QAction(tr("&Exit"), this);
    removeColumnButton = new QPushButton("&Remove column");
    connect(addColumnButton, SIGNAL(clicked()), this, SLOT(addColumn()));
    connect(removeColumnButton, SIGNAL(clicked()), this, SLOT(removeColumn()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    tableWidget = new QTableWidget();
    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(3);
 //   setCentralWidget(tableWidget);
  //  setWindowTitle(tr("NoSql Database"));
    QHBoxLayout *layout = new QHBoxLayout;

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);
    layout->addWidget(tableWidget);
    layout->addWidget(addColumnButton);
    layout->addWidget(removeColumnButton);
    int x=700;
    int y=400;
    setMinimumSize(x,y);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
}
void NoSql::addColumn(){
    tableWidget->setColumnCount(tableWidget->columnCount()+1);
}
void NoSql::removeColumn(){
    if(tableWidget->columnCount()>1){
         tableWidget->setColumnCount(tableWidget->columnCount()-1);
    }
}
void NoSql::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        textEdit->setText(in.readAll());
        file.close();
    }
}

void NoSql::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                    tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream data( &file );
            QStringList strList;
            //            for( int c = 0; c <tableWidget->columnCount(); ++c )
            //            {
            //                strList <<
            //                           "\" " +
            //                          tableWidget->horizontalHeaderItem(c)->data(Qt::DisplayRole).toString() +
            //                           "\" ";
            //            }
            //            strList <<
            //                       "\" " +
            //                      textEdit->toPlainText().toUtf8() +
            //                       "\" ";
            //            data << strList.join(";") << "\n";
            for( int r = 0; r <tableWidget->rowCount(); ++r )
            {
                strList.clear();
                for( int c = 0; c <tableWidget->columnCount(); ++c )
                {   QTableWidgetItem* item =tableWidget->item(r,c);        //Load items
                    if (!item || item->text().isEmpty())                        //Test if there is something at item(r,c)
                    {
                        tableWidget->setItem(r,c,new QTableWidgetItem("0"));//IF there is nothing write 0
                    }
                    strList <<tableWidget->item( r, c )->text() +"\t";

                }
                data << strList.join( "" )+"\n";
            }
            //          statusBar()->showMessage(tr("File saved successfully."), 3000);
            file.close();
        }
    }
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
