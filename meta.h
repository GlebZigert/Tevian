#ifndef META_H
#define META_H

#include <QObject>
#include <QPointF>
#include <QList>
#include <QRectF>

class Meta : public QObject
{
    Q_OBJECT
public:
    explicit Meta(QObject *parent = nullptr);

     QList<QPointF> landmarks;
     QRectF bbox;

signals:

};

#endif // META_H
