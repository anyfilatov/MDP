#ifndef DIAGRAMWIDGET_H
#define DIAGRAMWIDGET_H

#include <QWidget>

class DiagramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DiagramWidget(QWidget *parent = 0);
    ~DiagramWidget();

    int redNodesPercent() const { return redNodesPercent_; }

protected:
    void paintEvent(QPaintEvent *);

public slots:
    void setRedNodesPercent(int redNodesPercent);
    void setBlackNodesPercent(int blackNodesPercent);

private:
    int redNodesPercent_;
    int blackNodesPercent_;

    int blackDiagramAngle();
    int redStartDiagramAngle();
    int redDiagramAngle();

    QPoint blackCoordForPercent();
    int xIndentForBlackValue(double angle);
    int yIndentForBlackValue(double angle);

    QPoint redCoordForPercent();
    int xIndentForRedValue(double angle);
    int xIndentForRedValue2(double angle);
};

#endif // DIAGRAMWIDGET_H
