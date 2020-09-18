#include "applistsortfiltermodel.h"

#include "applistmodel.h"

AppListSortFilterModel::AppListSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setSortRole(AppListModel::NameRole);
    setFilterRole(AppListModel::NameRole);
}

void AppListSortFilterModel::sortModel()
{
    sort(0, Qt::AscendingOrder);
}

void AppListSortFilterModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    AppListModel *model = qobject_cast<AppListModel *>(sourceModel);
    connect(model, &AppListModel::changed, this, &AppListSortFilterModel::sortModel);

    QSortFilterProxyModel::setSourceModel(sourceModel);

    sortModel();
}
