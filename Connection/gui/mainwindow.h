#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "requester.h"
#include <functional>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    typedef std::function<void(const QString&, int)> CallBack;
    explicit MainWindow(AsyncRequester<CallBack>& requester, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendResult_clicked();
    void resultRecived(const QString& text, int code);
    void on_getResult_clicked();

private:
    AsyncRequester<CallBack>& requester_;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
