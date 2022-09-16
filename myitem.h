// myitem.h

#ifndef MYITEM_H
#define MYITEM_H

#include <QPainter>
#include <QGraphicsItem>

// class for customization
class MyItem :public QGraphicsItem
{
public:
    MyItem();

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // item state


    int width;
    int height;

    void load(QString filepath);
    void addPoint(QPointF);
private:

    QPixmap pxm;
    QList<QPointF> list;

protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYITEM_H
