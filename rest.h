#ifndef REST_H
#define REST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkProxy>
#include <QList>
#include <QPoint>
#include <QFile>

enum expectType{
    token,
    detect,
    anything_else

};

class Rest : public QObject
{
    Q_OBJECT
public:
    explicit Rest(QObject *parent = nullptr);
    ~Rest();

    void request_detect(QString filepath);

private:

 QNetworkAccessManager mgr;
 QString token;

 int expect;

 void request_token();

 void get_token_from_JSON(QJsonDocument doc);

 void get_bbox_from_JSON(QJsonDocument doc);


private slots:
     void onfinish(QNetworkReply *rep);
signals:
     void box(int,int,int,int);
     void landmarks(QList<QPointF>);
};

#endif // REST_H
