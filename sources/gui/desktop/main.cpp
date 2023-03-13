#include "mainwindow.h"

#include "persistence_registration.h"
#include "presenter_registration.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "skribisto-desktopp_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    new Persistence::PersistenceRegistration(&app);
    new Presenter::PresenterRegistration(&app);

    MainWindow w;
    w.show();
    return app.exec();
}
