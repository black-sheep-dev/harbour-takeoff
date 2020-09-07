#include <QtQuick>
#include <sailfishapp.h>

#include <QTranslator>

#include "autostartmanager.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-takeoff.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QCoreApplication::setApplicationName(QStringLiteral("Takeoff"));
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

    qmlRegisterType<App>("org.nubecula.harbour.takeoff", 1, 0, "App");
    qmlRegisterType<AppListModel>("org.nubecula.harbour.takeoff", 1, 0, "AppListModel");

    qmlRegisterSingletonType<AutostartManager>("org.nubecula.harbour.takeoff",
                                               1,
                                               0,
                                               "AutostartManager",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        AutostartManager *manager = new AutostartManager();

        return manager;
    });

    return SailfishApp::main(argc, argv);
}
