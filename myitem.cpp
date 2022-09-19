// myitem.cpp

#include "myitem.h"
#include <QImageReader>

MyItem::MyItem()
{
    this->width=pxm.width();
    this->height=pxm.height();
    setFlag(ItemIsMovable);
}

QRectF MyItem::boundingRect() const
{
    return QRectF(0,0,this->width, this->height);

}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rect = boundingRect();
    painter->drawPixmap(0,0,this->width,this->height,pxm);
    painter->drawRect(0,0,this->width,this->height);
    foreach(QPointF point, list){
        painter->drawEllipse(point.x(),
                             point.y(),
                             1,
                             1);
    }

}

void MyItem::load(QString qStrFilePath)
{

    if (qStrFilePath.isEmpty())
        return;

    QImageReader reader(qStrFilePath);
    if (!reader.canRead())
        return;

    QImage qimg = reader.read();
    pxm = QPixmap::fromImage(qimg);
    this->width=pxm.width();
    this->height=pxm.height();
    update();
}

void MyItem::addPoint(QPointF point)
{
    list.append(point);
}



void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    update();
    QGraphicsItem::mousePressEvent(event);
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
