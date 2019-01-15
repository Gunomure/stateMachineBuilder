#include "stateobj.h"
#include <QDebug>

StateObj::StateObj(QString name, QColor color, int borderWidth)
    : QObject()
{
    this->name = name;
    this->setObjectName(name);
    this->borderColor = color;
    this->borderWidth = borderWidth;
    this->isEndState = false;
    this->isStartState = false;
    this->relations = QVariantMap();
}

StateObj::~StateObj()
{

}

QRectF StateObj::boundingRect() const
{
    return this->rect;
//    qDebug() << pos();
//    return QRectF(pos(), size);
}

void StateObj::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen(QPen(borderColor, borderWidth));
//    painter->setBrush(Qt::lightGray);
    painter->setOpacity(0.8);
    painter->drawEllipse(boundingRect());
    painter->setPen(Qt::black);
    painter->setFont(QFont("", 90, 10));
    QString showingText = name;

    QString showingTimeOut = timeOut.isEmpty() ? "" : ("timeout = " + timeOut);
    painter->drawText(QRectF(boundingRect().center().x() - boundingRect().width()/2.5,
                             boundingRect().center().y() - boundingRect().height()/2.5,
                             boundingRect().width()*2,
                             boundingRect().height()), showingText);
    painter->drawText(QRectF(boundingRect().center().x() - boundingRect().width()/2.5,
                             boundingRect().center().y() + 2,
                             boundingRect().width()*2,
                             boundingRect().height()), showingTimeOut);
}

void StateObj::configure(StateKind state)
{
    switch (state) {
    case start:
        this->borderColor = Qt::blue;
        this->borderWidth = 6;
        this->isStartState = true;
        break;
   case end:
        this->borderColor = Qt::black;
        this->borderWidth = 6;
        this->isEndState = true;
        break;
    case ignoreFilter:
        if (!isEndState && !isStartState) {
            this->borderColor = Qt::red;
            this->borderWidth = 3;
        }
        break;
    case exceptionFilter:
        if (!isEndState && !isStartState) {
            this->borderColor = Qt::red;
            this->borderWidth = 6;
        }
        break;
    default:
        this->borderColor = Qt::red;
        this->borderWidth = 3;
        break;
    }
}

void StateObj::setName(QString name)
{
    this->name = name;
    setObjectName(name);
}

QList<QPointF> StateObj::getCoordsSides()
{
    QList<QPointF> res;
    res.append(QPointF(this->boundingRect().center().x(),
                       this->boundingRect().center().y() - this->boundingRect().width()/2));//левая сторона
    res.append(QPointF(this->boundingRect().center().x(),
                       this->boundingRect().topLeft().y()));//верхняя сторона
    res.append(QPointF(this->boundingRect().center().x(),
                       this->boundingRect().center().y() + this->boundingRect().width()/2));//правая сторона
    res.append(QPointF(this->boundingRect().center().x(),
                       this->boundingRect().bottomRight().y()));//нижняя сторона
    return res;
}
