#ifndef APPLIBRARYAPI_H
#define APPLIBRARYAPI_H

#define     HARBOUR_TAKEOFF_DEFAULT_APP_LIBRARY_API_URL         "https://static-api.nubecula.org/harbour-takeoff-app-library.json"

#include <QObject>

#include <QJsonObject>
#include <QNetworkAccessManager>

#include "app.h"

class AppLibraryAPI : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(bool autoUse READ autoUse WRITE setAutoUse NOTIFY autoUseChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit AppLibraryAPI(QObject *parent = nullptr);
    ~AppLibraryAPI() override;

    Q_INVOKABLE void resetSettings();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE void updateLibrary();

    void updateApps(const QList<App *> &apps);

    // properties
    bool active() const;
    bool autoUpdate() const;
    bool autoUse() const;
    QString url() const;

signals:
    void libraryUpdated();

    // properties
    void activeChanged(bool enabled);
    void autoUseChanged(bool enabled);
    void urlChanged(const QString &url);
    void autoUpdateChanged(bool enabled);

public slots:
    // properties;
    void setActive(bool enable);
    void setAutoUpdate(bool enable);
    void setAutoUse(bool enable);
    void setUrl(const QString &url);

private slots:
    void onReply(QNetworkReply *reply);

private:
    void readLocalLibrary();
    void updateLocalLibrary(const QJsonObject &library);

    void readSettings();
    void writeSettings();

    QJsonObject m_localLibrary;
    quint16 m_localLibraryVersion{0};
    QNetworkAccessManager *m_manager{nullptr};

    // properties
    bool m_active{false};
    bool m_autoUpdate{false};
    bool m_autoUse{false};
    QString m_url;
};

#endif // APPLIBRARYAPI_H
