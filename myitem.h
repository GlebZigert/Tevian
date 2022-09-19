#ifndef MYITEM_H
#define MYITEM_H

#include <QPainter>
#include <QGraphicsItem>

class MyItem :public QGraphicsItem
{
public:
    MyItem();

    QRectF boundingRect() const;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    int width;
    int height;

    void load(QString filepath);
    void addPoint(QPointF);
private:

    QPixmap pxm;
    QList<QPointF> list;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYITEM_H
