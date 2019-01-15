#ifndef ARROW_H
#define ARROW_H

#include <QPointF>
#include <QPainter>
#include <QGraphicsItem>
#include <math.h>

class Arrow : public QObject, public QGraphicsItem
{
    Q_OBJECT
    //чтобы был возможен qobject_cast к QGraphicsItem
    Q_INTERFACES(QGraphicsItem)
public:
    Arrow(QPointF start, QPointF end);

    void setRelationName(QString text) { this->relationName = text;}
    void setRaises(QVariantList raisesInpt) { this->raises.append(raisesInpt); }
    void addRaise(QString raise) { this->raises.append(raise); }

    QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    QPointF start, end;
    QVariantList raises;
    QString relationName;
    QRectF ownGeometry;
};

#endif // ARROW_H
