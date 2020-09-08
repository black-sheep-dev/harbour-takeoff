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

# The name of your application
VERSION = 0.4.7

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET = harbour-takeoff

CONFIG += sailfishapp

systemd.files = $$PWD/systemd/org.nubecula.takeoff.service
systemd.path = /usr/lib/systemd/user

INSTALLS += systemd

SOURCES += src/harbour-takeoff.cpp \
    src/app.cpp \
    src/applistmodel.cpp \
    src/autostartmanager.cpp

DISTFILES += qml/harbour-takeoff.qml \
    icons/512x512/harbour-takeoff.png \
    icons/scaleable/harbour-takeoff.svg \
    qml/cover/CoverPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/ActiveAppsPage.qml \
    qml/pages/ManagerPage.qml \
    rpm/harbour-takeoff.changes \
    rpm/harbour-takeoff.changes.run.in \
    rpm/harbour-takeoff.spec \
    rpm/harbour-takeoff.yaml \
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
    translations/harbour-takeoff-zh_CN.ts

HEADERS += \
    src/app.h \
    src/applistmodel.h \
    src/autostartmanager.h

RESOURCES += \
    ressources.qrc
