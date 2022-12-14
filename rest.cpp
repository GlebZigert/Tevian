#include "rest.h"

Rest::Rest(QObject *parent) : QObject(parent)
{
    qDebug()<<"Rest::Rest";
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("192.168.0.1");
    proxy.setPort(3128);

    mgr.setProxy(proxy);
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

void Rest::request_detect()
{

    if(queue.size()==0){
        return;
    }
    current = queue.dequeue();
    qDebug()<<"Rest::request_detect for: "<<current;
    const QUrl url(QStringLiteral("https://backend.facecloud.tevian.ru/api/v1/detect?fd_min_size=0&fd_max_size=0&fd_threshold=0.8&rotate_until_faces_found=false&orientation_classifier=false&demographics=true&attributes=true&landmarks=false&liveness=false&quality=false&masks=false"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "image/jpeg");
    request.setRawHeader(QByteArray("Authorization"), QByteArray(token.toUtf8()));

    qDebug()<<"filePath: "<<current;
    QFile CurrentFile(current);
    if(!CurrentFile.open(QIODevice::ReadOnly)) return;

    QByteArray data = CurrentFile.readAll();
    qDebug()<<"data size: "<<data.size();

    mgr.post(request, data);
    expect=expectType::detect;
}

void Rest::add_request(QString filepath)
{
    queue.enqueue(filepath);
}

void Rest::get_token_from_JSON(QJsonDocument doc)
{
    int status = doc.object().value("status_code").toInt();
    qDebug()<<"status: "<<status;

    if (status==200) {
        qDebug()<<"success: access token returned";
        QJsonObject jsonObject = doc.object();
        QJsonObject data =jsonObject["data"].toObject();
        token="Bearer ";
        token+=data["access_token"].toString();
        qDebug()<<"token: "<<token;
    }
}

void Rest::get_bbox_from_JSON(QJsonDocument doc)
{
    int status = doc.object().value("status_code").toInt();
    qDebug()<<"status: "<<status;

    if (status==200) {
        qDebug()<<"success";
        QJsonObject jsonObject = doc.object();
        QJsonArray data = jsonObject.value("data").toArray();
        QJsonObject one = data[0].toObject();

        emit meta(current,one);

        request_detect();
    }
}

void Rest::onfinish(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    //???????????????? JSON
    qDebug()<<str;
    QJsonParseError docError;
    QJsonDocument doc=QJsonDocument::fromJson(bts,&docError);

    if(docError.errorString().toInt()!=QJsonParseError::NoError){
        qDebug()<<"????????????????????: ???????????? ?? ???????????????? ???????????????? ????????????"<<docError.errorString();
        return;
    }
    switch (expect) {
    case expectType::token:

        qDebug()<<"?????????????? ?????????? ???? JSON";
        get_token_from_JSON(doc);

        break;

    case expectType::detect:

        qDebug()<<"detect";
        get_bbox_from_JSON(doc);

        break;

    default:
        break;
    }
}
