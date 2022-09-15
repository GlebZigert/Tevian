#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

class MyGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	MyGraphicsView(QWidget *parent = 0);
	~MyGraphicsView();
	void viewFit();
    void zoomIn();
    void zoomOut();
    void load(QString filepath);
    int xxx;
    int n;
    int w,h;
    QGraphicsScene* scene;
    QGraphicsItemGroup  *group_1;
    QGraphicsItemGroup  *group_2;

    QGraphicsPixmapItem* pointer;
private:
    void scaleView(qreal scaleFactor);
    bool isResized;
	bool isLandscape;
    void deleteItemsFromGroup(QGraphicsItemGroup *group);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
