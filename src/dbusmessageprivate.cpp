#include <QtDBus/private/qdbusmessage_p.h>

DBusMessage *QDBusMessagePrivate::toDBusMessage(const QDBusMessage &message, QDBusConnection::ConnectionCapabilities capabilities,
                                                QDBusError *error) {
    Q_UNUSED(capabilities)
    Q_UNUSED(error)
    return message.d_ptr->msg;
}
