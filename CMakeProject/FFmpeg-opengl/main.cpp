#include <QObject>
#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <qDebug>
#include <QLabel>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    cout << "hello cmake!" << endl;

    QMainWindow* view = new QMainWindow;
    view->setWindowTitle("upper monitor");
    view->show();

    return app.exec();
}