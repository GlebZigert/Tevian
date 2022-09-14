#include "rest.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkProxy>

Rest::Rest(QObject *parent) : QObject(parent)
{
    qDebug()<<"Rest::Rest";
    connect(&mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));

    request_token();
}

Rest::~Rest()
{
    qDebug()<<"Rest::~Rest";
}



void Rest::request_token()
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
     expect=expectType::token;
}

void Rest::get_token_from_JSON(QJsonDocument doc)
{
    int status = doc.object().value("status_code").toInt();
    qDebug()<<"status: "<<status;
    QJsonObject jsonObject;
    QJsonObject data;
    switch (status) {
    case 200:
        qDebug()<<"success: access token returned";
         jsonObject = doc.object();
         data =jsonObject["data"].toObject();
        token=data["access_token"].toString();
        qDebug()<<"token: "<<token;

        break;

    case 400:
        qDebug()<<"error detected in request.";
        break;

    case 401:
        qDebug()<<"invalid email or password provided";
        break;

    default:
        break;
    }

}


void Rest::onfinish(QNetworkReply *rep)
{

    QByteArray bts = rep->readAll();
    QString str(bts);
    //Разберем JSON
    qDebug()<<str;
    QJsonParseError docError;
    QJsonDocument doc=QJsonDocument::fromJson(bts,&docError);

     if(docError.errorString().toInt()!=QJsonParseError::NoError){
         qDebug()<<"Информация: Ошибки с форматом передачи данных"<<docError.errorString();
         return;

     }
    switch (expect) {
    case expectType::token:

        qDebug()<<"достаем токен из JSON";
        get_token_from_JSON(doc);


        break;
    default:
        break;
    }




}
