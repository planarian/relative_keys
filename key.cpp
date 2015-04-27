#include "key.h"
#include <QString>
#include <QPainter>

Key::Key(const Switcher * swi, const CustomColor & a, const CustomColor & i, QWidget * parent) :
    QWidget(parent), toggles(swi), activeColor(a), inactiveColor(i)
{
    isPressed = 0;
    solValue = "";
    textColor = Qt::black;
}

QPoint Key::absPosition() const
{
    return mapToGlobal(QPoint(0,0));
}

void Key::checkIfVisible()
{
    if (visibleRegion().isEmpty())
        emit notVisibleSignal(this);
}

void Key::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush * mainBrush, * topBrush, *bottomBrush;
    QLinearGradient edgeGrad(QPointF(width(), 0), QPointF(0,0));

    if (isPressed)
    {
        //side gradients
        edgeGrad.setColorAt(0,activeColor.light);
        edgeGrad.setColorAt(0.15,activeColor.standard);
        edgeGrad.setColorAt(0.90,activeColor.standard);
        edgeGrad.setColorAt(1,activeColor.dark);
        mainBrush = new QBrush(edgeGrad);
        painter.fillRect(rect(), *mainBrush);

        //top gradient
        edgeGrad.setColorAt(0.15,activeColor.light);
        edgeGrad.setColorAt(0.90,activeColor.light);
        topBrush = new QBrush(edgeGrad);
        painter.fillRect(QRect(0,0,this->width(),1), *topBrush);

        //bottom gradient
        if (keyType == 1)
        {
            edgeGrad.setColorAt(0.15,activeColor.dark);
            edgeGrad.setColorAt(0.90,activeColor.dark);
            bottomBrush = new QBrush(edgeGrad);
            painter.fillRect(QRect(0,this->height()-1,this->width(),1), *bottomBrush);
        }

        if (solValue != "")
        {
            painter.setPen(Qt::white);
            painter.drawText(rect(),Qt::AlignHCenter | Qt::AlignVCenter,solValue);
        }
    }
    else
    {
        //side gradients
        edgeGrad.setColorAt(0,inactiveColor.light);
        edgeGrad.setColorAt(0.15,inactiveColor.standard);
        edgeGrad.setColorAt(0.90,inactiveColor.standard);
        edgeGrad.setColorAt(1,inactiveColor.dark);
        mainBrush = new QBrush(edgeGrad);
        painter.fillRect(rect(), *mainBrush);

        //top gradient
        edgeGrad.setColorAt(0.15,inactiveColor.light);
        edgeGrad.setColorAt(0.90,inactiveColor.light);
        topBrush = new QBrush(edgeGrad);
        painter.fillRect(QRect(0,0,this->width(),1), *topBrush);

        //bottom gradient
        if (keyType == 1)
        {
            edgeGrad.setColorAt(0.15,inactiveColor.dark);
            edgeGrad.setColorAt(0.90,inactiveColor.dark);
            bottomBrush = new QBrush(edgeGrad);
            painter.fillRect(QRect(0,this->height()-1,this->width(),1), *bottomBrush);
        }

        if (solValue != "")
        {
            painter.setPen(Qt::black);
            painter.drawText(rect(),Qt::AlignHCenter | Qt::AlignVCenter,solValue);
        }
    }

}
