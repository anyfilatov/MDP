#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CacheInfoService.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showCacheInfo(int index);
    void updateData();
    void debugMode();
    void debugModeOff();
    void autoupdateOn();
    void autoupdateOff();

private:
    Ui::MainWindow *ui;
    CacheInfoService *service;
    QTimer *updateTimer;
};

#endif // MAINWINDOW_H
