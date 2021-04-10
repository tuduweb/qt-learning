#ifndef REMOTESTREAMINTERFACE_H
#define REMOTESTREAMINTERFACE_H

#include <QObject>

class RemoteStreamInterface : public QObject
{
    Q_OBJECT
public:
    explicit RemoteStreamInterface(QObject *parent = nullptr);

    virtual int StreamInit() { return -1; }
    virtual int StreamStart() { return -1; }
    virtual int StreamStop() { return -1; }
    virtual int StreamDestory() { return -1; }

    virtual void loadSettings(const QJsonObject& settings){ return; }


signals:

};

#endif // REMOTESTREAMINTERFACE_H
