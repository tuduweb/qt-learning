#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QSplashScreen>
#include <QProgressBar>
#include <QLabel>
class SplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    explicit SplashScreen(QWidget *parent = nullptr);

    QProgressBar *progressBar;
    QLabel *label;


    //随机数列表
     QList<int> numbersList;
     //启动界面停留的时间
     int elapseTime;

     void generateAscendRandomNumber();
     void setProgress();

protected slots:
     void slotUpdateProgress();
signals:

};

#endif // SPLASHSCREEN_H
