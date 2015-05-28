#ifndef TABSWIDGET_H
#define TABSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include <QPushButton>
#include <QSignalMapper>

class TabsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabsWidget(QWidget *parent = 0);
    ~TabsWidget();

    void setTabs(int tabsCount);

signals:
    void buttonClicked(int);

private:
    QSignalMapper mapper_;
    QVBoxLayout *layout_;
    QList<QPushButton *> buttons_;

    void deleteButtons();
};

#endif // TABSWIDGET_H
