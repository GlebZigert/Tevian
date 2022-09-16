#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
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

    qreal scale;
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
