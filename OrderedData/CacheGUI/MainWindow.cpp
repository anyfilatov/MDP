#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    service = new CacheInfoService;

//    ui
    ui->tabsWidget->setTabs(service->nodes().size());
    connect(ui->tabsWidget, SIGNAL(buttonClicked(int)),
            this, SLOT(showCacheInfo(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete service;
}

void MainWindow::showCacheInfo(int index)
{
    ui->cacheInfoWidget->setCacheInfo(service->nodes()[index]);
}
