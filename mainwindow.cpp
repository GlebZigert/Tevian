#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rest.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    current=0;
    connect(&rest,SIGNAL(meta(QString,QJsonObject)),this,SLOT(get_meta(QString,QJsonObject)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSharedPointer<Meta> MainWindow::convertJsonToMeta(QJsonObject one)
{

       QSharedPointer<Meta> meta =  QSharedPointer<Meta>::create();

    QJsonObject bbox = one["bbox"].toObject();

    int height = bbox["height"].toInt();
    int width = bbox["width"].toInt();
    int x = bbox["x"].toInt();
    int y = bbox["y"].toInt();

      qDebug()<<"get bbox: "<<height<<" "<<width<<" "<<x<<" "<<y;

   QJsonArray landmarks_json = one["landmarks"].toArray();

   QList<QPointF> list;
//      qDebug()<<" landmarks:";
   for(QJsonValue one : landmarks_json) {

       QPointF point;
       int x = one["x"].toInt();
       int y = one["y"].toInt();
  //     qDebug()<<x<<" "<<y;
       point.setX(x);
       point.setY(y);
       meta->landmarks.append(point);
   }



   return meta;




}



void MainWindow::on_actionLOAD_triggered()
{

     paths = QFileDialog::getOpenFileNames(this,
        tr("Open Image"),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (paths.isEmpty())
        return;

    foreach(auto one,paths){
        qDebug()<<one;
        rest.add_request(one);
    }
    rest.request_detect();

 //   ui->widget->load(qStrFilePath);
   // ui->m_graphicsView->viewFit();





  //  rest.request_detect(qStrFilePath);

}

void MainWindow::get_meta(QString file, QJsonObject meta)
{
    qDebug()<<"get meta for "<<file;
    map.insert(file,convertJsonToMeta(meta));



}


void MainWindow::on_pushButton_2_clicked()
{
qDebug()<<"next";
if(paths.size()==0)
    return;







QString key = paths.at(current);

ui->widget->load(key);
ui->widget->update_ladmarks(map.value(key)->landmarks);
if(current+1>=paths.size()){
    current=0;
}else
current++;
}


void MainWindow::on_pushButton_3_clicked()
{
qDebug()<<"prev";
}

