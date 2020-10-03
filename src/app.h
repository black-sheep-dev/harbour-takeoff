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
    Q_PROPERTY(QString startCmdCustom READ startCmdCustom WRITE setStartCmdCustom NOTIFY startCmdCustomChanged)
    Q_PROPERTY(QString startCmdLibrary READ startCmdLibrary WRITE setStartCmdLibrary NOTIFY startCmdLibraryChanged)
    Q_PROPERTY(bool useLibraryStartCmd READ useLibraryStartCmd WRITE setUseLibraryStartCmd NOTIFY useLibraryStartCmdChanged)

public:
    explicit App(QObject *parent = nullptr);

    bool autostart() const;
    QString icon() const;
    QString name() const;
    QString packageName() const;
    QString startCmd() const;
    QString startCmdCustom() const;
    QString startCmdLibrary() const;
    bool useLibraryStartCmd() const;

signals:
    void autostartChanged(bool enabled);
    void iconChanged(const QString &icon);
    void nameChanged(const QString &name);
    void packageNameChanged(const QString &name);
    void startCmdChanged(const QString &cmd);
    void startCmdCustomChanged(const QString &cmd);
    void startCmdLibraryChanged(const QString &cmd);
    void useLibraryStartCmdChanged(bool enabled);

public slots:
    void setAutostart(bool enabled = true);
    void setIcon(const QString &icon);
    void setName(const QString &name);
    void setPackageName(const QString &name);
    void setStartCmd(const QString &cmd);
    void setStartCmdCustom(const QString &cmd);
    void setStartCmdLibrary(const QString &cmd);
    void setUseLibraryStartCmd(bool enable);

private:
    bool m_autostart{false};
    QString m_icon;
    QString m_name;
    QString m_packageName;
    QString m_startCmd;
    QString m_startCmdCustom;
    QString m_startCmdLibrary;
    bool m_useLibraryStartCmd{false};
};

#endif // APP_H
