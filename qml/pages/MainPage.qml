import QtQuick 2.1
import Sailfish.Silica 1.0
import org.nemomobile.configuration 1.0
import org.nemomobile.dbus 2.0
import Sailfish.Media 1.0
import com.jolla.settings.system 1.0
import org.nemomobile.systemsettings 1.0

Page {
    id: page

    property var categories: ["silent", "chat", "sms", "email", "android_notify"]
    property var categoryNames: ["Silent", "Chat", "SMS", "Email", "Custom"]

    ConfigurationGroup {
        id: config
        path: "/apps/android-notifications"
        property int mode: 0
        property var whitelist: []
        property var blacklist: []
        property string defaultCategory: "android_notify"
        property var category_silent: []
        property var category_chat: []
        property var category_sms: []
        property var category_email: []
        property var category_android_notify: []
    }

    DBusInterface {
        id: profiled
        service: "com.nokia.profiled"
        path: "/com/nokia/profiled"
        iface: "com.nokia.profiled"

        property string profileName: "general"
        property string notificationToneKey: "android.notification.tone"
        property string notificationTone: ""

        Component.onCompleted: {
            profiled.typedCall("get_value",
                               [
                                   {"type": "s", "value": profileName},
                                   {"type": "s", "value": notificationToneKey}
                               ],
                               function (result) {
                                   notificationTone = result;
                               });
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: "About"
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
            MenuItem {
                text: "How it works"
                onClicked: pageStack.push(Qt.resolvedUrl("InstructionPage.qml"))
            }
        }

        Column {
            id: column
            width: page.width

            PageHeader {
                title: qsTr("Android notifications")
            }

            ComboBox {
                id: modeCombo
                width: parent.width
                label: "Working mode:"
                currentIndex: config.mode
                menu: ContextMenu {
                    MenuItem {
                        text: "Whitelist"
                        onClicked: config.mode = 0
                    }
                    MenuItem {
                        text: "Blacklist"
                        onClicked: config.mode = 1
                    }
                }
            }

            ValueButton {
                width: parent.width
                label: "Items in list:"
                value: config.mode == 0 ? config.whitelist.length : config.blacklist.length
                description: "Click to edit %1".arg(config.mode == 0 ? "whitelist" : "blacklist")
                onClicked: {
                    var selector = pageStack.push(Qt.resolvedUrl("SelectionPage.qml"), {selectedValues: config.mode == 0 ? config.whitelist : config.blacklist})
                    selector.accepted.connect(function() {
                        var selected = selector.selectedValues
                        if (config.mode == 0) {
                            config.whitelist = selected
                        }
                        else {
                            config.blacklist = selected
                        }
                    })
                }
            }

            ComboBox {
                id: defaultCombo
                width: parent.width
                label: "Default category:"
                menu: ContextMenu {
                    Repeater {
                        width: parent.width
                        model: categories.length
                        delegate: MenuItem {
                            text: categoryNames[index]
                            onClicked: config.defaultCategory = categories[index]
                        }
                    }
                }
                Component.onCompleted: {
                    _updating = false
                    currentIndex = categories.indexOf(config.defaultCategory)
                }
            }

            SectionHeader {
                text: "Categories"
            }

            Repeater {
                width: parent.width
                model: categories.length
                delegate: ValueButton {
                    width: parent.width
                    label: categoryNames[index] + ":"
                    value: "%1 applications".arg(config["category_" + categories[index]].length)
                    onClicked: {
                        var selector = pageStack.push(Qt.resolvedUrl("SelectionPage.qml"), {selectedValues: config["category_" + categories[index]]})
                        selector.accepted.connect(function() {
                            var selected = selector.selectedValues
                            config["category_" + categories[index]] = selected
                            for (var i = 0; i < selected.length; i++) {
                                for (var j = 0; j < categories.length; j++) {
                                    if (j != index) {
                                        var categoryName = "category_" + categories[j]
                                        var temp = config[categoryName]
                                        var itemIndex = temp.indexOf(selected[i])
                                        if (itemIndex >= 0) {
                                            temp.splice(itemIndex, 1)
                                            config[categoryName] = temp
                                        }
                                    }
                                }
                            }
                        })
                    }
                }
            }

            ValueButton {
                label: "Custom notification tone"
                value: profiled.notificationTone ? metadataReader.getTitle(profiled.notificationTone) : "no sound"
                onClicked: {
                    var dialog = pageStack.push(dialogComponent, {
                        activeFilename: profiled.notificationTone,
                        activeSoundTitle: value,
                        activeSoundSubtitle: "Custom notification tone",
                        noSound: false
                        })

                    dialog.accepted.connect(
                        function() {
                            profiled.notificationTone = dialog.selectedFilename
                            profiled.typedCall("set_value",
                                               [
                                                   {"type": "s", "value": profiled.profileName},
                                                   {"type": "s", "value": profiled.notificationToneKey},
                                                   {"type": "s", "value": profiled.notificationTone}
                                               ])
                        })
                }
            }
        }
    }

    AlarmToneModel {
        id: alarmToneModel
    }

    MetadataReader {
        id: metadataReader
    }

    Component {
        id: dialogComponent

        SoundDialog {
            alarmModel: alarmToneModel
        }
    }
}


