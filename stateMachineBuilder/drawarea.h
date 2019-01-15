#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "stateobj.h"
#include "arrow.h"

class DrawArea : public QGraphicsView
{
    Q_OBJECT
public:
    DrawArea(QWidget *parent);

    void addItem(QGraphicsItem* item);
    void updateScene();
    void setStates(QMap<QString, StateObj *> list);
    void clearArea();

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void drawStates();

private:
    QPointF getRandCoord();
    int getRand(int min, int max);
    void makeWithoutColliding();
    bool checkColliding(StateObj* textItem);
    void drawArrows();
    QPair<QPointF, QPointF> getMinDistPoints(StateObj *start, StateObj *end);
    bool checkOutArea(QPointF point);
    void deleteItem(QGraphicsItem* item);

private:
    QMap<QString, StateObj*> stateObjects;
    QMap<QString, Arrow*> arrows; //чтоб потом удалить со сцены
};

#endif // DRAWAREA_H
