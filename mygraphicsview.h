#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include "myitem.h"
class MyGraphicsView :public QGraphicsView
{
    Q_OBJECT

private:
    MyItem* item;
    int angle;

    void zoomIn();
    void zoomOut();
    void scaleView(qreal scaleFactor);
    bool isResized;
    bool isLandscape;

    QPointF ppoint;

    qreal scale;

    QGraphicsItemGroup  *points;   // Объявляем первую группу элементов


    void deleteItemsFromGroup(QGraphicsItemGroup *group);

public:
    MyGraphicsView(QWidget *parent = nullptr);

    void load(QString filapath);

signals:


protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MYGRAPHICSVIEW_H
