#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rest.h"
#include <QStandardPaths>
#include <meta.h>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList paths;
    int current;

    QMap<QString, QSharedPointer<Meta>> map;

    QSharedPointer<Meta> convertJsonToMeta(QJsonObject);

    void load(int index);


private slots:
    void on_actionLOAD_triggered();
    void get_meta(QString file, QJsonObject meta);

    void on_pushButton_2_clicked();
    void wheelEvent(QWheelEvent *event) override;
    void on_pushButton_3_clicked();



    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Rest rest;






};
#endif // MAINWINDOW_H
