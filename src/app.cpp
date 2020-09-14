#include "app.h"

App::App(QObject *parent) :
    QObject(parent),
    m_autostart(false),
    m_icon(QString()),
    m_name(QString()),
    m_packageName(QString()),
    m_startCmd(QString()),
    m_startCmdCustom(QString())
{

}

bool App::autostart() const
{
    return m_autostart;
}

QString App::icon() const
{
    return m_icon;
}

QString App::name() const
{
    return m_name;
}

QString App::packageName() const
{
    return m_packageName;
}

QString App::startCmd() const
{
    return m_startCmd;
}

QString App::startCmdCustom() const
{
    return m_startCmdCustom;
}

void App::setAutostart(bool enabled)
{
    if (m_autostart == enabled)
        return;

    m_autostart = enabled;
    emit autostartChanged(m_autostart);
}

void App::setIcon(const QString &icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged(m_icon);
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

void App::setStartCmd(const QString &cmd)
{
    if (m_startCmd == cmd)
        return;

    m_startCmd = cmd;
    emit startCmdChanged(m_startCmd);
}

void App::setStartCmdCustom(const QString &cmd)
{
    if (m_startCmdCustom == cmd)
        return;

    m_startCmdCustom = cmd;
    emit startCmdCustomChanged(m_startCmdCustom);
}

