#ifndef META_H
#define META_H

#include <QObject>

class Meta : public QObject
{
    Q_OBJECT
public:
    explicit Meta(QObject *parent = nullptr);

signals:

};

#endif // META_H
