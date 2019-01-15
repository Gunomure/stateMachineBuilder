#include "arrow.h"

#define ARROWWIDTH 60
#define ARROWHEIGHT 100
#define WIDTHSHAFT 30

Arrow::Arrow(QPointF start, QPointF end)
{
    this->start = start;
    this->end = end;
    this->relationName = "";

    double widthLine = fabs(start.x() - end.x());
    double height = fabs(start.y() - end.y());
    double x = qMin(end.x(), start.x());
    double y = qMin(end.y(), start.y());
    this->ownGeometry = QRectF(x, y, widthLine, height);
}

QRectF Arrow::boundingRect() const
{
    return ownGeometry;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    //TODO прибраться, вынести инициализации переменных в функцию
    QPolygon arrow;
    arrow << QPoint(0, -ARROWWIDTH/2)
          << QPoint(ARROWHEIGHT, 0)
          << QPoint(0, ARROWWIDTH/2);

    QPointF p0(start);
    QPointF p1(end);
    double dx = p1.x() - p0.x();
    double dy = p1.y() - p0.y();
    double angle = atan2(dy, dx);
    QPointF p2(p1.x() - ARROWHEIGHT * cos(angle),
               p1.y() - ARROWHEIGHT * sin(angle));

    painter->setPen(QPen(Qt::gray, WIDTHSHAFT));
    painter->setOpacity(0.6);
    painter->drawLine(p0, p2);
    painter->save();
    painter->translate(p2);
    painter->rotate(180/3.14159 * angle);
    painter->drawConvexPolygon(arrow);
    painter->restore();

    painter->setOpacity(1);
    painter->setPen(QPen(Qt::black));
    QFont font;
    font.setPointSize(90);

    painter->setFont(font);
    painter->drawText(QPointF(ownGeometry.center().x(), ownGeometry.center().y()), relationName);
    int i = 1;
    for (QVariant text : raises) {
        painter->drawText(QPointF(ownGeometry.center().x(), ownGeometry.center().y() + 90*i),
                          QString("-%1").arg(text.toString()));
        i += 1.5;
    }
}
