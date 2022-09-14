#ifndef REST_H
#define REST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class Rest : public QObject
{
    Q_OBJECT
public:
    explicit Rest(QObject *parent = nullptr);
    ~Rest();

private:

 QNetworkAccessManager mgr;
 QString token;

 void get_token();


private slots:
     void onfinish(QNetworkReply *rep);
signals:

};

#endif // REST_H
