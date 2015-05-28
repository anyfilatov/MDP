#include "CacheInfoWidget.h"
#include "ui_CacheInfoWidget.h"

CacheInfoWidget::CacheInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CacheInfoWidget)
{
    ui->setupUi(this);
    connect(ui->slider, SIGNAL(valueChanged(int)), ui->diagramWidget, SLOT(setBlackNodesPercent(int)));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    // Hide unused views
    ui->requestsPerMin->hide();
    ui->workTime->hide();
    ui->label->hide();
    ui->label_3->hide();
}

CacheInfoWidget::~CacheInfoWidget()
{
    delete ui;
}

void CacheInfoWidget::setCacheInfo(CacheInfo info)
{
    ui->ip->setText(toRichText("IP " + info.ip()));
    ui->workTime->setText(toRichText(info.workTime()));
    ui->keysCount->setText(toRichText(info.keysCount()));
    ui->requestsPerMin->setText(toRichText(info.requestsPerMinute()));
    ui->keysPerReplics->setText(toRichText(info.keysPerReplics()));
    ui->hits->setText(toRichText(info.hits()));
    ui->treeHeight->setText(toRichText(info.treeHeight()));
    ui->diagramWidget->setBlackNodesPercent(info.blackNodesPercent());
}

QString CacheInfoWidget::toRichText(QString str)
{
    return "<html><head/><body><p><span style=\" font-size:24pt;\">" + str + "</span></p></body></html>";
}

void CacheInfoWidget::showSlider(bool b)
{
    if (b) {
        ui->slider->show();
    } else {
        ui->slider->hide();
    }
}
