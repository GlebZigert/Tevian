﻿#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsScene>

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

    QGraphicsScene* m_graphicsScene;

private:
    void scaleView(qreal scaleFactor);
    bool isResized;
	bool isLandscape;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};
