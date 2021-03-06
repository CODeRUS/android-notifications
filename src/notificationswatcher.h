#ifndef NOTIFICATIONSWATCHER_H
#define NOTIFICATIONSWATCHER_H

#include <QObject>

#include <QQuickView>

#include "mdconfagent.h"

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
    virtual ~NotificationsWatcher();

public slots:
    void start();

    QString introspect(const QString &path) const;
    bool handleMessage(const QDBusMessage &message, const QDBusConnection &connection);

    bool handleRawMessage(DBusMessage *msg);

private slots:
    void onViewDestroyed();
    void onViewClosing(QQuickCloseEvent*);

    void timerTimeout();

private:
    void showUI();
    QVariant parse(const QDBusArgument &argument);
    bool handleNotify(const QVariantList &arguments);

    QHash<int, QVariantList> pendingSerials;
    QHash<uint, QVariantList> pendingNotifications;
    QHash<QObject*, uint> signalTimers;

    QDBusInterface *notifIface;
    MDConfAgent *dconf;
    QQuickView *view;
};

#endif // NOTIFICATIONSWATCHER_H
