#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rest.h"
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setFocusPolicy(Qt::WheelFocus);
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

    meta->bbox.setX(x);
    meta->bbox.setY(y);
    meta->bbox.setWidth(width);
    meta->bbox.setHeight(height);

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

   QJsonObject masks = one["masks"].toObject();

   qDebug()<<"==masks: "<<masks;

   meta->full_face_mask=QString::number( masks["full_face_mask"].toDouble());
  meta->lower_face_mask=QString::number( masks["lower_face_mask"].toDouble());
meta->no_mask=QString::number( masks["no_mask"].toDouble());
meta->other_mask=QString::number( masks["other_mask"].toDouble());


  qDebug()<<"full_face_mask "<< meta->full_face_mask;

  qDebug()<<"lower_face_mask "<< meta->lower_face_mask;


  qDebug()<<"no_mask "<< meta->no_mask;


  qDebug()<<"other_mask "<< meta->other_mask;

   return meta;




}

void MainWindow::load(int index)
{
    QString key = paths.at(index);

    if(map.value(key).isNull())
        return;
    ui->widget->load(key);

    ui->widget->update_meta(map.value(key));


}



void MainWindow::on_actionLOAD_triggered()
{

     QStringList newPaths = QFileDialog::getOpenFileNames(this,
        tr("Open Image"),
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (newPaths.isEmpty())
        return;

    foreach(auto one,newPaths){
       if(!paths.contains(one)){
           paths.append(one);
            rest.add_request(one);
       }


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







load(current);


if(current+1>=paths.size()){
    current=0;
}else
    current++;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    qDebug()<<"MainWindow::wheelEvent";
}


void MainWindow::on_pushButton_3_clicked()
{
qDebug()<<"prev";
qDebug()<<"next";


if(paths.size()==0)
    return;

load(current);

if(current-1<0){
    current=paths.size()-1;
}else
    current--;
}

