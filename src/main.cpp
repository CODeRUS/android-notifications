#include <sailfishapp.h>
#include <QGuiApplication>
#include <QTimer>

#include "notificationswatcher.h"

#include <grp.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    setuid(getpwnam("nemo")->pw_uid);
    setgid(getgrnam("privileged")->gr_gid);

    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationDisplayName("Android notifications");
    app->setApplicationName("Android notifications");
    app->setApplicationVersion(QString(APP_VERSION));
    app->setQuitOnLastWindowClosed(false);
    QScopedPointer<NotificationsWatcher> watcher(new NotificationsWatcher(app.data()));
    QTimer::singleShot(10, watcher.data(), SLOT(start()));

    return app->exec();
}

