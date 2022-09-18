#include "myview.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QTime>

MyView::MyView(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    setScene(scene);

    scene->addEllipse(0,0,1,1);

    points = new QGraphicsItemGroup();






    rect_w=1920;
    rect_h=1080;

    n=1;
    scene->setSceneRect(0,0,1920,1080);





    scale=1;
    flag = false;
QPen pen(Qt::blue);
    area = new QGraphicsRectItem(0,0,scene->width()/2,scene->height()/2);
    area->setFlag(QGraphicsItem::ItemIsMovable,false);
    area->setPen(pen);
    scene->addItem(area);
   QPen penRed(Qt::red);
    rect = new MyItem();


    rect->setFlag(QGraphicsItem::ItemIsMovable,true);
    scene->addItem(rect);

    qDebug()<<"1: "<<rect->x()<<" "<<rect->y();
    rect->moveBy(0,0);
    qDebug()<<"2: "<<rect->x()<<" "<<rect->y();

     scene->addItem(points);
}

void MyView::zoomIn()
{
    if(n<50){

    qDebug()<<"in";
    if(scaleView(qreal(1.05)))
        n++;
    }
}

void MyView::zoomOut()
{
    if(n>2){
    qDebug()<<"out";
    if(scaleView(1/qreal(1.05)))
        n--;
    }else{

        //взять 0 0 ректа и совметить с базой


    }
}

void MyView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    foreach( QGraphicsItem *item, scene->items(group->boundingRect())) {
       if(item->group() == group ) {
          delete item;
       }
}
}

void MyView::update_meta()
{
    qDebug()<<"update_meta";

    if(meta.isNull())
        return;

    deleteItemsFromGroup(points);

    foreach(QPointF point, meta->landmarks){

    QPointF xx = rect->mapToScene(point);

//    qDebug()<<"xx: "<<xx.x()<<" "<<xx.y();
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(xx.x(),xx.y(),2,2,nullptr);


            circle->setPen(QPen(Qt::green, 2));

    points->addToGroup(circle);
    }

    QGraphicsRectItem *bbox = new QGraphicsRectItem(rect->mapRectToScene(meta->bbox));
        bbox->setPen(QPen(Qt::red, 2));
    points->addToGroup(bbox);


}

bool MyView::scaleView(qreal scaleFactor)
{
    QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
  //  scene->addRect(f.x(),f.y(),1,1);


    //Точка масштабирования


    //Точка масштабирования в пиксмапе

    QPointF f1 = rect->mapFromScene(f);
    //Расстояние от точки до левого края пиксмапа

    //точка пихмапа 0 0 на сцене
    QPointF r00 = rect->mapToScene(0,0);
    QPointF rwh = rect->mapToScene(rect_w,rect_h);

    int a1=f.x()-r00.x();
    int a2=f.y()-r00.y();
    int a3=rwh.x()-f.x();
    int a4=rwh.y()-f.y();


    QPointF a00 = area->mapToScene(0,0);
    QPointF awh = area->mapToScene(area->rect().width(),area->rect().height());



    int b1=f.x()-a00.x();
    int b2=f.y()-a00.y();
    int b3=awh.x()-b1;
    int b4=awh.y()-b2;

    a1*=scaleFactor;
    a2*=scaleFactor;
    a3*=scaleFactor;;
    a4*=scaleFactor;
/*
    qDebug()<<a1<<" "
<<a2<<" "
<<a3<<" "
<<a4<<" "
<<b1<<" "
<<b2<<" "
<<b3<<" "
<<b4<<" ";
    */

    if((a1>b1)
            &&(a2>b2)
            &&(a3>b3)
            &&(a4>b4)){

        qDebug()<<"true";
    }
    else{

bool res=false;

        if((a1<b1)
                &&(a2>=b2)
                &&(a3>=b3)
                &&(a4>=b4)){

            f.setX(a00.x());
            res=true;
        }

        if((a1<b1)
                &&(a2<b2)
                &&(a3>=b3)
                &&(a4>=b4)){

            f.setX(a00.x());
            f.setY(a00.y());
            res=true;
        }

        if((a1<b1)
                &&(a2>=b2)
                &&(a3>=b3)
                &&(a4<b4)){

            f.setX(a00.x());
            f.setY(awh.y());
            res=true;
        }

//=======================================
        if((a1>=b1)
                &&(a2<b2)
                &&(a3>=b3)
                &&(a4>=b4)){

            f.setY(a00.y());
            res=true;
        }



        if((a1>=b1)
                &&(a2<b2)
                &&(a3<b3)
                &&(a4>=b4)){

            f.setX(awh.x());
            f.setY(a00.y());
            res=true;
        }

 //=====================================

        if((a1>=b1)
                &&(a2>=b2)
                &&(a3<b3)
                &&(a4>=b4)){

            f.setX(awh.x());
            res=true;
        }



        if((a1>=b1)
                &&(a2>=b2)
                &&(a3<b3)
                &&(a4<b4)){

            f.setX(awh.x());
            f.setY(awh.y());
            res=true;
        }


        if((a1>=b1)
                &&(a2>=b2)
                &&(a3>=b3)
                &&(a4<b4)){

            f.setY(awh.y());
            res=true;
        }

        if((a2<b2)&(a4>b4)){

            f.setY(0);
            res=true;
        }

        if((a2>b2)&(a4<b4)){

            f.setY(area_h);
            res=true;
        }





          if(!res){
        qDebug()<<"false!!!";
           return false;
           }

    }
    scale*=scaleFactor;


    //   QPointF q = neq QPointF(0,0);

     QPointF p =rect->mapFromScene(f);

   // qDebug()<<"p: "<<p.x()<<" "<<p.y();
    rect->setTransformOriginPoint(p);
    //   item->addPoint(p);




    //   if(rect->mapToScene(0,0).x()<area->mapToScene(0,0).x())
    //   if(rect->mapToScene(rect_w,0).x()>area->mapToScene(area_w,0).x())

    //    if(rect->mapToScene(0,0).y()<area->mapToScene(0,0).y())
    //    if(rect->mapToScene(0,rect_h).y()>area->mapToScene(0,area_h).y()){

    rect->setScale(scale);
 //   qDebug()<<"point2: "<<p.x()<<" "<<p.y();

    QPointF point3 = rect->mapToScene(p);

//    qDebug()<<"point3: "<<point3.x()<<" "<<point3.y();

    rect->moveBy((f.x()-point3.x()),(f.y()-point3.y()));




    /*
     QPointF point2 = rect->mapToScene(p);

     rect->moveBy((point.x()-point2.x()),(point.y()-point2.y()));

     qDebug()<<": "<<point.x()-point2.x()<<" "<<point.y()-point2.y();

*/
    //   }
    update_meta();
    return true;
}



