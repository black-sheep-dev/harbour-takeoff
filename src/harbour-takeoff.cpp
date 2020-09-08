#include <QtQuick>
#include <sailfishapp.h>

#include "autostartmanager.h"

#include <thread>

int main(int argc, char *argv[])
{
    // start processes on start
    if (argc > 1) {
        for (int i = 0; i < argc; ++i) {
            if (QString(argv[i]) == QStringLiteral("--takeoff")) {

                QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                    return 0;

                QTextStream in(&file);

                while (!in.atEnd()) {
                    const QString line = in.readLine().simplified();

                    if (line.startsWith("###")) {
                        QProcess *process = new QProcess();
                        process->startDetached(in.readLine().simplified());
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                    }
                }

                file.close();

                std::this_thread::sleep_for(std::chrono::milliseconds(3000));

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
}
