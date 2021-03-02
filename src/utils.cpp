#include "utils.h"

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

QString Utils::readDesktopFileContent(const QString &fileName)
{
    QFile file(QStringLiteral("/usr/share/applications/") + fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream in(&file);

    return in.readAll();
}

QStringList Utils::readDefinition()
{
    QStringList files;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/harbour-takeoff/takeoff.def"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return files;

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().simplified();

        // remove old definition file ---> app verison < 0.7
        if (line.startsWith(QLatin1Char('#'))) {
            file.close();
            file.remove();

            return files;
        }

        if (line.isEmpty())
            continue;

        files.append(line);
    }

    file.close();

    return files;
}

void Utils::writeDefinition(const QList<App *> &apps)
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/harbour-takeoff/takeoff.def"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (const auto app: apps) {
        if (!app->autostart())
            continue;

        out << app->desktopFile() << "\n";
    }
}
