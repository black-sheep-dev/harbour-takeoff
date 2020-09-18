#ifndef APPLISTSORTFILTERMODEL_H
#define APPLISTSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class AppListSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AppListSortFilterModel(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void sortModel();

    // QAbstractProxyModel interface
public:
    void setSourceModel(QAbstractItemModel *sourceModel) override;
};

#endif // APPLISTSORTFILTERMODEL_H
