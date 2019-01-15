#ifndef STATEOBJ_H
#define STATEOBJ_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

#include "enums.h"

class StateObj : public QObject, public QGraphicsItem
{
    Q_OBJECT
    //чтобы был возможен qobject_cast к QGraphicsItem
    Q_INTERFACES(QGraphicsItem)
public:
    StateObj(QString name = "", QColor color = Qt::red, int borderWidth = 2);
    ~StateObj();

    void setName(QString name);
    QString getName() { return this->name; }

    void setTimeOut(QString time) { this->timeOut = time; }
    void setColor(QColor color) { this->borderColor = color; }
    void setWidth(int lineWidth) { this->borderWidth = lineWidth; }
    void setSize(QSize size) { this->size = size; }
    void setRect(QRectF rect) { this->rect = rect; }

    void addRelation(QString condition, QString stateName)
    {
        this->relations.insert(condition, stateName);
    }

    QList<QPointF> getCoordsSides();

    QVariantMap getRelations() { return relations;}

    void addRaise(QString condition, QString raiseName) {
        QVariantList tempList;
        if (raises.contains(condition)) {
            tempList = raises.value(condition).toList();
        }
        tempList.append(raiseName);
        raises.insert(condition, tempList);
    }

    QVariantMap getRaises() { return this->raises; }

    void configure(StateKind state);

    QRectF boundingRect() const Q_DECL_OVERRIDE;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

private:
    QString name;
    QString timeOut;
    QColor borderColor;
    int borderWidth;
    QSize size;
    QRectF rect;
    QVariantMap relations;
    QVariantMap raises;
    bool isEndState;
    bool isStartState;
};

#endif // STATEOBJ_H
