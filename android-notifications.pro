TARGET = android-notifications

QT += dbus
CONFIG += sailfishapp link_pkgconfig
PKGCONFIG += sailfishapp mlite5

INCLUDEPATH += /usr/include/mlite5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += \
    src/main.cpp \
    src/notificationswatcher.cpp \
    src/lipsticknotification.cpp \
    src/notification.cpp \
    src/notificationmanagerproxy.cpp

HEADERS += \
    src/notificationswatcher.h \
    src/lipsticknotification.h \
    src/notification.h \
    src/notificationmanagerproxy.h


dbus.files = dbus/org.coderus.androidnotifications.service
dbus.path = /usr/share/dbus-1/services

systemd.files = systemd/android-notifications.service
systemd.path = /usr/lib/systemd/user

INSTALLS += dbus systemd

OTHER_FILES += \
    rpm/android-notifications.spec \
    android-notifications.desktop \
    qml/main.qml \
    qml/cover/CoverPage.qml \
    qml/pages/MainPage.qml

