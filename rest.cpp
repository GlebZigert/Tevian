#include "rest.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkProxy>

Rest::Rest(QObject *parent) : QObject(parent)
{
    qDebug()<<"Rest::Rest";
    connect(&mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));

    get_token();
}

Rest::~Rest()
{
    qDebug()<<"Rest::~Rest";
}

void Rest::get_token()
{
    const QUrl url(QStringLiteral("https://backend.facecloud.tevian.ru/api/v1/login"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
     QJsonObject obj;
    obj["email"] = "gleb.zigert@yandex.ru";
    obj["password"] = "gztevian";
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
     mgr.post(request, data);
}

void Rest::onfinish(QNetworkReply *rep)
{

    QByteArray bts = rep->readAll();
    QString str(bts);
    qDebug()<<str;
}
