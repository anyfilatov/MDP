#ifndef GUI
#define GUI
#include <QtGui>
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>

class NoSql : public QMainWindow
{
    Q_OBJECT

public:
    NoSql();
private slots:
    void open();
    void save();
    void quit();
    void addColumn();
    void removeColumn();
private:

    QTextEdit *textEdit;
    QPushButton *quitButton;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QMenu *fileMenu;
    QTableWidget * tableWidget;
    QPushButton *addColumnButton;
    QPushButton *removeColumnButton;
};
#endif // GUI

