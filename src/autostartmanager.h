#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>

#include <QStandardPaths>

#include "applistmodel.h"

class AutostartManager : public QObject
{
    Q_OBJECT
public:
    explicit AutostartManager(QObject *parent = nullptr);
    ~AutostartManager();

    Q_INVOKABLE AppListModel *activeApps();
    Q_INVOKABLE QString activeAppsCount() const;
    Q_INVOKABLE AppListModel *apps();

public slots:
    Q_INVOKABLE void execute(const QString &cmd);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void takeoff();

    Q_INVOKABLE void applyChanges();
    Q_INVOKABLE void onAutostartChanged(bool enabled);

private:
    void cleanup();
    QString getStartCmd(const QString &cmd) const;
    void loadApps();
    void readDefinitions();
    void writeDefinitions();

    QStringList m_activeApps;
    QStringList m_startCmds;
    AppListModel *m_activeAppsModel;
    AppListModel *m_appsModel;
};

#endif // AUTOSTARTMANAGER_H
