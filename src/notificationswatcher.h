#ifndef NOTIFICATIONSWATCHER_H
#define NOTIFICATIONSWATCHER_H

#include <QObject>

#include <QQuickView>

#include <MGConfItem>

#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusMetaType>

class NotificationsWatcher : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Notifications")
public:
    explicit NotificationsWatcher(QObject *parent = 0);

public slots:
    void start();

    Q_SCRIPTABLE Q_NOREPLY void showUI(const QStringList&);
    Q_SCRIPTABLE void Notify(const QString &app_name, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantHash &hints, int);

private slots:
    void onViewDestroyed();
    void onViewClosing(QQuickCloseEvent*);

private:
    QDBusInterface *notifIface;
    MGConfItem *dconf;
    QQuickView *view;
};

#endif // NOTIFICATIONSWATCHER_H
