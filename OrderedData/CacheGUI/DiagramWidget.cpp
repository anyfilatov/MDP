#include "DiagramWidget.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>

static const int INDENT = 150;
static const int Y_INDENT = 40;
static const int SHOW_PERCENT_VALUE = 10;

DiagramWidget::DiagramWidget(QWidget *parent) : QWidget(parent)
{
    redNodesPercent_ = 50;
    blackNodesPercent_ = 50;
}

DiagramWidget::~DiagramWidget()
{

}

void DiagramWidget::setRedNodesPercent(int redNodesPercent)
{
    if (redNodesPercent >= 0 && redNodesPercent <= 100) {
        this->redNodesPercent_ = redNodesPercent;
        this->blackNodesPercent_ = 100 - redNodesPercent;
        this->repaint();
    }
}

void DiagramWidget::setBlackNodesPercent(int blackNodesPercent)
{
    if (blackNodesPercent >= 0 && blackNodesPercent <= 100) {
        this->blackNodesPercent_ = blackNodesPercent;
        this->redNodesPercent_ = 100 - blackNodesPercent;
        this->repaint();
    }
}

int DiagramWidget::blackDiagramAngle()
{
    return (blackNodesPercent_ / 100.0) * 360.0;
}

int DiagramWidget::redStartDiagramAngle()
{
    return 90.0 + blackDiagramAngle();
}

int DiagramWidget::redDiagramAngle()
{
    return 360 - blackDiagramAngle();
}

QPoint DiagramWidget::blackCoordForPercent()
{
    double angle = 90 + blackDiagramAngle()/2;
    double x = 0;
    double y = 0;
    if (angle <= 180) {
        angle -= 90;
        x = -qSin(qDegreesToRadians(angle)) * INDENT - xIndentForBlackValue(angle);
        y = -qCos(qDegreesToRadians(angle)) * INDENT - yIndentForBlackValue(angle);
    } else if (angle > 180) {
        x =  qCos(qDegreesToRadians(angle)) * INDENT - xIndentForBlackValue(angle);
        y = -qSin(qDegreesToRadians(angle)) * INDENT - yIndentForBlackValue(angle);
    }
    return QPoint(x, y);
}

int DiagramWidget::xIndentForBlackValue(double angle)
{
    if (angle <= 180) {
        return - blackNodesPercent_ * 1.025 + 71.25;
    }
    return 20;
}

int DiagramWidget::yIndentForBlackValue(double angle)
{
    if (angle > 180) {
        return blackNodesPercent_ * 0.4 + 20;
    }
    return Y_INDENT;
}

QPoint DiagramWidget::redCoordForPercent()
{
    double angle = 90 + blackDiagramAngle() + redDiagramAngle()/2;
    double x = 0;
    double y = 0;
    if (angle < 360) {
        angle -= 270;
        x = qSin(qDegreesToRadians(angle)) * INDENT + xIndentForRedValue2(angle);
        y = qCos(qDegreesToRadians(angle)) * INDENT - Y_INDENT;
    } else {
        angle -= 360;
        x =  qCos(qDegreesToRadians(angle)) * INDENT + xIndentForRedValue(angle);
        y = -qSin(qDegreesToRadians(angle)) * INDENT - Y_INDENT;
    }

    qDebug() << angle << x << y;
    return QPoint(x, y);
}

int DiagramWidget::xIndentForRedValue(double angle)
{
    if (angle < 90) {
        return - redNodesPercent_ * 0.825 - 53.75;
    }
    return -95;
}

int DiagramWidget::xIndentForRedValue2(double angle) // EPIC NAMING MOTHERFUCKA
{
    if (angle < 90) {
        return redNodesPercent_ * 0.66 - 128;
    }
    return -95;
}

void DiagramWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setWindow(-250, -250, 500, 500);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor myRed(253, 78, 82);
    QColor myBlack(34, 46, 59);

    painter.setPen(QPen(Qt::NoPen));

    // Draw Black
    painter.setBrush(QBrush(myBlack, Qt::SolidPattern));
    painter.drawPie(-220, -220, 440, 440, 90*16, blackDiagramAngle() * 16);

    // Draw Red
    painter.setBrush(QBrush(myRed, Qt::SolidPattern));
    painter.drawPie(-220, -220, 440, 440, redStartDiagramAngle() * 16, redDiagramAngle() * 16);

    // Draw Text
    QFont font;
    font.setPointSize(50);
    painter.setFont(font);
    painter.setPen(QPen(Qt::white, 4, Qt::SolidLine, Qt::RoundCap));


    if (blackNodesPercent_ >= SHOW_PERCENT_VALUE) {
        QPoint blackPoint = blackCoordForPercent();
        painter.drawText(blackPoint.x(), blackPoint.y(),
                         130, 110,
                         Qt::AlignHCenter | Qt::AlignTop,
                         QString::number(blackNodesPercent_) + "%");
    }

    if (redNodesPercent_ >= SHOW_PERCENT_VALUE) {
        QPoint redPoint = redCoordForPercent();
        painter.drawText(redPoint.x(), redPoint.y(),
                         130, 110,
                         Qt::AlignHCenter | Qt::AlignTop,
                         QString::number(redNodesPercent_) + "%");
    }
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawEllipse(-5, -5, 10, 10);
}
