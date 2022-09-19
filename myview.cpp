#include "myview.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QTime>

MyView::MyView(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    setScene(scene);
    points = new QGraphicsItemGroup();
    rect_w=1920;
    rect_h=1080;
    n=1;
    scene->setSceneRect(0,0,1920,1080);
    scale=1;
    flag = false;
    QPen pen(Qt::white);
    area = new QGraphicsRectItem(0,0,scene->width()/2,scene->height()/2);
    area->setFlag(QGraphicsItem::ItemIsMovable,false);
    area->setPen(pen);
    scene->addItem(area);
    QPen penRed(Qt::red);
    rect = new MyItem();
    rect->setFlag(QGraphicsItem::ItemIsMovable,true);
    scene->addItem(rect);
    rect->moveBy(0,0);
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
    if(n>-50){
        qDebug()<<"out";
        if(scaleView(1/qreal(1.05)))
            n--;
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
        QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(xx.x(),xx.y(),2,2,nullptr);


        circle->setPen(QPen(Qt::green, 2));

        points->addToGroup(circle);
    }
    QGraphicsRectItem *bbox = new QGraphicsRectItem(rect->mapRectToScene(meta->bbox));
    bbox->setPen(QPen(Qt::red, 2));
    points->addToGroup(bbox);
    QGraphicsTextItem *mask = new QGraphicsTextItem("full face mask: "+meta->full_face_mask,nullptr);
    QGraphicsTextItem *mask1 = new QGraphicsTextItem("lower face mask: "+meta->lower_face_mask,nullptr);
    QGraphicsTextItem *mask2 = new QGraphicsTextItem("no face mask: "+meta->no_mask,nullptr);
    QGraphicsTextItem *mask3 = new QGraphicsTextItem("other mask: "+meta->other_mask,nullptr);
    QGraphicsRectItem* back = new QGraphicsRectItem(0,0,200,50);
    back->setPen(QPen(Qt::white, 2));
    back->setBrush(Qt::white);
    mask1->moveBy(0,10);
    mask2->moveBy(0,20);
    mask3->moveBy(0,30);
    points->addToGroup(back);
    points->addToGroup(mask);
    points->addToGroup(mask1);
    points->addToGroup(mask2);
    points->addToGroup(mask3);



}

bool MyView::scaleView(qreal scaleFactor)
{
    QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
    scale*=scaleFactor;
    QPointF p =rect->mapFromScene(f);
    rect->setTransformOriginPoint(p);
    rect->setScale(scale);
    QPointF point3 = rect->mapToScene(p);
    rect->moveBy((f.x()-point3.x()),(f.y()-point3.y()));
    update_meta();

    if(((rect->mapToScene(0,0).x())>area->mapToScene(area->rect().width(),0).x())||
            ((rect->mapToScene(0,0).y())>area->mapToScene(0,area->rect().height()).y())||
            ((rect->mapToScene(rect_w,rect_h).x())<area->mapToScene(0,0).x())||
            (rect->mapToScene(rect_w,rect_h).y()<area->mapToScene(0,0).y())){


        QPointF p1 = area->mapToScene(0,0);
        QPointF p2 = rect->mapToScene(0,0);
        rect->moveBy(p1.x()-p2.x(),p1.y()-p2.y());
    }
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
    if(flag==true){
        if(rect->mapToScene(0,0).x()+x<=area->mapToScene(0,0).x())
            if(rect->mapToScene(rect_w,0).x()+x>=area->mapToScene(area->rect().width(),0).x())
                if(rect->mapToScene(0,0).y()+y<=area->mapToScene(0,0).y())
                    if(rect->mapToScene(0,rect_h).y()+y>=area->mapToScene(0,area->rect().height()).y()){
                    }
        rect->moveBy(x,y);
        update_meta();
    }
    prev=f;
    if(((rect->mapToScene(0,0).x())>area->mapToScene(area->rect().width(),0).x())||
            ((rect->mapToScene(0,0).y())>area->mapToScene(0,area->rect().height()).y())||
            ((rect->mapToScene(rect_w,rect_h).x())<area->mapToScene(0,0).x())||
            (rect->mapToScene(rect_w,rect_h).y()<area->mapToScene(0,0).y())){
        QPointF p1 = area->mapToScene(0,0);
        QPointF p2 = rect->mapToScene(0,0);
        rect->moveBy(p1.x()-p2.x(),p1.y()-p2.y());
    }
}


void MyView::mousePressEvent(QMouseEvent *event)
{
    QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
    QPointF ff=rect->mapFromScene(f);
    auto res = rect->contains(ff);
    if(res==true)
        flag=true;
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
    update_meta();
}

void MyView::wheelEvent(QWheelEvent *event)
{
    QPointF f =mapToScene(viewport()->mapFromGlobal(QCursor::pos()));
    QPointF ff=rect->mapFromScene(f);
    auto res = rect->contains(ff);
    if(res!=true){
        return;
    }
    if (event->delta() > 0) zoomIn();
    else zoomOut();
}
