#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtNetwork/QNetworkReply>
#include "rest.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_graphicsScene;

    Rest rest;

private slots:
    void onPressLoadImage();
    void onPressSaveImage();
    void onPressFitWindow();
    void onfinish(QNetworkReply *rep);

};

#endif // MAINWINDOW_H
