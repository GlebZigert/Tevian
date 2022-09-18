#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rest.h"
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();





private slots:
    void on_actionLOAD_triggered();
   void draw_landmarks(QList<QPointF>);
private:
    Ui::MainWindow *ui;

    Rest rest;




};
#endif // MAINWINDOW_H
