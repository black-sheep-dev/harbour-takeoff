#include "autostartmanager.h"

#include <QDebug>
#include <QDirIterator>
#include <QSettings>
#include <QTextStream>

AutostartManager::AutostartManager(QObject *parent) :
    QObject(parent),
    m_activeAppsModel(new AppListModel(this)),
    m_appsModel(new AppListModel(this))
{
    connect(m_activeAppsModel, &AppListModel::changed, this, &AutostartManager::applyChanges);
}

AppListModel *AutostartManager::activeApps()
{
    return m_activeAppsModel;
}

QString AutostartManager::activeAppsCount() const
{
    return QString::number(m_activeAppsModel->apps().count());
}

AppListModel *AutostartManager::apps()
{
    return m_appsModel;
}

void AutostartManager::refresh()
{
    cleanup();
    readScript();
    loadApps();
}

void AutostartManager::reset()
{
    m_activeAppsModel->reset();
    writeScript();

    refresh();
}

void AutostartManager::applyChanges()
{
    writeScript();
}

void AutostartManager::onAutostartChanged(bool enabled)
{
    App *app = qobject_cast<App *>(sender());

    if (!app)
        return;

    if (enabled) {
        m_activeApps.append(app->packageName());
        m_activeAppsModel->addApp(app);
    } else {
        m_activeApps.removeAll(app->packageName());
        m_activeAppsModel->removeApp(app);
    }

    writeScript();
}

void AutostartManager::cleanup()
{
    m_activeAppsModel->reset();

    const QList<App *> apps = m_appsModel->apps();

    m_appsModel->reset();

    qDeleteAll(apps.begin(), apps.end());
}

QString AutostartManager::getStartCmd(const QString &cmd) const
{
    const QStringList parts = cmd.split(" ");

    if (parts.isEmpty())
        return QString();

    // check if binary exists
    QString binary = parts.last();

    if (!binary.startsWith(QStringLiteral("/usr/bin/"))) {
        binary.prepend("/usr/bin/");
    }

    if (!QFile(binary).exists())
        return QString();

    // prepare start cmd
    QString start("/usr/bin/invoker -n -s ");

    const QStringList type = parts.filter("--type=");

    if (type.isEmpty())
        start.append("--type=silica-qt5");
    else
        start.append(type.first());

    start.append(" ");
    start.append(binary);

    return start;
}


void AutostartManager::loadApps()
{
    QDirIterator it(QStringLiteral("/usr/share/applications"),
                    QStringList() << "*.desktop",
                    QDir::Files,
                    QDirIterator::NoIteratorFlags);

    while (it.hasNext()) {
        QFile file(it.next());

        // remove  apps
        if (QFileInfo(file).baseName().startsWith("apkd_"))
            continue;

        // read *.desktop file
        QSettings ini(file.fileName(), QSettings::IniFormat);

        ini.beginGroup(QStringLiteral("Desktop Entry"));

        const QString cmd = ini.value(QStringLiteral("Exec")).toString().simplified();

        if ( ini.value(QStringLiteral("Type")).toString() != "Application"
             || ini.value(QStringLiteral("NoDisplay"), false).toBool()
             || ini.contains(QStringLiteral("NotShowIn"))
             || cmd.isEmpty() )
            continue;

        // create app
        App *app = new App(this);

        app->setIcon(ini.value(QStringLiteral("Icon")).toString());
        app->setPackageName(QFileInfo(file).baseName());
        app->setName(ini.value(QStringLiteral("Name")).toString());

        app->setStartCmd(cmd);

        // check if app is active
        if (m_activeApps.contains(app->packageName())) {
              app->setAutostart(true);
              m_activeAppsModel->addApp(app);
        }

        connect(app, &App::autostartChanged, this, &AutostartManager::onAutostartChanged);

        m_appsModel->addApp(app);

        ini.endGroup();
    }
}

void AutostartManager::readScript()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_activeApps.clear();

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().simplified();

        if (line.startsWith("###")) {
            m_activeApps.append(line.mid(3, line.length() - 3));
        }
    }

    file.close();
}

void AutostartManager::writeScript()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (const App *app: m_activeAppsModel->apps()) {
        out << "###" << app->packageName() << "\n";
        out << app->startCmd() << "\n";
    }

    file.close();
}
