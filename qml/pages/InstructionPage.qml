import QtQuick 2.1
import Sailfish.Silica 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("How it works")
            }

            Column {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
                spacing: Theme.paddingLarge

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    text: "Working mode"
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "In <b>Blacklist</b> mode notifications from blacklisted applications will not be modified. Notifications from other applications will be assigned to category according settings or default one."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "In <b>Whitelist</b> mode only notifications from whitelisted applications will be assigned to category according settings or default one. Notifications from other applications will ot be modified. If Whitelist mode active and whitelist have no applications it will do nothing."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    text: "Categories"
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "<b>Silent.</b> Notification will not produce any sound/vibration/led, but it will be visible on lockscreen."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "<b>Chat.</b> Notification will follow system Chat sound settings, and it will be also visible on lockscreen."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "<b>SMS.</b> Notification will follow system SMS sound settings, and it will be also visible on lockscreen."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "<b>Email.</b> Notification will follow system Email sound settings, and it will be also visible on lockscreen."
                }

                Label {
                    width: parent.width
                    wrapMode: Text.Wrap
                    text: "<b>Custom.</b> You may select own sound tone for this notification category, and it will be also visible on lockscreen."
                }
            }
        }
    }
}
