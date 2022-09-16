#include "mygraphicsview.h"
#include "myitem.h"
#include <QDebug>
#include <QWheelEvent>


MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent){
    angle=0;



    isResized=false;
    isLandscape=false;
    n=1;
    ppoint = QPointF(100,100);
    list.append(ppoint);
    scale=1;

    item = new MyItem();

    item->setFlag(QGraphicsItem::ItemIsMovable,false);

    auto scene = new QGraphicsScene(this);


 setScene(scene);
setSceneRect(0,0,1920,1080);

     points = new QGraphicsItemGroup();











    this->scene()->addItem(item);
    scene->addItem(points);

  QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(0,0,10,10,nullptr);

  circle->setBrush(QBrush(Qt::black, Qt::BDiagPattern));
 // QPen pen()
          circle->setPen(QPen(Qt::red, 3));

scene->addItem(circle);

 //points->addToGroup(circle);

}

void MyGraphicsView::zoomIn()
{

    if(n<30){
       n++;
    qDebug()<<"zoomIn";
   // QPointF point =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

  //  QPointF p =item->mapFromScene(point);

  //  item->setTransformOriginPoint(p);
    scaleView(qreal(1.05));
    }
}

void MyGraphicsView::zoomOut()
{
    if(n>1){
    n--;

    scaleView(1/qreal(1.05));

   //   QPointF p = QPointF(item->width/2,item->height/2);
  //    item->setTransformOriginPoint(p);

     qDebug()<<"zoomOut";

    }

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

update_meta();

   //  points->addToGroup(scene()->addEllipse(xx.x(),xx.y(),5,5));


  //


}

void MyGraphicsView::update_meta()
{
    deleteItemsFromGroup(points);

    foreach(QPointF point, list){

    QPointF xx = item->mapToScene(point);

    qDebug()<<"xx: "<<xx.x()<<" "<<xx.y();
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(xx.x(),xx.y(),2,2,nullptr);


            circle->setPen(QPen(Qt::green, 2));

    points->addToGroup(circle);
    }
}

void MyGraphicsView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    foreach( QGraphicsItem *item, scene()->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
}
}



void MyGraphicsView::load(QString filapath)
{
    deleteItemsFromGroup(points);
    item->load(filapath);
    setSceneRect(0,0,item->width,item->height);

}

void MyGraphicsView::update_ladmarks(QList<QPointF> source)
{
    list.clear();
    list.append(source);
    update_meta();
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
    update_meta();
    QGraphicsView::mouseMoveEvent(event);
}


