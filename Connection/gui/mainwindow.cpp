#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "result.h"
#include "dialog.h"
#include "codeeditor.h"
#include <QMovie>
#include <QDebug>
#include <unistd.h>
#include "requester.h"
#include <functional>
#include <QFileDialog>

MainWindow::MainWindow(AsyncRequester<CallBack>& requester, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    requester_(requester)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie("/home/savinov/edu/mdp/mdp/Connection/gui/loading.gif");

//    ui->loadLable->setText("aaaa");
    ui->loadLable->setMovie(movie);
    ui->loadLable->hide();
    movie->start();
    ui->textLoadingLabel->setText( "Please wait. Loading...");
    ui->textLoadingLabel->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resultRecived(const QString& text, int code){
    ui->loadLable->hide();
    LOG_INFO(text.toStdString());
    ui->textLoadingLabel->setText("Answer: " + QString::number(code) + " " + text );
}

void MainWindow::on_sendResult_clicked()
{
    ui->textLoadingLabel->setText( "Please wait. Loading...");
    CallBack callBack = std::bind(&MainWindow::resultRecived, this, std::placeholders::_1, std::placeholders::_2);
    requester_.request(ui->plainTextEdit->toPlainText(), 1, callBack);
    ui->loadLable->show();
    ui->textLoadingLabel->show();
    QString m = "Скрипт загружен успешно!";
    Result *d = new Result;
    d->setMessage(m);
    d->show();
}

void MainWindow::on_getResult_clicked()
{
    QFileDialog dialog(this);
    dialog.show();
    dialog.setDefaultSuffix("txt");
//    dialog.setFilter(QDir::("*.lua"));
    dialog.exec();
    auto list = dialog.selectedFiles();
    if(list.empty()){
        Result r;
        r.setMessage("File not choosed");
        r.show();
    } else {
        QString fileName = list.at(0);
        QFile file(fileName);
        file.open(QIODevice::ReadWrite);
        auto ba = file.readAll();
        QString str(ba);
        ui->plainTextEdit->clear();
        ui->plainTextEdit->appendPlainText(str);
    }
}
