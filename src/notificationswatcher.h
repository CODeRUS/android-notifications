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

class NotificationsWatcher : public QDBusVirtualObject
{
    Q_OBJECT
public:
    explicit NotificationsWatcher(QObject *parent = 0);

public slots:
    void start();

    QString introspect(const QString &path) const;
    bool handleMessage(const QDBusMessage &message, const QDBusConnection &connection);

private slots:
    void onViewDestroyed();
    void onViewClosing(QQuickCloseEvent*);

private:
    void showUI();
    void handleNotify(const QVariantList &arguments);

    QDBusInterface *notifIface;
    MGConfItem *dconf;
    QQuickView *view;
};

#endif // NOTIFICATIONSWATCHER_H
