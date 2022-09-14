#ifndef REST_H
#define REST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

enum expectType{
        token,
        anything_else

    };

class Rest : public QObject
{
    Q_OBJECT
public:
    explicit Rest(QObject *parent = nullptr);
    ~Rest();

private:

 QNetworkAccessManager mgr;
 QString token;

 int expect;

 void request_token();
 void get_token_from_JSON(QJsonDocument doc);


private slots:
     void onfinish(QNetworkReply *rep);
signals:

};

#endif // REST_H
