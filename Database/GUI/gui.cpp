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
#include "GUI/GUIFileParser/GuiFileParser.h"
#include "dbclient.h"

NoSql::NoSql(Dispatcher* ndb)
{
    isServer = true;
    db = ndb;
    initActions();
    initSignals();
    initFileMenuBar();
}
NoSql::NoSql(DBClient* ndb)
{
    isServer = false;
    dbc = ndb;
    initActions();
    initSignals();
    initFileMenuBar();
}

void NoSql::initFileMenuBar(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadDataFilesAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    layoutManagement();
}
void NoSql::initSignals(){
    connect(loadDataFilesAction, SIGNAL(triggered()), this, SLOT(loadDataFiles()));
    connect(addColumnButton, SIGNAL(clicked()), this, SLOT(addColumn()));
    connect(removeColumnButton, SIGNAL(clicked()), this, SLOT(removeColumn()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));
    connect(loadDataFromDbButton, SIGNAL(clicked()), this, SLOT(loadDataFromDb()));
    connect(nextDataFromDbButton, SIGNAL(clicked()), this, SLOT(nextDataFromDb()));
}
void NoSql::initActions(){
    addColumnButton = new QPushButton("&Add column");
    openAction = new QAction(tr("&Open Structure"), this);
    saveAction = new QAction(tr("&Save Structure"), this);
    exitAction = new QAction(tr("&Exit"), this);
    removeColumnButton = new QPushButton("&Remove column");

    loadDataFilesAction = new QAction (tr("&Load data files"), this);
    loadDataFromDbButton = new QPushButton("Load data from db");
    nextDataFromDbButton = new QPushButton("Next 30 data");

}
void NoSql::layoutManagement(QTableWidget* table){
    QWidget* structure = new QWidget();
    QWidget* dbview = new QWidget();
    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->addTab(structure, "Structure");
    tabWidget->addTab(dbview, "Database");
    tableWidget = new QTableWidget();
    tableWidget->setRowCount(2);
    tableWidget->setColumnCount(3);
    //   setCentralWidget(tableWidget);
    setWindowTitle(tr("NoSql Database"));
    QHBoxLayout *layout = new QHBoxLayout();
    userIdField = new QLineEdit(tr("User Id"));
    tableIdField = new QLineEdit(tr("Table Id"));
    setCentralWidget(tabWidget);
    QVBoxLayout* buttonsAndFielsLayout = new QVBoxLayout();

    QVBoxLayout* columnsButtonLayout = new QVBoxLayout();
    columnsButtonLayout->addWidget(addColumnButton);
    columnsButtonLayout->addWidget(removeColumnButton);

    buttonsAndFielsLayout->addLayout(columnsButtonLayout);

    QVBoxLayout* idsLayout = new QVBoxLayout();
    idsLayout->addWidget(userIdField);
    idsLayout->addWidget(tableIdField);
    buttonsAndFielsLayout->addLayout(idsLayout);
    if (table==NULL){
        layout->addWidget(tableWidget);
    }else{
        delete tableWidget;
        tableWidget = table;
        layout->addWidget(table);
    }
    layout->addLayout(buttonsAndFielsLayout);
    layout->setStretchFactor(buttonsAndFielsLayout,1);
    layout->setStretchFactor(tableWidget,4);
    structure->setLayout(layout);
    int x=700;
    int y=400;
    setMinimumSize(x,y);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    QVBoxLayout* dbviewLayout = new QVBoxLayout();
    QHBoxLayout* dbfieldsLayout = new QHBoxLayout();

    userIdDb = new QLineEdit(tr("User Id"));
    tableIdDb = new QLineEdit(tr("Table Id"));
    processIdDb = new QLineEdit(tr("Process Id"));

    dbfieldsLayout->addWidget(userIdDb);
    dbfieldsLayout->addWidget(tableIdDb);
    dbfieldsLayout->addWidget(processIdDb);
    dbfieldsLayout->addWidget(loadDataFromDbButton);
    dbfieldsLayout->addWidget(nextDataFromDbButton);
    dbviewLayout->addLayout(dbfieldsLayout);
    tableWidgetDB = new QTableWidget();
    tableWidgetDB->setRowCount(2);
    tableWidgetDB->setColumnCount(3);
    dbviewLayout->addWidget(tableWidgetDB);
    dbview->setLayout(dbviewLayout);

}
void NoSql::addColumn(){
    tableWidget->setColumnCount(tableWidget->columnCount()+1);
}
void NoSql::removeColumn(){
    if(tableWidget->columnCount()>1){
        tableWidget->setColumnCount(tableWidget->columnCount()-1);
    }
}
void NoSql::loadDataFromDb(){
    int userId = userIdDb->text().toInt();
    int tableId = tableIdDb->text().toInt();
    int processId = processIdDb->text().toInt();
    MDPData* data;
    if (isServer){
        db->toStart(userId,tableId,processId);
        data = db->getNextStrings(userId,tableId,processId,30);
    }else{
        dbc->toStart(userId,tableId,processId);
        data = dbc->getNextStrings(userId,tableId,processId,30);
    }
    putDataInTable(data);
}
void NoSql::nextDataFromDb(){
    int userId = userIdDb->text().toInt();
    int tableId = tableIdDb->text().toInt();
    int processId = processIdDb->text().toInt();
    MDPData* data;
    if (isServer){
        data = db->getNextStrings(userId,tableId,processId,30);
    }else{
        data = dbc->getNextStrings(userId,tableId,processId,30);
    }
    putDataInTable(data);
}
void NoSql::putDataInTable(MDPData* data){
    vector<vector<QString> > rows = data->getCells();
    vector<QString> headers = data->getHeaders();
    int j=0;
    tableWidgetDB->setColumnCount(headers.size());
    tableWidgetDB->setRowCount(rows.size());
    foreach(QString header, headers){
        tableWidgetDB->setItem(0,j,new QTableWidgetItem(header));
        j++;
    }
    int i=1;
    foreach(vector<QString> cells, rows){
        int j=0;
        foreach(QString cell, cells){
            tableWidgetDB->setItem(i,j,new QTableWidgetItem(cell));
            j++;
        }
        i++;
    }
}
void NoSql::loadDataFiles(){
    QStringList fileNames =  QFileDialog::getOpenFileNames(this, tr("Open File"), "");
    vector<QString>* headers = new vector<QString>();
    vector<int>* numberHeaders = new vector<int>();
    qDebug()<<"Start reading structure table";
    for (int i=0;i<tableWidget->columnCount();i++){
        headers->push_back(tableWidget->item(0,i)->text());
        numberHeaders->push_back(tableWidget->item(1,i)->text().toInt());
    }
    qDebug()<<"Start loading files";
    foreach (const QString &filename, fileNames){
        GuiFileParser parser;
        parser.setBlockSize(10000);
        parser.setHeaders(headers,numberHeaders);
        parser.setIds(userIdField->text().toInt(),tableIdField->text().toInt(),0);
        qDebug() << filename;
        if (isServer){
            parser.loadFile(filename, *db);
        }else{
            parser.loadFile(filename, *dbc);
        }
    }
}
void NoSql::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                    tr("Structure Files (*.stc);"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        QTableWidget* widget = new QTableWidget();
        widget->setRowCount(2);
        bool flag =true;
        int i = 0;
        do{
            QString lineconst =  in.readLine();
            flag = lineconst!=NULL;
            if (flag){
                QStringList tokens= lineconst.split("\t",QString::SkipEmptyParts);
                qDebug() << tokens;
                if(i==0){
                    widget->setColumnCount(tokens.size());
                }
                int j=0;
                foreach( QString str, tokens) {
                    widget->setItem(i,j,new QTableWidgetItem(str));
                    j++;
                }
            }
            i++;
        }while (flag && i<2);
        layoutManagement(widget);
    }

}

void NoSql::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                    tr("Structure Files (*.stc);"));

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