MyView::~MyView()
{

}



void MyView::update_meta(QSharedPointer<Meta> mmeta)
{
   meta=mmeta;
}

void MyView::load(QString filapath)
{
    rect->moveBy(0,0);
    area->moveBy(0,0);
    deleteItemsFromGroup(points);
    rect->load(filapath);
    setSceneRect(0,0,rect->width,rect->height);
    area->setRect(0,0,rect->width,rect->height);

    area_w=rect->width;
    area_h=rect->height;

    rect_w=rect->width;
    rect_h=rect->height;


}

void MyView::mouseMoveEvent(QMouseEvent *event)
{
     QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

     int x = -(prev.x()-f.x());
     int y = -(prev.y()-f.y());

  //   qDebug()<<x<<" "<<y;

     if(flag==true){

         qDebug()<<rect->mapToScene(0,0).x()<<" "<<area->mapToScene(0,0).x();

        if(rect->mapToScene(0,0).x()+x<=area->mapToScene(0,0).x())
        if(rect->mapToScene(rect_w,0).x()+x>=area->mapToScene(area->rect().width(),0).x())

        if(rect->mapToScene(0,0).y()+y<=area->mapToScene(0,0).y())

        if(rect->mapToScene(0,rect_h).y()+y>=area->mapToScene(0,area->rect().height()).y()){
            rect->moveBy(x,y);
            update_meta();
        }


     }



     prev=f;

}


void MyView::mousePressEvent(QMouseEvent *event)
{

     QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

     QPointF ff=area->mapFromScene(f);

    auto res = area->contains(ff);

    if(res==true)
        flag=true;

    qDebug()<<res;

    prev=f;

      QPointF p =rect->mapFromScene(f);
      qDebug()<<"p: "<<p.x()<<" "<<p.y();

 QPointF point2 = rect->mapToScene(p);
 qDebug()<<"point2: "<<point2.x()<<" "<<point2.y();
}

void MyView::mouseReleaseEvent(QMouseEvent *event)
{
    flag=false;
}

void MyView::resizeEvent(QResizeEvent *event)
{
    int w= event->size().width();
    int h = event->size().height();


    qDebug()<<"resize "<<w<<" "<<h;
    setSceneRect(0,0,w,h);

     area->setRect(0,0,w,h);
    QPointF x=QPointF(0,0);

    QPointF a=area->mapToScene(0,0);

    area->moveBy(x.x()-a.x(),x.y()-a.y());

    rect->moveBy(x.x()-a.x(),x.y()-a.y());

    if(rect->mapToScene(0,0).x()>area->mapToScene(0,0).x()){

         rect->moveBy(rect->mapToScene(0,0).x()-area->mapToScene(0,0).x(),0);
     }

  if(rect->mapToScene(rect_w,0).x()<area->mapToScene(area->rect().width(),0).x()){

      rect->moveBy(area->mapToScene(area->rect().width(),0).x()-rect->mapToScene(rect_w,0).x(),0);
  }




     if(rect->mapToScene(0,0).y()>area->mapToScene(0,0).y()){

          rect->moveBy(0,rect->mapToScene(0,0).y()-area->mapToScene(0,0).y());

     }
    if(rect->mapToScene(0,rect_h).y()<area->mapToScene(0,area->rect().height()).y()){

          rect->moveBy(0,area->mapToScene(0,area->rect().height()).y()-rect->mapToScene(0,rect_h).y());
    }

    update_meta();
}

void MyView::wheelEvent(QWheelEvent *event)
{
 qDebug()<<"MyView::wheelEvent";

    QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));

    QPointF ff=area->mapFromScene(f);

    qDebug()<<f.x()<<" "<<f.y();

   auto res = area->contains(ff);

   if(res!=true){
       qDebug()<<"fail!!";
   return;
   }
   if (event->delta() > 0) zoomIn();
   else zoomOut();
/*
       if (event->modifiers() == Qt::ControlModifier)
       {

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
       */
}
