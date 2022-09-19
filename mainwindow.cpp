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

    for(QJsonValue one : landmarks_json) {

        QPointF point;
        int x = one["x"].toInt();
        int y = one["y"].toInt();

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

    add(newPaths);


}

void MainWindow::get_meta(QString file, QJsonObject meta)
{
    qDebug()<<"get meta for "<<file;
    map.insert(file,convertJsonToMeta(meta));

    if(map.size()<paths.size()){
        QString str="Обработано изображений: ";
        str+=QString::number(map.size());
        str+=" из ";
        str+=QString::number(paths.size());

        ui->label->setText(str);
    }else{
        ui->label->clear();
    }
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

    if(paths.size()==0)
        return;

    load(current);

    if(current-1<0){
        current=paths.size()-1;
    }else
        current--;
}

void MainWindow::add(QStringList newPaths)
{
    if (newPaths.isEmpty())
        return;

    foreach(auto one,newPaths){
        if(!paths.contains(one)){
            paths.append(one);
            rest.add_request(one);
        }


    }

    rest.request_detect();
}

void MainWindow::on_pushButton_clicked()
{
    on_actionLOAD_triggered();
}

void MainWindow::on_actionLoadFolder_triggered()
{
    QString path = QFileDialog::getExistingDirectory(this);
    qDebug()<<path;

    QStringList nameFilters;
    nameFilters<<"*jpg";
    nameFilters<<"*jpeg";
    QStringList result;
    QDir dir(path);
    if(dir.exists() == false){
        return;
    }
    QFileInfoList lst = dir.entryInfoList(nameFilters,QDir::Files);
    foreach(QFileInfo fi, lst){
        QString fpath = fi.absoluteFilePath();



        result += fpath;
    }

    if (result.isEmpty())
        return;

    add(result);

}

