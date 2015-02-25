#include "notificationswatcher.h"

#include "lipsticknotification.h"
#include "notification.h"

#include "sailfishapp.h"

#include <QGuiApplication>

#include <QDebug>

NotificationsWatcher::NotificationsWatcher(QObject *parent) :
    QDBusVirtualObject(parent)
{
    qDBusRegisterMetaType<QVariantHash>();
    qDBusRegisterMetaType<LipstickNotification>();
    qDBusRegisterMetaType<NotificationList>();

    notifIface = new QDBusInterface("org.freedesktop.Notifications",
                                    "/org/freedesktop/Notifications",
                                    "org.freedesktop.Notifications",
                                    QDBusConnection::sessionBus(), this);
    dconf = new MGConfItem("/apps/android-notifications", this);
    dconf->set(0);

    view = NULL;
}

void NotificationsWatcher::start()
{
    bool ready = QDBusConnection::sessionBus().registerService("org.coderus.androidnotifications");

    if (!ready) {
        qWarning() << "Service already registered, exiting...";
        QGuiApplication::quit();
        return;
    }
    else {
        qDebug() << "Service registered successfully!";

        QDBusConnection::sessionBus().registerVirtualObject("/", this, QDBusConnection::SubPath);

        QDBusInterface busInterface("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");
        busInterface.call(QDBus::NoBlock, "AddMatch", "eavesdrop='true',type='method_call',interface='org.freedesktop.Notifications',member='Notify'");
    }
}

QString NotificationsWatcher::introspect(const QString &path) const
{
    if (path == "/") {
        return "<node name=\"org\"/>";
    }
    else if (path == "/org") {
        return "<node name=\"coderus\"/>";
    }
    else if (path == "/org/coderus") {
        return "<node name=\"androidnotifications\"/>";
    }
    else if (path == "/org/coderus/androidnotifications") {
        return "<interface name=\"org.coderus.androidnotifications\">\
                    <method name=\"showUI\">\
                        <arg direction=\"in\" type=\"as\" name=\"params\"/>\
                        <annotation name=\"org.freedesktop.DBus.Method.NoReply\" value=\"true\"/>\
                    </method>\
                </interface>";
    }

    return "";
}

bool NotificationsWatcher::handleMessage(const QDBusMessage &message, const QDBusConnection &connection)
{
    const QVariantList dbusArguments = message.arguments();
    const QString member = message.member();
    const QString interface = message.interface();

    if (interface == "org.freedesktop.Notifications" && member == "Notify") {
        handleNotify(dbusArguments);

        return true;
    }
    else if (interface == "org.coderus.androidnotifications" && member == "showUI") {
        QDBusMessage reply = message.createReply();
        connection.call(reply, QDBus::NoBlock);

        showUI();

        return true;
    }

    return false;
}

void NotificationsWatcher::showUI()
{
    if (!view) {
        qDebug() << "Construct view";
        view = SailfishApp::createView();
        QObject::connect(view, SIGNAL(destroyed()), this, SLOT(onViewDestroyed()));
        QObject::connect(view, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(onViewClosing(QQuickCloseEvent*)));
        view->setTitle("Android notifications");
        view->setSource(SailfishApp::pathTo("qml/main.qml"));
        view->showFullScreen();
    }
    else if (view->windowState() == Qt::WindowNoState) {
        qDebug() << "Create view";
        view->create();
        view->showFullScreen();
    }
    else {
        qDebug() << "Show view";
        view->raise();
        view->requestActivate();
    }
}

void NotificationsWatcher::handleNotify(const QVariantList &arguments)
{
    QString androidAppName("AndroidNotification");
    QString appName = arguments.value(0).toString();
    QString appIcon = arguments.value(2).toString();
    QString body = arguments.value(4).toString();
    QVariantHash hints = arguments.value(6).toHash();

    if (appName == androidAppName && !body.isEmpty() && !appIcon.isEmpty() && !hints.contains(LipstickNotification::HINT_PRIORITY)) {
        QDBusReply<NotificationList> reply = notifIface->call("GetNotifications", appName);
        if (reply.isValid()) {
            NotificationList notifications = reply.value();
            foreach (LipstickNotification *lipsticknotification, notifications.notifications()) {
                if (lipsticknotification->appName() == androidAppName && !lipsticknotification->hints().contains(LipstickNotification::HINT_PRIORITY)) {
                    qDebug() << "Faking sound";
                    Notification dummy;
                    dummy.setReplacesId(dconf->value(0).toInt());
                    dummy.close();
                    dummy.setHintValue(LipstickNotification::HINT_FEEDBACK, QString("chat"));
                    dummy.publish();
                    dconf->set(dummy.replacesId());

                    qDebug() << "Replacing notification" << lipsticknotification->replacesId();
                    Notification notification;
                    notification.setAppName(androidAppName);
                    notification.setSummary(lipsticknotification->summary());
                    notification.setBody(lipsticknotification->body());
                    notification.setPreviewSummary(lipsticknotification->hints().value(LipstickNotification::HINT_PREVIEW_SUMMARY).toString());
                    notification.setPreviewBody(lipsticknotification->hints().value(LipstickNotification::HINT_PREVIEW_BODY).toString());
                    notification.setHintValue(LipstickNotification::HINT_PREVIEW_ICON, lipsticknotification->hints().value(LipstickNotification::HINT_PREVIEW_ICON).toString());
                    notification.setHintValue(LipstickNotification::HINT_ICON, lipsticknotification->hints().value(LipstickNotification::HINT_PREVIEW_ICON).toString());
                    notification.setHintValue(LipstickNotification::HINT_PRIORITY, QString("100"));
                    notification.setHintValue(LipstickNotification::HINT_FEEDBACK, QString("chat"));
                    notification.setReplacesId(lipsticknotification->replacesId());
                    notification.publish();
                }
            }
        }
        else {
            qWarning() << reply.error().message();
        }
    }
}

void NotificationsWatcher::onViewDestroyed()
{
    qDebug() << "Window destroyed";
    view = NULL;
}

void NotificationsWatcher::onViewClosing(QQuickCloseEvent *)
{
    qDebug() << "Window closed";
    delete view;
}
