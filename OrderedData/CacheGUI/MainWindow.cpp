#include "MainWindow.h"
#include "ui_MainWindow.h"

static const int UPDATE_TIME_SEC = 5;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    service = new CacheInfoService;
    updateTimer = new QTimer;
    updateData();
    autoupdateOn();

    ui->tabsWidget->setTabs(service->nodes().size());
    connect(ui->tabsWidget, SIGNAL(buttonClicked(int)),
            this, SLOT(showCacheInfo(int)));
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));

    connect(ui->actionUpdate, SIGNAL(triggered()),
            this, SLOT(updateData()));
    connect(ui->actionDebug_mode, SIGNAL(triggered()),
            this, SLOT(debugMode()));
    connect(ui->actionDebug_mode_off, SIGNAL(triggered()),
            this, SLOT(debugModeOff()));
    connect(ui->actionAutoupdate_on, SIGNAL(triggered()),
            this, SLOT(autoupdateOn()));
    connect(ui->actionAutoupdate_off, SIGNAL(triggered()),
            this, SLOT(autoupdateOff()));

    // Hide debug view
    ui->cacheInfoWidget->showSlider(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete service;
    delete updateTimer;
}

void MainWindow::showCacheInfo(int index)
{
    ui->cacheInfoWidget->setCacheInfo(service->nodes()[index]);
}

void MainWindow::updateData()
{
    service->updateData();
    showCacheInfo(0);
    qDebug() << "updated";
}

void MainWindow::debugMode()
{
    ui->cacheInfoWidget->showSlider(true);
}

void MainWindow::debugModeOff()
{
    ui->cacheInfoWidget->showSlider(false);
}

void MainWindow::autoupdateOn()
{
    updateTimer->start(UPDATE_TIME_SEC*1000);
}

void MainWindow::autoupdateOff()
{
    updateTimer->stop();
}

