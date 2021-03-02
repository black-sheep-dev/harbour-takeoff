#include "autostartmanager.h"

#include <QDebug>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProcess>
#include <QSettings>

#include "launcher.h"
#include "utils.h"

AutostartManager::AutostartManager(QObject *parent) :
    QObject(parent)
{
    readSettings();
}

AutostartManager::~AutostartManager()
{
    writeSettings();
    applyChanges();
}

AppListModel *AutostartManager::activeApps()
{
    return m_activeAppsModel;
}

AppListModel *AutostartManager::apps()
{
    return m_appsModel;
}

QString AutostartManager::desktopFileContent(const QString &fileName) const
{
    return Utils::readDesktopFileContent(fileName);
}

int AutostartManager::startDelay() const
{
    return m_startDelay;
}

void AutostartManager::refresh()
{
    cleanup();
    loadApps();
}

void AutostartManager::reset()
{
    refresh();
}

void AutostartManager::takeoff()
{
    Launcher::takeoff();
}

void AutostartManager::applyChanges()
{
    Utils::writeDefinition(m_appsModel->apps());
}

void AutostartManager::setStartDelay(int secs)
{
    if (m_startDelay == secs)
        return;

    m_startDelay = secs;
    emit startDelayChanged(m_startDelay);

    writeSettings();
}

void AutostartManager::onAutostartChanged()
{
    auto app = qobject_cast<App *>(sender());

    if (app == nullptr)
        return;

    if (app->autostart()) {
        m_activeAppsModel->addApp(app);
    } else {
        m_activeAppsModel->removeApp(app);
    }

    applyChanges();
}

void AutostartManager::cleanup()
{
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

    if (!binary.startsWith(QLatin1String("/usr/bin/"))) {
        binary.prepend(QStringLiteral("/usr/bin/"));
    }

    if (!QFile(binary).exists())
        return QString();

    // prepare start cmd
    QString start("/usr/bin/invoker -n -s ");

    const QStringList type = parts.filter(QLatin1String("--type="));

    if (type.isEmpty())
        start.append(QStringLiteral("--type=silica-qt5"));
    else
        start.append(type.first());

    start.append(" ");
    start.append(binary);

    return start;
}


void AutostartManager::loadApps()
{
    QDirIterator it(QStringLiteral("/usr/share/applications"),
                    QStringList() << QStringLiteral("*.desktop"),
                    QDir::Files,
                    QDirIterator::NoIteratorFlags);

    QList<App *> apps;

    while (it.hasNext()) {
        const QFileInfo info = QFileInfo(it.next());

        // read *.desktop file
        QSettings ini(info.filePath(), QSettings::IniFormat);

        ini.beginGroup(QStringLiteral("Desktop Entry"));

        const QString cmd = ini.value(QStringLiteral("Exec")).toString().simplified();

        if ( ini.value(QStringLiteral("Type")).toString() != QLatin1String("Application")
             || ini.value(QStringLiteral("NoDisplay"), false).toBool()
             || ini.contains(QStringLiteral("NotShowIn"))
             || cmd.isEmpty() )
            continue;

        // create app
        App *app = new App(this);

        app->setJailed(cmd.contains(QLatin1String("sailjail")));

        app->setDesktopFile(info.fileName());

        // set package name
        if (info.baseName().startsWith(QLatin1String("apkd_"))) {
            app->setPackageName(ini.value(QStringLiteral("X-apkd-packageName")).toString());
        } else {
            app->setPackageName(info.baseName());
        }

        // set icon
        QString icon = ini.value(QStringLiteral("Icon")).toString();

        if (!icon.startsWith(QLatin1Char('/')))
                icon.prepend(QStringLiteral("image://theme/"));

        app->setIcon(icon);

        // set name
        app->setName(ini.value(QStringLiteral("Name")).toString());

        ini.endGroup();

        apps.append(app);
    }

    m_appsModel->setApps(apps);


    // check if app is active
    const QStringList activeApps = Utils::readDefinition();

    QList<App *> active;
    active.reserve(activeApps.count());

    for (auto app: apps) {
        const int idx = activeApps.indexOf(app->desktopFile());

        if (idx < 0)
            continue;

        app->setAutostart(true);
        active.insert(idx, app);
    }

    m_activeAppsModel->setApps(active);

    // create connections
    for (auto app: apps) {
        connect(app, &App::autostartChanged, this, &AutostartManager::onAutostartChanged);
    }
}

void AutostartManager::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("STARTUP"));
    m_startDelay = settings.value(QStringLiteral("delay"), 0).toInt();
    settings.endGroup();
}

void AutostartManager::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("STARTUP"));
    settings.setValue(QStringLiteral("delay"), m_startDelay);
    settings.endGroup();
}
