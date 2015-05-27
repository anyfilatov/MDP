#include "mywindow.h"
#include "realclients.h"

MyWindow1::MyWindow1(integration::RealClientBD* clientBD, integration::RealClientDisp* clientDisp, integration::RealClientRBT* clientRBT){
    QLabel * namelabelRBT = new QLabel("Log for RB-Tree:");
    QLabel * namelabelDisp = new QLabel("Log for Dispatcher:");
    QLabel * namelabelBD = new QLabel("Log for Data Base:");
    logRBT = new QTextEdit;
    logDisp = new QTextEdit;
    logBD = new QTextEdit;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(namelabelRBT,0,0);
    mainLayout->addWidget(namelabelDisp,0,1);
    mainLayout->addWidget(namelabelBD,0,2);
    mainLayout->addWidget(logRBT,1,0);
    mainLayout->addWidget(logDisp,1,1);
    mainLayout->addWidget(logBD,1,2);
    setLayout(mainLayout);
    setWindowTitle("System of reservation");
    //integration::RealClientBD* tyui = (integration::RealClientBD)sourceRecources->getL(0).getNode();
    clientOfBD = clientBD;
    clientOfDisp = clientDisp;
    clientOfRBT = clientRBT;
    QObject::connect(clientOfDisp,SIGNAL(logStr(QString)),this,SLOT(addDisp(QString)));
    QObject::connect(clientOfBD,SIGNAL(logStr(QString)),this,SLOT(addBD(QString)));

    QObject::connect(clientOfRBT,SIGNAL(logStr(QString)),this,SLOT(addRBT(QString)));
}


MyWindow1::~MyWindow1(){
    delete logRBT;
    delete logDisp;
    delete logBD;
}

QTextEdit* MyWindow1::getContentRBT(){
    return logRBT;
}

QTextEdit* MyWindow1::getContentDisp(){
    return logDisp;
}

QTextEdit* MyWindow1::getContentBD(){
    return logBD;
}

void MyWindow1::addBD(QString qstr){
    logBD->moveCursor(QTextCursor::End);
    logBD->insertPlainText(qstr);
    logBD->moveCursor(QTextCursor::End);
    std::cout << qstr.toStdString() << std::endl;
}
void MyWindow1::addDisp(QString qstr){
    logDisp->moveCursor(QTextCursor::End);
    logDisp->insertPlainText(qstr);
    logDisp->moveCursor(QTextCursor::End);
    std::cout << qstr.toStdString() << std::endl;
}
void MyWindow1::addRBT(QString qstr){
    logRBT->moveCursor(QTextCursor::End);
    logRBT->insertPlainText(qstr);
    logRBT->moveCursor(QTextCursor::End);
    std::cout << qstr.toStdString() << std::endl;
}
