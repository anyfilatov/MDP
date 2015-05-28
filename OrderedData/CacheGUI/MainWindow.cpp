#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    service = new CacheInfoService;
    updateData();

    ui->tabsWidget->setTabs(service->nodes().size());
    connect(ui->tabsWidget, SIGNAL(buttonClicked(int)),
            this, SLOT(showCacheInfo(int)));

    connect(ui->actionUpdate, SIGNAL(triggered()),
            this, SLOT(updateData()));
    connect(ui->actionDebug_mode, SIGNAL(triggered()),
            this, SLOT(debugMode()));
    connect(ui->actionDebug_mode_off, SIGNAL(triggered()),
            this, SLOT(debugModeOff()));

    // Hide debug view
    ui->cacheInfoWidget->showSlider(false);
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

void MainWindow::updateData()
{
    service->updateData();
    showCacheInfo(0);
}

void MainWindow::debugMode()
{
    ui->cacheInfoWidget->showSlider(true);
}

void MainWindow::debugModeOff()
{
    ui->cacheInfoWidget->showSlider(false);
}

