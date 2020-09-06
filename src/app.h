#ifndef APP_H
#define APP_H

#include <QObject>

class App : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autostart READ autostart WRITE setAutostart NOTIFY autostartChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString startCmd READ startCmd WRITE setStartCmd NOTIFY startCmdChanged)

public:
    explicit App(QObject *parent = nullptr);

    bool autostart() const;
    QString icon() const;
    QString name() const;
    QString packageName() const;
    QString startCmd() const;

signals:
    void autostartChanged(bool enabled);
    void iconChanged(const QString &icon);
    void nameChanged(const QString &name);
    void packageNameChanged(const QString &name);
    void startCmdChanged(const QString &cmd);

public slots:
    void setAutostart(bool enabled = true);
    void setIcon(const QString &icon);
    void setName(const QString &name);
    void setPackageName(const QString &name);
    void setStartCmd(const QString &cmd);

private:
    bool m_autostart;
    QString m_icon;
    QString m_name;
    QString m_packageName;
    QString m_startCmd;
};

#endif // APP_H
