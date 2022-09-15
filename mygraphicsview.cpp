#include "mygraphicsview.h"
#include "QGraphicsEllipseItem"
#include <QImageReader>
#include <QDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setTransformationAnchor(AnchorUnderMouse);

	isResized = false;
	isLandscape = false;

    setDragMode(QGraphicsView::ScrollHandDrag);

    m_graphicsScene = new QGraphicsScene();
    m_graphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);
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
    m_graphicsScene->setBackgroundBrush(QPixmap::fromImage(bground));

   this->setScene(m_graphicsScene);


}

MyGraphicsView::~MyGraphicsView()
{
}

void MyGraphicsView::zoomIn()
{
	scaleView(qreal(1.2));
}

void MyGraphicsView::zoomOut()
{
    scaleView(1 / qreal(1.2));
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
    if (!m_graphicsScene->sceneRect().isEmpty())
    {
        m_graphicsScene->clear();
    }
    QImage qimg = reader.read();
    m_graphicsScene->setSceneRect(qimg.rect());
    m_graphicsScene->addPixmap(QPixmap::fromImage(qimg));

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
    else if (expRectLength > imgLength * 10) // maximum zoom : x10
	{
		if (!isResized || scaleFactor > 1)
			return;
	}
	else
	{
		isResized = false;
	}

	scale(scaleFactor, scaleFactor);
    qDebug()<<scaleFactor
           <<" "<<this->scene()->width()
           <<" "<<this->scene()->height()
           <<" "<<this->height()
           <<" "<<this->width()
             ;

}

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
	isResized = true;
	QGraphicsView::resizeEvent(event);
}

