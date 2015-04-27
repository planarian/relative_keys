#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include "mainWindow.h"

class I : public QThread   //borrowed from http://qt-project.org/wiki/How_to_create_a_splash_screen_with_an_induced_delay
{
public:
        static void sleep(unsigned long secs) {
                QThread::sleep(secs);
        }
};

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    QSplashScreen * splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/img/splash.png"));
    splash->show();
    I::sleep(2);

    MainWindow appWindow;
    appWindow.showMaximized();

    splash->finish(&appWindow);
    delete splash;

    return a.exec();
}


