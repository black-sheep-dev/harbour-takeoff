# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# VERSION
VERSION = 0.7.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# The name of your application
TARGET = harbour-takeoff
DEFINES += APP_TARGET=\\\"$$TARGET\\\"

CONFIG += sailfishapp

systemd.files = $$PWD/systemd/org.nubecula.takeoff.service
systemd.path = /usr/lib/systemd/user

startup.files = $$PWD/systemd/harbour-takeoff-startup.sh
startup.path = /usr/bin/

INSTALLS += systemd startup

SOURCES += src/harbour-takeoff.cpp \
    src/app.cpp \
    src/applistmodel.cpp \
    src/applistsortfiltermodel.cpp \
    src/autostartmanager.cpp \
    src/launcher.cpp \
    src/utils.cpp

DISTFILES += qml/harbour-takeoff.qml \
    api/harbour-takeoff-app-library.json \
    icons/512x512/harbour-takeoff.png \
    icons/scaleable/harbour-takeoff.svg \
    qml/cover/CoverPage.qml \
    qml/dialogs/EditAppDialog.qml \
    qml/pages/AboutPage.qml \
    qml/pages/ActiveAppsPage.qml \
    qml/pages/DesktopFileViewerPage.qml \
    qml/pages/ManagerPage.qml \
    qml/pages/SettingsPage.qml \
    rpm/harbour-takeoff.changes \
    rpm/harbour-takeoff.changes.run.in \
    rpm/harbour-takeoff.spec \
    rpm/harbour-takeoff.yaml \
    systemd/harbour-takeoff-startup.sh \
    systemd/org.nubecula.takeoff.service \
    translations/*.ts \
    harbour-takeoff.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172 512x512

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += \
    translations/harbour-takeoff-de.ts \
    translations/harbour-takeoff-sv.ts \
    translations/harbour-takeoff-zh_CN.ts

HEADERS += \
    src/app.h \
    src/applistmodel.h \
    src/applistsortfiltermodel.h \
    src/autostartmanager.h \
    src/launcher.h \
    src/utils.h

RESOURCES += \
    ressources.qrc
