#include "applibraryapi.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>

AppLibraryAPI::AppLibraryAPI(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &AppLibraryAPI::onReply);

    readSettings();
    readLocalLibrary();

    if (m_active && m_autoUpdate)
        updateLibrary();
}

AppLibraryAPI::~AppLibraryAPI()
{
    writeSettings();
}

void AppLibraryAPI::resetSettings()
{
    setActive(false);
    setAutoUpdate(false);
    setAutoUse(false);
    setUrl(QStringLiteral(HARBOUR_TAKEOFF_DEFAULT_APP_LIBRARY_API_URL));

    writeSettings();
}

void AppLibraryAPI::saveSettings()
{
    writeSettings();

    if (m_active && m_autoUpdate)
        updateLibrary();
}

void AppLibraryAPI::updateLibrary()
{
    QUrl url(m_url);

    if (!url.isValid()) {
#ifdef QT_DEBUG
        qDebug() << "INVALID URL";
        qDebug() << url;
#endif
        return;
    }

    QNetworkRequest request(url);

    if (m_url.contains("https"))
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Accept-Encoding", "gzip");

    m_manager->get(request);
}

void AppLibraryAPI::updateApps(const QList<App *> &apps)
{
    for (App *app : apps) {
        app->setStartCmdLibrary(m_localLibrary.value(app->packageName()).toString(QString()));
    }
}

bool AppLibraryAPI::active() const
{
    return m_active;
}

bool AppLibraryAPI::autoUpdate() const
{
    return m_autoUpdate;
}

bool AppLibraryAPI::autoUse() const
{
    return m_autoUse;
}

QString AppLibraryAPI::url() const
{
    return m_url;
}

void AppLibraryAPI::setActive(bool enable)
{
    if (m_active == enable)
        return;

    m_active = enable;
    emit activeChanged(m_active);
}

void AppLibraryAPI::setAutoUpdate(bool enable)
{
    if (m_autoUpdate == enable)
        return;

    m_autoUpdate = enable;
    emit autoUpdateChanged(m_autoUpdate);
}

void AppLibraryAPI::setAutoUse(bool enable)
{
    if (m_autoUse == enable)
        return;

    m_autoUse = enable;
    emit autoUseChanged(m_autoUse);
}

void AppLibraryAPI::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged(m_url);
}

void AppLibraryAPI::onReply(QNetworkReply *reply)
{
    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->error();
#endif
        return;
    }

    const QByteArray data = reply->readAll();

#ifdef QT_DEBUG
    qDebug() << data;
#endif

    QJsonParseError error{};

    const QJsonObject library = QJsonDocument::fromJson(data, &error).object();

    if (error.error) {
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("JSON PARSE ERROR");
#endif
        return;
    }

    if (library.isEmpty())
        return;

    updateLocalLibrary(library);
}

void AppLibraryAPI::readLocalLibrary()
{
    QFile file;

    if (m_active)
        file.setFileName(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + "/app_library.json");
    else
        file.setFileName(":/files/library/harbour-takeoff-app-library.json");


    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QJsonParseError error{};

    const QJsonObject library = QJsonDocument::fromJson(file.readAll(), &error).object();

    file.close();

    if (error.error) {
#ifdef QT_DEBUG
    qDebug() << "JSON PARSE ERROR";
#endif
        return;
    }

    m_localLibraryVersion = quint16(library.value(QStringLiteral("library_version")).toInt());
    m_localLibrary = library.value(QStringLiteral("apps")).toObject();
}

void AppLibraryAPI::updateLocalLibrary(const QJsonObject &library)
{
    const quint16 version = quint16(library.value(QStringLiteral("library_version")).toInt());

    if (version <= m_localLibraryVersion) {
#ifdef QT_DEBUG
        qDebug() << "LIBRARY VERSION OLD/SAME";
#endif
        return;
    }

    QFile file(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + APP_TARGET + "/app_library.json");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

#ifdef QT_DEBUG
    out << QJsonDocument(library).toJson();
#else
    out << QJsonDocument(library).toJson(QJsonDocument::Compact);
#endif


    file.close();

    m_localLibrary = library.value(QStringLiteral("apps")).toObject();
    m_localLibraryVersion = version;

    emit libraryUpdated();
}

void AppLibraryAPI::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP_LIBRARY_API"));
    m_active = settings.value(QStringLiteral("active"), false).toBool();
    m_autoUpdate = settings.value(QStringLiteral("auto_update"), false).toBool();
    m_autoUse = settings.value(QStringLiteral("auto_use"), false).toBool();
    m_url = settings.value(QStringLiteral("url"), QStringLiteral(HARBOUR_TAKEOFF_DEFAULT_APP_LIBRARY_API_URL)).toString();
    settings.endGroup();
}

void AppLibraryAPI::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP_LIBRARY_API"));
    settings.setValue(QStringLiteral("active"), m_active);
    settings.setValue(QStringLiteral("auto_update"), m_autoUpdate);
    settings.setValue(QStringLiteral("auto_use"), m_autoUse);
    settings.setValue(QStringLiteral("url"), m_url);
    settings.endGroup();
}
