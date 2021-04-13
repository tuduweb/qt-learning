#include "splashscreen.h"
#include <QGuiApplication>
#include <QPixmap>

SplashScreen::SplashScreen(QWidget *parent) : QSplashScreen(QGuiApplication::primaryScreen())
{
    QImage image("N:/splash.jpg");
    QPixmap pixmap = QPixmap::fromImage(image).scaled(1600, 900);

    setPixmap(pixmap);
    //this->showMessage(QStringLiteral("12345"), Qt::AlignHCenter|Qt::AlignBottom, Qt::red);
    progressBar = new QProgressBar(this);
    //设置进度条的位置
    progressBar->setGeometry(pixmap.width()*0.02,pixmap.height()-50,pixmap.width()*0.96,30);
    //设置进度条的样式
    progressBar->setStyleSheet("QProgressBar {color:black;font:30px;text-align:center; }QProgressBar::chunk {background-color: rgb(202, 165, 14);}");
    //设置进度条的范围
    progressBar->setRange(0, 100);
    //设置进度条的当前进度
    progressBar->setValue(0);

    label = new QLabel(this);
    label->setAlignment(Qt::AlignRight);
    label->setGeometry(pixmap.width()*0.02,pixmap.height()-100,pixmap.width()*0.96, 50);
    label->setStyleSheet("QLabel {color: black; text-align:right; font-size:48px;}");
    label->setText(QString("LOADING"));

    elapseTime = 10000;

    generateAscendRandomNumber();
    setProgress();



}

#include <QTime>
#include <QTimer>
void SplashScreen::generateAscendRandomNumber()
{

    int i;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //生成100个大小在[0,100]之间的随机数
    for(i=0;i<100;i++)
    {
        numbersList.append(qrand()%101);
    }
    //递增排序
    qSort(numbersList.begin(),numbersList.end());
}

void SplashScreen::setProgress()
{

    int tempTime=elapseTime/100;
    for(int i=0;i<100;i++)
    {
       QTimer::singleShot(i*tempTime, this, SLOT(slotUpdateProgress()));
    }
    QTimer::singleShot(elapseTime, this, SLOT(close()));
}

void SplashScreen::slotUpdateProgress()
{
    static int num=0;
    progressBar->setValue(numbersList[num]);
    num++;
}
