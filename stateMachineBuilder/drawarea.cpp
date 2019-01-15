#include "drawarea.h"
#include <QWheelEvent>
#include <math.h>
#include <QDebug>

#define WIDTHELLIPSE 400
#define HEIGHTELLIPSE 300
#define SCENEWIDTH 5000
#define SCENEHEIGHT 5000

DrawArea::DrawArea(QWidget *parent)
{
    this->setScene(new QGraphicsScene(this));
    this->scene()->setSceneRect(QRectF(QPointF(0,0), QSize(SCENEWIDTH, SCENEHEIGHT)));

    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    setViewportUpdateMode(MinimalViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setOptimizationFlag(QGraphicsView::DontClipPainter);
    this->setTransformationAnchor(AnchorUnderMouse); //точно оставить,скроллит в направлении мышки
}

void DrawArea::wheelEvent(QWheelEvent *event)
{
    double scale = pow((double)2, event->delta() / 2400.0);
    this->scale(scale, scale);
}

void DrawArea::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    this->fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);
}

void DrawArea::addItem(QGraphicsItem* item)
{
    this->scene()->addItem(item);
    updateScene();
}

void DrawArea::updateScene()
{
    this->scene()->update();
}

void DrawArea::setStates(QMap<QString, StateObj*> list)
{
    this->stateObjects = list;
    for (StateObj* state : list) {
        addItem(state);
    }
    drawStates();
    drawArrows();
    updateScene();
}

void DrawArea::drawStates()
{
    for (StateObj* state : stateObjects) {
        state->setRect(QRectF(
                           QPointF(getRandCoord()),
                           QSize(WIDTHELLIPSE,HEIGHTELLIPSE)));
    }
    makeWithoutColliding();
}

void DrawArea::drawArrows()
{
    for (StateObj* obj : stateObjects) {
        for (QString condition : obj->getRelations().keys()) {
            QString stateName = obj->getRelations().value(condition).toString();
            if (stateObjects.contains(stateName)) {
                QPair<QPointF, QPointF> coords = getMinDistPoints(obj, stateObjects.value(stateName));
                Arrow* arrow = new Arrow(coords.first,
                                         coords.second);
                arrow->setRelationName(condition);
                arrows.insert(obj->getName() + condition, arrow);
                addItem(arrow);
            }
        }
    }
    for (StateObj* obj : stateObjects) {
        QVariantMap raises = obj->getRaises();
        for (QString condition : raises.keys()) {
            QString key = obj->getName() + condition;
            if (arrows.contains(key)) {
                QVariantList tempList = raises.value(condition).toList();
                arrows[key]->setRaises(tempList);
            }
        }
    }
    updateScene();
}

QPair<QPointF, QPointF> DrawArea::getMinDistPoints(StateObj* start, StateObj* end)
{
    QList<QPointF> first = start->getCoordsSides();
    QList<QPointF> second = end->getCoordsSides();
    QPair<QPointF, QPointF> res;
    auto tempDiff = first.first() - second.first();
    double minLen = hypot(tempDiff.rx(), tempDiff.ry());
    for (QPointF firstPos : first) {
        for (QPointF secondPos : second) {
            auto diff = firstPos - secondPos;
            double lenght = hypot(diff.rx(), diff.ry());
            if (lenght < minLen)
                res = QPair<QPointF, QPointF>(firstPos, secondPos);
        }
    }
    return res;
}

QPointF DrawArea::getRandCoord()
{
    int width = this->scene()->sceneRect().width();
    int height = this->scene()->sceneRect().height();
    return QPointF(getRand(0, width), getRand(0, height));
}

int DrawArea::getRand(int min, int max)
{
    return rand() % max + min;
}

void DrawArea::makeWithoutColliding()
{
    //сдвигаем текст сначала вверх,если вверху все еще тексты,то сдвигаем вниз,пока не будет свободно
    int n = 0;

    QList<QGraphicsItem*> items = this->scene()->items();
    for(QGraphicsItem* item : items)
    {
        StateObj* stateObj = dynamic_cast<StateObj*>(item);
        if(stateObj && stateObj->objectName() != "")
        {
            while(n < 3 && checkColliding(stateObj))
            {
                int multiplierX = getRand(-1, 1);
                int multiplierY = getRand(-1, 1);
                QPointF newCoord(stateObj->pos().x() + 2*multiplierX*stateObj->boundingRect().width(),
                                 stateObj->boundingRect().y() + 2*multiplierY*stateObj->boundingRect().height());
                if (!checkOutArea(newCoord)) {
                    stateObj->setRect(QRectF(newCoord,
                                             QSize(WIDTHELLIPSE,HEIGHTELLIPSE)));
                    stateObj->setPos(newCoord);
                }
                ++n;
            }
        }
    }

    this->scene()->update();
}

bool DrawArea::checkColliding(StateObj *textItem)
{
    QList<QGraphicsItem*> list = textItem->collidingItems(Qt::IntersectsItemBoundingRect);
    for(QGraphicsItem* grapghicsItem : list)
    {
        StateObj* item = dynamic_cast<StateObj*>(grapghicsItem);
        if(item)
        {
            QString objName = item->objectName();
            if(objName != "")
            {
                return true;
            }
        }
    }
    return false;
}

bool DrawArea::checkOutArea(QPointF point)
{
    return (point.x() < 0 || point.x() > this->scene()->width())
            || (point.y() < 0 || point.y() > this->scene()->height());
}

void DrawArea::clearArea()
{
    for (StateObj* state : stateObjects) {
        deleteItem(state);
        state->deleteLater();
    }
    for (Arrow* arrow : arrows) {
        deleteItem(arrow);
        arrow->deleteLater();
    }
    stateObjects.clear();
    arrows.clear();
    updateScene();
}

void DrawArea::deleteItem(QGraphicsItem* item)
{
    if(this->scene())
    {
        this->scene()->removeItem(item);
        this->scene()->update();
    }
}
