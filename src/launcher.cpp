#include "launcher.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

#include <thread>

void Launcher::takeoff(bool wait)
{
    // read start delay
    if (wait) {
        QSettings settings;

        settings.beginGroup(QStringLiteral("STARTUP"));
        int delay = settings.value(QStringLiteral("delay"), 0).toInt();
        settings.endGroup();

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    // read defintion file and start processes
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/harbour-takeoff/takeoff.def"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().simplified();

        if (line.isEmpty())
            continue;

        // read *.desktop file
        QSettings ini(QStringLiteral("/usr/share/applications/") + line, QSettings::IniFormat);

        ini.beginGroup(QStringLiteral("Desktop Entry"));

        const QString cmd = ini.value(QStringLiteral("Exec")).toString();

        QString runCmd;

        if (cmd.contains(QStringLiteral("sailjail")))
            continue;



        if ( cmd.contains(QStringLiteral("apkd-launcher"))
             || cmd.contains(QStringLiteral("invoker"))
             || !ini.contains(QStringLiteral("X-Nemo-Application-Type")) ) {

            runCmd = cmd;

        } else {

            runCmd = QString("/usr/bin/invoker --type=%1 -s -n -F %2 %3").arg(
                            ini.value(QStringLiteral("X-Nemo-Application-Type"), QStringLiteral("silica-qt5")).toString(),
                            ini.fileName(),
                            ini.value(QStringLiteral("Exec")).toString()
                        );
        }

        QProcess::startDetached(runCmd);

        if (wait)
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    }

    file.close();

    if (wait)
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}
