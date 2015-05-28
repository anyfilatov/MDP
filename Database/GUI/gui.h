#ifndef GUI
#define GUI
#include <QtGui>
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include "Dispatcher/Dispatcher.h"
class NoSql : public QMainWindow
{
    Q_OBJECT

public:
    NoSql(Dispatcher* db);
private slots:
    void open();
    void save();
    void quit();
    void addColumn();
    void removeColumn();
    void loadDataFiles();
private:
    Dispatcher* db;
    QTextEdit *textEdit;
    QPushButton *quitButton;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction* loadDataFilesAction;
    QMenu *fileMenu;
    QTableWidget * tableWidget;
    QPushButton *addColumnButton;
    QPushButton *removeColumnButton;
    QLineEdit *userIdField ;
    QLineEdit *tableIdField;


    void layoutManagement(QTableWidget* table = NULL);
};
#endif // GUI

