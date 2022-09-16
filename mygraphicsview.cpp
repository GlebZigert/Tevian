#include "mygraphicsview.h"
#include "myitem.h"
#include <QDebug>
#include <QWheelEvent>


MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent){
    angle=0;
     isResized=false;
     isLandscape=false;

    scale=1;

     item = new MyItem();

    auto scene = new QGraphicsScene(this);

        setScene(scene);
        setSceneRect(0,0,item->width,item->height);
        this->scene()->addItem(item);

        scene->addEllipse(-1,-1,1,1);

}

void MyGraphicsView::zoomIn()
{
    qDebug()<<"zoomIn";
   // QPointF point =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

  //  QPointF p =item->mapFromScene(point);

  //  item->setTransformOriginPoint(p);
    scaleView(qreal(1.05));
}

void MyGraphicsView::zoomOut()
{
    scaleView(1/qreal(1.05));

   //   QPointF p = QPointF(item->width/2,item->height/2);
  //    item->setTransformOriginPoint(p);
     qDebug()<<"zoomOut";
}

void MyGraphicsView::scaleView(qreal scaleFactor)
{
    if(sceneRect().isEmpty())
        return;

    QRectF expectedRect = transform().scale(scaleFactor, scaleFactor).mapRect(sceneRect());
    qreal expRectLength;
    int viewportLength;
    int imgLength;

    /*
    if (isLandscape)
    {
        expRectLength = expectedRect.width();
        viewportLength = viewport()->rect().width();
        imgLength = sceneRect().width();
    }
    else
    {
        expRectLength = expectedRect.height();
        viewportLength = viewport()->rect().height();
        imgLength = sceneRect().height();
    }

    if (expRectLength < viewportLength / 2) // minimum zoom : half of viewport
    {
     //   if (!isResized || scaleFactor < 1)
      //      return;
    }
    else if (expRectLength > imgLength * 30) // maximum zoom : x10
    {
     //   if (!isResized || scaleFactor > 1)
     //       return;
    }
    else
    {
        isResized = false;
    }
    */
    qDebug()<<".";
    scale*=scaleFactor;
  //  item->setTransformOriginPoint( 0, 0);
  //  item->setTransformOriginPoint( QCursor::pos().x(), QCursor::pos().y());
  //  item->setTransformOriginPoint(QCursor::pos().x(),QCursor::pos().y());
     //  qDebug()<<QCursor::pos().x()<<" "<<QCursor::pos().y()<<" "<<point.x()<<" "<<point.y();
     QPointF point =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

     QPointF p =item->mapFromScene(point);



     item->setTransformOriginPoint(p);
  //   item->addPoint(p);

    item->setScale(scale);

     QPointF point2 = item->mapToScene(p);

     item->moveBy((point.x()-point2.x()),(point.y()-point2.y()));

     qDebug()<<": "<<point.x()-point2.x()<<" "<<point.y()-point2.y();
}



void MyGraphicsView::load(QString filapath)
{
    item->load(filapath);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{

    QPointF point =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

    QPointF p =item->mapFromScene(point);

    qDebug()<<"-- "<<p.x()<<" "<<p.y();

//    qDebug()<<item->x()<<" "<<item->y();
  //   qDebug()<<p1.x()<<" "<<p1.y();
  //  qDebug()<<QCursor::pos().x()<<" "<<QCursor::pos().y()<<" "<<point.x()<<" "<<point.y()<<" "<<item->pos().x()<<" "item->pos().y();
 //   qDebug()<<QCursor::pos().x()<<" "<<QCursor::pos().y()<<" "<<item->pos().x()<<" "<<item->pos().y()<<" "<<point.x() <<" "<<point.y();



  //  item->setTransformOriginPoint(point);
    /*
    qDebug()<<".";
    angle+=5
    item->setRotation(angle);
    */


       if (event->modifiers() == Qt::ControlModifier)
       {
           if (event->delta() > 0) zoomIn();
           else zoomOut();
       }
       else if (event->modifiers() == Qt::ShiftModifier)
       {
           QWheelEvent fakeEvent(event->pos(), event->delta(), event->buttons(), Qt::NoModifier, Qt::Horizontal);
           QGraphicsView::wheelEvent(&fakeEvent);
       }
       else if (event->modifiers() == Qt::NoModifier)
       {
           QGraphicsView::wheelEvent(event);
       }
}

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{

}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
 //   QPointF point = item->mapToScene(0,0);
//  //  qDebug()<<QCursor::pos().x()<<" "<<QCursor::pos().y()<<" "<<point.x()<<" "<<point.y()<<" "<<item->pos().x()<<" "item->pos().y();
//    qDebug()<<item->pos().x()<<" "<<item->pos().y()<<" "<<point.x() <<" "<<point.y();
    QGraphicsView::mouseMoveEvent(event);
}


