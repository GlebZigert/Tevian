﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>


#include <QImageWriter>

#include <QMessageBox>

#include <QHttpPart>

#include <QNetworkProxy>


#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkProxy>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&rest,SIGNAL(box(int,int,int,int)),this,SLOT(draw_bbox(int,int,int,int)));
    connect(&rest,SIGNAL(landmarks(QList<QPoint>)),this,SLOT(draw_landmarks(QList<QPoint>)));




    ui->statusBar->showMessage("ready", 0);




}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPressLoadImage()
{
    QString qStrFilePath = QFileDialog::getOpenFileName(this,
        tr("Open Image"),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (qStrFilePath.isEmpty())
        return;

    ui->m_graphicsView->load(qStrFilePath);
   // ui->m_graphicsView->viewFit();

    ui->statusBar->showMessage("image loaded", 0);



    rest.request_detect(qStrFilePath);


}


void MainWindow::onPressFitWindow()
{
    ui->m_graphicsView->viewFit();
}

void MainWindow::onfinish(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    qDebug()<<str;
}

void MainWindow::draw_bbox(int h, int w, int x, int y)
{
    qDebug()<<"draw bbox: "<<h<<" "<<w<<" "<<x<<" "<<y;


}

void MainWindow::draw_landmarks(QList<QPoint> landmarks)
{
    qDebug()<<"draw landmarks: ";
/*
       for(QPoint point : landmarks) {

           auto circle = new QGraphicsEllipseItem(point.x(), point.y(), 5, 5);
           circle->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
           circle->setBrush(Qt::green);
           circle->setFlag(QGraphicsItem::ItemIgnoresTransformations);
          m_graphicsScene->addItem(circle);
       }
       */



}


