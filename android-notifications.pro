TARGET = android-notifications

QT += dbus dbus-private sql
CONFIG += link_pkgconfig sailfishapp
PKGCONFIG += mlite5 dbus-1 dconf

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    src/main.cpp \
    src/notificationswatcher.cpp \
    src/lipsticknotification.cpp \
    src/notification.cpp \
    src/notificationmanagerproxy.cpp \
    src/dbusmessageprivate.cpp \
    src/desktopfilemodel.cpp \
    src/desktopfilemodelplugin.cpp \
    src/desktopfilesortmodel.cpp \
    src/mdconf.cpp \
    src/mdconfagent.cpp

HEADERS += \
    src/notificationswatcher.h \
    src/lipsticknotification.h \
    src/notification.h \
    src/notificationmanagerproxy.h \
    src/desktopfilemodel.h \
    src/desktopfilemodelplugin.h \
    src/desktopfilesortmodel.h \
    src/mdconf_p.h \
    src/mdconfagent.h

dbus.files = dbus/org.coderus.androidnotifications.service
dbus.path = /usr/share/dbus-1/services

systemd.files = systemd/android-notifications.service
systemd.path = /usr/lib/systemd/user

events.files = events/android_notify.ini
events.path = /usr/share/ngfd/events.d/

profiled.files = profiled/60.android-notifications.ini
profiled.path = /etc/profiled

INSTALLS += dbus systemd events profiled

OTHER_FILES += \
    rpm/android-notifications.spec \
    android-notifications.desktop \
    qml/main.qml \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/SelectionPage.qml \
    qml/pages/AboutPage.qml \
    qml/pages/InstructionPage.qml
