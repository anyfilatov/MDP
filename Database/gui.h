#ifndef GUI
#define GUI
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/QPushButton>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
class NoSql : public QWidget
{
    Q_OBJECT

public:
    NoSql();
private slots:
    void quit();
private:
//    QCoreApplication* qApp;
    QTextEdit *textEdit;
    QPushButton *quitButton;
};
#endif // GUI

