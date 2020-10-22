#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>

#include <QStandardPaths>
#include <QNetworkAccessManager>

#include "applibraryapi.h"
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
    Q_INVOKABLE QString activeAppsCount() const;
    Q_INVOKABLE AppListModel *apps();
    Q_INVOKABLE AppLibraryAPI *libraryAPI();

    bool apiActive() const;
    bool apiAutoUse() const;
    QString apiUrl() const;

    // properties
    int startDelay() const;

public slots:
    Q_INVOKABLE void execute(const QString &cmd);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void takeoff();

    Q_INVOKABLE void applyChanges();
    Q_INVOKABLE void onAutostartChanged(bool enabled);

    // properties
    void setStartDelay(int secs);

signals:
    // properties
    void startDelayChanged(int secs);

private slots:
    void onLibraryUpdate();

private:
    void cleanup();
    QString getStartCmd(const QString &cmd) const;
    void loadApps();

    void writeStartScript();

    void readCustomSettings();
    void writeCustomSettings();

    void readDefinitions();
    void writeDefinitions();

    void readSettings();
    void writeSettings();

    QStringList m_activeApps;
    AppListModel *m_activeAppsModel;
    AppListModel *m_appsModel;

    AppLibraryAPI *m_libraryAPI;

    // properties
    int m_startDelay{0};
};

#endif // AUTOSTARTMANAGER_H
