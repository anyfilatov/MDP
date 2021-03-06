#ifndef GUI
#define GUI
#include <QtGui>
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include "Dispatcher/Dispatcher.h"
#include "dbclient.h"
class NoSql : public QMainWindow
{
    Q_OBJECT

public:
    NoSql(Dispatcher* db);
    NoSql(DBClient* db);
private slots:
    void open();
    void save();
    void quit();
    void addColumn();
    void removeColumn();
    void loadDataFiles();
    void loadDataFromDb();
    void nextDataFromDb();
private:
    bool isServer;
    Dispatcher* db;
    DBClient* dbc;
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
    QLineEdit* userIdDb;
    QLineEdit* tableIdDb;
    QLineEdit* processIdDb;
    QPushButton* loadDataFromDbButton;
    QPushButton* nextDataFromDbButton;
    QTableWidget* tableWidgetDB;


    void layoutManagement(QTableWidget* table = NULL);
    void putDataInTable(MDPData* data);
    void initActions();
    void initSignals();
    void initFileMenuBar();
};
#endif // GUI

