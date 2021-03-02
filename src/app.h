#ifndef APP_H
#define APP_H

#include <QObject>

class App : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autostart READ autostart WRITE setAutostart NOTIFY autostartChanged)
    Q_PROPERTY(QString desktopFile READ desktopFile WRITE setDesktopFile NOTIFY desktopFileChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(bool jailed READ jailed WRITE setJailed NOTIFY jailedChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName NOTIFY packageNameChanged)

public:
    explicit App(QObject *parent = nullptr);

    bool autostart() const;
    QString desktopFile() const;
    QString icon() const;
    bool jailed() const;
    QString name() const;
    QString packageName() const;

signals:
    void autostartChanged(bool enabled);
    void desktopFileChanged(const QString &file);
    void iconChanged(const QString &icon);
    void jailedChanged(bool jailed);
    void nameChanged(const QString &name);
    void packageNameChanged(const QString &name);

public slots:
    void setAutostart(bool enabled = true);
    void setDesktopFile(const QString &file);
    void setIcon(const QString &icon);
    void setJailed(bool jailed);
    void setName(const QString &name);
    void setPackageName(const QString &name);

private:
    bool m_autostart{false};
    QString m_desktopFile;
    QString m_icon;
    bool m_jailed{false};
    QString m_name;
    QString m_packageName;

};

#endif // APP_H
