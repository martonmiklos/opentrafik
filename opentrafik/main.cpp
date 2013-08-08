#include "mainwindow.h"
#include "svnversion.h"

#include <unistd.h>

#include <QApplication>
#include <QGuiApplication>
#include <QSplashScreen>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QTranslator>
#include <QSettings>

#include <qtsingleapplication.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("OpenTrafik");
    QCoreApplication::setApplicationVersion("1.0");
    QGuiApplication::setQuitOnLastWindowClosed(false);

    QtSingleApplication a(argc, argv);
    if (a.sendMessage(""))
        return 0;

    QSplashScreen splash;
    QImage splashImage(":/images/logo.png");

    /*QFont font;
    font.setBold(true);
    font.setFamily("Arial");
    font.setPixelSize(37);
    QPainter painter;
    painter.begin(&splashImage);
    painter.setFont(font);
    painter.drawText(242, 250, QString("SVN %1 build: %2").arg(SVN_REVISION).arg(BUILD_NUMBER));
    painter.end();*/

    splash.setPixmap(QPixmap::fromImage(splashImage));
    splash.show();
    a.processEvents();

    QSettings settings;
    settings.beginGroup("gui");
    QString lang = settings.value("lang").toString();
    QTranslator appTranslator, qtTranslator;
    qtTranslator.load(":/translations/qt_"+lang+".qm");
    appTranslator.load(":/translations/opentrafik_"+lang+".qm");
    a.installTranslator(&appTranslator);
    a.installTranslator(&qtTranslator);
    settings.endGroup();

    MainWindow w;
    splash.finish(&w);
    a.setActivationWindow(&w);
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)),
             &w, SLOT(show()));
    
    return a.exec();
}
