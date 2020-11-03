#include "applistmodel.h"

AppListModel::AppListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

App *AppListModel::app(const int index)
{
    if (index >= m_apps.count())
        return nullptr;

    return m_apps.at(index);
}

QList<App *> AppListModel::apps() const
{
    return m_apps;
}

void AppListModel::moveDown(int index)
{
    if ((index + 1) >= m_apps.count())
        return;

    beginResetModel();
    m_apps.move(index, index + 1);
    endResetModel();

    emit changed();
}

void AppListModel::moveUp(int index)
{
    if (index <= 0)
        return;

    beginResetModel();
    m_apps.move(index, index - 1);
    endResetModel();

    emit changed();
}

void AppListModel::addApp(App *app)
{
    if (m_apps.contains(app))
        return;

    beginInsertRows(QModelIndex(), m_apps.count(), m_apps.count());
    m_apps.append(app);
    endInsertRows();
}

void AppListModel::removeApp(int index)
{
    if (index < 0 || index >= m_apps.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_apps.takeAt(index);
    endRemoveRows();
}

void AppListModel::removeApp(App *app)
{
    const int index = m_apps.indexOf(app);

    if (index < 0)
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_apps.takeAt(index);
    endRemoveRows();
}

void AppListModel::setApps(const QList<App *> &apps)
{
    beginResetModel();
    m_apps.clear();
    m_apps = apps;
    endResetModel();
}

void AppListModel::reset()
{
    beginResetModel();
    m_apps.clear();
    endResetModel();
}

int AppListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_apps.count();
}

QVariant AppListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    App *app = m_apps.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return app->name();

    case AutostartRole:
        return app->autostart();

    case DesktopFileData:
        return app->desktopFileData();

    case IconRole:
        return app->icon();

    case NameRole:
        return app->name();

    case PackageNameRole:
        return app->packageName();

    case StartCmdRole:
        return app->startCmd();

    case StartCmdCustomRole:
        return app->startCmdCustom();

    case StartCmdLibraryRole:
        return app->startCmdLibrary();

    case UseLibraryCmdRole:
        return app->useLibraryStartCmd();

    default:
        return QVariant();
    }
}

bool AppListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    App *app = m_apps.at(index.row());

    switch (role) {

    case AutostartRole:
        app->setAutostart(value.toBool());
        break;

    case StartCmdRole:
        app->setStartCmd(value.toString());
        break;

    case StartCmdCustomRole:
        app->setStartCmdCustom((value.toString()));
        break;

    default:
        break;
    }

    emit dataChanged(index, index);

    return true;
}

QHash<int, QByteArray> AppListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[AutostartRole]        = "autostart";
    roles[DesktopFileData]      = "desktop_file_data";
    roles[IconRole]             = "icon";
    roles[NameRole]             = "name";
    roles[PackageNameRole]      = "package_name";
    roles[StartCmdRole]         = "start_cmd";
    roles[StartCmdCustomRole]   = "start_cmd_custom";
    roles[StartCmdLibraryRole]  = "start_cmd_library";
    roles[UseLibraryCmdRole]    = "use_library_cmd";

    return roles;
}
