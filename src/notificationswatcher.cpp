#include "notificationswatcher.h"

#include "lipsticknotification.h"
#include "notification.h"

#include "sailfishapp.h"

#include <QGuiApplication>

#include <QDebug>

NotificationsWatcher::NotificationsWatcher(QObject *parent) :
    QObject(parent)
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

        QDBusConnection::sessionBus().registerObject("/org/freedesktop/Notifications", this, QDBusConnection::ExportScriptableSlots);
        QString matchString = "interface='org.freedesktop.Notifications',member='Notify',type='method_call',eavesdrop='true'";
        QDBusInterface busInterface("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus");
        busInterface.call("AddMatch", matchString);
    }
}

void NotificationsWatcher::showUI(const QStringList &)
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

void NotificationsWatcher::Notify(const QString &app_name, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantHash &hints, int)
{
    QString androidAppName("AndroidNotification");

    if (app_name == androidAppName && !hints.contains(LipstickNotification::HINT_PRIORITY)) {
        QDBusReply<NotificationList> reply = notifIface->call("GetNotifications", app_name);
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
