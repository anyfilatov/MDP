#pragma once
#include <QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QGridLayout>
#include <QCoreApplication>
#include <QString>
#include "dispetcher.h"
#include "realclients.h"

class MyWindow1 : public QWidget
{
    Q_OBJECT
private:
    integration::RealClientBD* clientOfBD;
    integration::RealClientDisp* clientOfDisp;
    integration::RealClientRBT* clientOfRBT;
    QTextEdit* logBD;
    QTextEdit* logDisp;
    QTextEdit* logRBT;

public:

    MyWindow1(integration::RealClientBD *, integration::RealClientDisp*, integration::RealClientRBT*);
    ~MyWindow1();
    QTextEdit* getContentRBT();
    QTextEdit* getContentDisp();
    QTextEdit* getContentBD();
public slots:
    void addBD(QString);
    void addDisp(QString);
    void addRBT(QString);
};
