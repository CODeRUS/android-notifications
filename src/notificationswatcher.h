#ifndef NOTIFICATIONSWATCHER_H
#define NOTIFICATIONSWATCHER_H

#include <QObject>

#include <QQuickView>

#include <MGConfItem>

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusVirtualObject>
#include <QDBusMetaType>
#include <QDBusMessage>

#include <dbus/dbus.h>

class NotificationsWatcher : public QDBusVirtualObject
{
    Q_OBJECT
public:
    explicit NotificationsWatcher(QObject *parent = 0);

public slots:
    void start();

    QString introspect(const QString &path) const;
    bool handleMessage(const QDBusMessage &message, const QDBusConnection &connection);

    bool handleRawMessage(DBusMessage *msg);

private slots:
    void onViewDestroyed();
    void onViewClosing(QQuickCloseEvent*);

    void timerTimeout();
    void handleNotification(uint id);

private:
    void showUI();
    QVariant parse(const QDBusArgument &argument);
    bool handleNotify(const QVariantList &arguments);

    QList<int> pendingSerials;
    QHash<QObject*, uint> signalTimers;

    QDBusInterface *notifIface;
    MGConfItem *dconf;
    QQuickView *view;
};

#endif // NOTIFICATIONSWATCHER_H
