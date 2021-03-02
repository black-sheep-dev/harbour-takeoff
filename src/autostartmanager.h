#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>

#include <QStandardPaths>
#include <QNetworkAccessManager>

#include "applistmodel.h"
#include "applistsortfiltermodel.h"

class AutostartManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int startDelay READ startDelay WRITE setStartDelay NOTIFY startDelayChanged)

public:
    explicit AutostartManager(QObject *parent = nullptr);
    ~AutostartManager() override;

    Q_INVOKABLE AppListModel *activeApps();
    Q_INVOKABLE AppListModel *apps();
    Q_INVOKABLE QString desktopFileContent(const QString &fileName) const;

    bool apiActive() const;
    bool apiAutoUse() const;
    QString apiUrl() const;

    // properties
    int startDelay() const;

public slots:
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void takeoff();

    Q_INVOKABLE void applyChanges();

    // properties
    void setStartDelay(int secs);

signals:
    // properties
    void startDelayChanged(int secs);

private slots:
    void onAutostartChanged();

private:
    void cleanup();
    QString getStartCmd(const QString &cmd) const;
    void loadApps();

    void readSettings();
    void writeSettings();

    AppListModel *m_activeAppsModel{new AppListModel(this)};
    AppListModel *m_appsModel{new AppListModel(this)};

    // properties
    int m_startDelay{0};
};

#endif // AUTOSTARTMANAGER_H
