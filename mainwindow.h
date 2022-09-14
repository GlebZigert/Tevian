#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "requester/requester.h"
#include <QtNetwork/QNetworkReply>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Requester requester;

private:
    Ui::MainWindow *ui;
    QGraphicsScene* m_graphicsScene;

private slots:
    void onPressLoadImage();
    void onPressSaveImage();
    void onPressFitWindow();
    void onfinish(QNetworkReply *rep);
};

#endif // MAINWINDOW_H
