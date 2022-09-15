#include "mygraphicsview.h"
#include "QGraphicsEllipseItem"
#include <QImageReader>
#include <QDebug>
#include <cmath>

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);

	isResized = false;
	isLandscape = false;

    xxx=50;
    n=0;
    setDragMode(QGraphicsView::ScrollHandDrag);

    scene = new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    group_1 = new QGraphicsItemGroup();
    group_2 = new QGraphicsItemGroup();

    qDebug()<<"items at [0] "<<scene->items().size();
    scene->addItem(group_1);
    qDebug()<<"items at [1] "<<scene->items().size();
    scene->addItem(group_2);
    qDebug()<<"items at [2] "<<scene->items().size();

    QImage bground(50, 50, QImage::Format_RGB888);
    for (int y = 0; y < 25; y++)
    {
        for (int x = 0; x < 25; x++)
        {
            bground.setPixel(x, y, qRgb(0xCA, 0xCA, 0xCA));
            bground.setPixel(x + 25, y, qRgb(0xFF, 0xFF, 0xFF));
            bground.setPixel(x, y + 25, qRgb(0xFF, 0xFF, 0xFF));
            bground.setPixel(x + 25, y + 25, qRgb(0xCA, 0xCA, 0xCA));
        }
    }
    scene->setBackgroundBrush(QPixmap::fromImage(bground));




   this->setScene(scene);


}

MyGraphicsView::~MyGraphicsView()
{
}

void MyGraphicsView::zoomIn()
{
     if(n<30){
    n++;

	scaleView(qreal(1.2));
     }
}

void MyGraphicsView::zoomOut()
{
    if(n>1){
        n--;
    scaleView(1/qreal(1.2));
    }
}

void MyGraphicsView::load(QString qStrFilePath)
{
    if (qStrFilePath.isEmpty())
        return;

    QImageReader reader(qStrFilePath);
    if (!reader.canRead())
    {

        qDebug()<<"Cannot read file";

        return;
    }
    if (!scene->sceneRect().isEmpty())
    {
        deleteItemsFromGroup(group_1);
        deleteItemsFromGroup(group_2);
        qDebug()<<"items at [3] "<<scene->items().size();
    }
    QImage qimg = reader.read();
    scene->setSceneRect(qimg.rect());


    w = qimg.width();
    h = qimg.height();
    QGraphicsPixmapItem* pxm = new QGraphicsPixmapItem(QPixmap::fromImage(qimg));
    pointer = pxm;


    qDebug()<<"items at [4]"<<scene->items().size();
    group_1->addToGroup(scene->addPixmap(QPixmap::fromImage(qimg)));
 qDebug()<<"items  at [5]"<<scene->items().size();


 auto circle = new QGraphicsEllipseItem(150, 150, 25, 25,pxm);
// circle->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
 circle->setBrush(Qt::green);
 circle->setFlag(QGraphicsItem::ItemIgnoresTransformations);

group_1->addToGroup(circle);
 // group_1->addToGroup(scene->addPixmap(QPixmap::fromImage(qimg)));

  /* Нарисуем красный квадрат
   * */

  /*
  QPen penRed(Qt::red);   //
  int width = this->width();      // определяем ширину нашего виджета
  int height = this->height();    // определяем высоту нашего виджета
  int sideOfSquare = (height > width) ? (width - 60) : (height - 60);
  int centerOfWidget_X = width/2;
  int centerOfWidget_Y = height/2;

  group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
                                     centerOfWidget_Y - (sideOfSquare/2),
                                     centerOfWidget_X + (sideOfSquare/2),
                                     centerOfWidget_Y - (sideOfSquare/2),
                                     penRed));

  group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
                                     centerOfWidget_Y - (sideOfSquare/2),
                                     centerOfWidget_X + (sideOfSquare/2),
                                     centerOfWidget_Y + (sideOfSquare/2),
                                     penRed));

  group_2->addToGroup(scene->addLine(centerOfWidget_X + (sideOfSquare/2),
                                     centerOfWidget_Y + (sideOfSquare/2),
                                     centerOfWidget_X - (sideOfSquare/2),
                                     centerOfWidget_Y + (sideOfSquare/2),
                                     penRed));

  group_2->addToGroup(scene->addLine(centerOfWidget_X - (sideOfSquare/2),
                                     centerOfWidget_Y + (sideOfSquare/2),
                                     centerOfWidget_X - (sideOfSquare/2),
                                     centerOfWidget_Y - (sideOfSquare/2),
                                     penRed));
*/
    viewFit();

}

void MyGraphicsView::viewFit()
{
	fitInView(sceneRect(), Qt::KeepAspectRatio);
	isResized = true;

	if (sceneRect().width() > sceneRect().height())
		isLandscape = true;
    else
		isLandscape = false;
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{

  //  scene->addItem(circle);

 qDebug()<<"items "<<scene->items().size();

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

void MyGraphicsView::scaleView(qreal scaleFactor)
{

 //   deleteItemsFromGroup(group_2);



    if(sceneRect().isEmpty())
        return;

	QRectF expectedRect = transform().scale(scaleFactor, scaleFactor).mapRect(sceneRect());
	qreal expRectLength;
	int viewportLength;
	int imgLength;

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
		if (!isResized || scaleFactor < 1)
			return;
	}
    else if (expRectLength > imgLength * 30) // maximum zoom : x10
	{
		if (!isResized || scaleFactor > 1)
			return;
	}
	else
	{
		isResized = false;
	}

	scale(scaleFactor, scaleFactor);
     xxx=xxx*scaleFactor;
     /*
    qDebug()<<n
           <<" "<<xxx<<" "<<50/pow(0.8,n);
             ;
    */
   //  qDebug()<<n<<" "<<w<<" "<<h<<" "<<scene->width()<<" "<<scene->height()<<" "<<this->width()<<" "<<this->height();

  //  auto point = QGraphicsItem::mapFromItem(&pointer, QPointF(50,50));

QPointF point = pointer->mapToScene(50,50);
qDebug()<<"point: "<<point.x()<<" "<<point.y();
}

void MyGraphicsView::deleteItemsFromGroup(QGraphicsItemGroup *group)
{
    /* Перебираем все элементы сцены, и если они принадлежат группе,
     * переданной в метод, то удаляем их
     * */
    if(scene->items().size()>0)
    foreach( QGraphicsItem *item, scene->items()) {
       if(item->group() == group ) {
           group->removeFromGroup(item);
           scene->removeItem(item);
          delete item;
           qDebug()<<"delete";
       }
    }

}




void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
	isResized = true;
	QGraphicsView::resizeEvent(event);
}

