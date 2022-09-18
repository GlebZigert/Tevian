#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rest.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&rest,SIGNAL(landmarks(QList<QPointF>)),this,SLOT(draw_landmarks(QList<QPointF>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::draw_landmarks(QList<QPointF> landmarks)
{
    qDebug()<<"draw_landmarks(QList<QPointF> landmarks)";
      ui->widget->update_ladmarks(landmarks);
}


void MainWindow::on_actionLOAD_triggered()
{

    QStringList paths = QFileDialog::getOpenFileNames(this,
        tr("Open Image"),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (paths.isEmpty())
        return;

    foreach(auto one,paths){
        qDebug()<<paths;
    }

 //   ui->widget->load(qStrFilePath);
   // ui->m_graphicsView->viewFit();





  //  rest.request_detect(qStrFilePath);

}

