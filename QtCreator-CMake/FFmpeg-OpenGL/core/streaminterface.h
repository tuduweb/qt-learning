#ifndef STREAMINTERFACE_H
#define STREAMINTERFACE_H

#include <QObject>

class StreamInterface : public QObject
{
    Q_OBJECT
public:
    explicit StreamInterface(QObject *parent = nullptr);

signals:

};

#endif // STREAMINTERFACE_H
