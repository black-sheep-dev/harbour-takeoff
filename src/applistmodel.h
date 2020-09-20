#ifndef APPLISTMODEL_H
#define APPLISTMODEL_H

#include <QAbstractListModel>

#include "app.h"

class AppListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AppRoles {
        AutostartRole      = Qt::UserRole + 1,
        IconRole,
        NameRole,
        PackageNameRole,
        StartCmdRole,
        StartCmdCustomRole,
        StartCmdLibraryRole,
        UseLibraryCmdRole
    };
    Q_ENUMS(AppRoles)

    explicit AppListModel(QObject *parent = nullptr);

    Q_INVOKABLE App *app(const int index);
    QList<App *> apps() const;

    // reorder function
    Q_INVOKABLE void moveDown(int index);
    Q_INVOKABLE void moveUp(int index);

signals:
    void changed();

public slots:
    void addApp(App *app);
    void removeApp(App *app);
    void setApps(const QList<App *> &apps);

    void reset();

private:
    QList<App *> m_apps;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

};

#endif // APPLISTMODEL_H
