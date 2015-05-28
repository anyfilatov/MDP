#ifndef CACHEINFOWIDGET_H
#define CACHEINFOWIDGET_H

#include <QWidget>
#include <CacheInfoService.h>

namespace Ui {
class CacheInfoWidget;
}

class CacheInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CacheInfoWidget(QWidget *parent = 0);
    ~CacheInfoWidget();

    void setCacheInfo(CacheInfo info);

private:
    Ui::CacheInfoWidget *ui;

    QString toRichText(QString);
};

#endif // CACHEINFOWIDGET_H
