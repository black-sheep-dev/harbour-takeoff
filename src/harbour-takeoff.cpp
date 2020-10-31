#include <QtQuick>
#include <sailfishapp.h>

#include "autostartmanager.h"

#include <thread>

#include "launcher.h"

int main(int argc, char *argv[])
{
    // start processes on start
    if (argc > 1) {
        for (int i = 0; i < argc; ++i) {
            if (QString(argv[i]) == QStringLiteral("--takeoff")) {

                Launcher::takeoff(true);

                exit(0);
            }
        }

    } else {
        // start gui
        QCoreApplication::setApplicationName(QStringLiteral("Takeoff"));
        QCoreApplication::setApplicationVersion(APP_VERSION);
        QCoreApplication::setOrganizationName(QStringLiteral("nubecula.org"));
        QCoreApplication::setOrganizationDomain(QStringLiteral("nubecula.org"));

        qmlRegisterType<App>("org.nubecula.harbour.takeoff", 1, 0, "App");
        qmlRegisterType<AppLibraryAPI>("org.nubecula.harbour.takeoff", 1, 0, "AppLibraryApi");
        qmlRegisterType<AppListModel>("org.nubecula.harbour.takeoff", 1, 0, "AppListModel");
        qmlRegisterType<AppListSortFilterModel>("org.nubecula.harbour.takeoff", 1, 0, "AppListSortFilterModel");

        qmlRegisterSingletonType<AutostartManager>("org.nubecula.harbour.takeoff",
                                                   1,
                                                   0,
                                                   "AutostartManager",
                                                   [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {

            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)

            auto *manager = new AutostartManager;

            return manager;
        });

        return SailfishApp::main(argc, argv);
    }
}
