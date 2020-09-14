#include "autostartmanager.h"

#include <QDebug>
#include <QDirIterator>
#include <QProcess>
#include <QSettings>
#include <QTextStream>

#include "launcher.h"

AutostartManager::AutostartManager(QObject *parent) :
    QObject(parent),
    m_activeAppsModel(new AppListModel(this)),
    m_appsModel(new AppListModel(this))
{
    connect(m_activeAppsModel, &AppListModel::changed, this, &AutostartManager::applyChanges);
}

AutostartManager::~AutostartManager()
{
    writeDefinitions();
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

void AutostartManager::execute(const QString &cmd)
{
    QProcess::startDetached(cmd);
}

void AutostartManager::refresh()
{
    cleanup();
    readDefinitions();
    loadApps();
}

void AutostartManager::reset()
{
    m_activeAppsModel->reset();
    writeDefinitions();

    refresh();
}

void AutostartManager::takeoff()
{
    Launcher::takeoff();
}

void AutostartManager::applyChanges()
{
    writeDefinitions();
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

    writeDefinitions();
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

        m_appsModel->addApp(app);

        ini.endGroup();
    }

    // check if app is active
    for (App *app: m_appsModel->apps()) {
        const int idx = m_activeApps.indexOf(app->packageName());

        if (idx < 0)
            continue;

        if (m_startCmds.at(idx) != app->startCmd())
            app->setStartCmdCustom(m_startCmds.at(idx));

        app->setAutostart(true);
        m_activeAppsModel->addApp(app);
    }

    // create connections
    for (App *app: m_appsModel->apps()) {
        connect(app, &App::autostartChanged, this, &AutostartManager::onAutostartChanged);
    }
}

void AutostartManager::readDefinitions()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_activeApps.clear();
    m_startCmds.clear();

    QTextStream in(&file);

    while (!in.atEnd()) {
        const QString line = in.readLine().simplified();

        if (line.startsWith("###")) {
            m_activeApps.append(line.mid(3, line.length() - 3));
            m_startCmds.append(in.readLine().simplified());
        }
    }

    file.close();
}

void AutostartManager::writeDefinitions()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/harbour-takeoff/takeoff.def");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    for (const App *app: m_activeAppsModel->apps()) {
        out << "###" << app->packageName() << "\n";

        if (!app->startCmdCustom().isEmpty())
            out << app->startCmdCustom();
        else
            out << app->startCmd();

        out << "\n";
    }

    file.close();
}
