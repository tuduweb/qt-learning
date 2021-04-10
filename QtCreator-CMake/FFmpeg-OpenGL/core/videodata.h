#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <QThread>
class VideoData : public QThread
{
    Q_OBJECT
public:
    VideoData(QObject *parent = nullptr);
    void setUrl(QString url){ m_url = url; };

signals:
    void sigNewFrame();
    void sigStarted(uchar*,int,int);

protected:
    void run() override;

private:
    QString m_url;
    bool is_started = false;
};

#endif // VIDEODATA_H
