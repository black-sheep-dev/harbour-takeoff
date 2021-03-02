#include "app.h"

App::App(QObject *parent) :
    QObject(parent)
{

}

bool App::autostart() const
{
    return m_autostart;
}

QString App::desktopFile() const
{
    return m_desktopFile;
}

QString App::icon() const
{
    return m_icon;
}

bool App::jailed() const
{
    return m_jailed;
}

QString App::name() const
{
    return m_name;
}

QString App::packageName() const
{
    return m_packageName;
}

void App::setAutostart(bool enabled)
{
    if (m_autostart == enabled)
        return;

    m_autostart = enabled;
    emit autostartChanged(m_autostart);
}

void App::setDesktopFile(const QString &file)
{
    if (m_desktopFile == file)
        return;

    m_desktopFile = file;
    emit desktopFileChanged(m_desktopFile);
}

void App::setIcon(const QString &icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged(m_icon);
}

void App::setJailed(bool jailed)
{
    if (m_jailed == jailed)
        return;

    m_jailed = jailed;
    emit jailedChanged(m_jailed);
}

void App::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void App::setPackageName(const QString &name)
{
    if (m_packageName == name)
        return;

    m_packageName = name;
    emit packageNameChanged(m_packageName);
}
