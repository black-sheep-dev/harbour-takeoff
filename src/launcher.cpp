#include "launcher.h"

#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStandardPaths>

#include <thread>

Launcher::Launcher()
{

}

void Launcher::takeoff(bool wait)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().simplified();

        if (line.startsWith("###")) {
            QProcess::startDetached(in.readLine().simplified());

            if (wait)
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
    }

    file.close();

    if (wait)
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}
