import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.takeoff 1.0

import "../dialogs/"

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        id: listView

        anchors.fill: parent
        clip: true

        model: AutostartManager.activeApps()

        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Manage")
                onClicked: pageStack.push(Qt.resolvedUrl("ManagerPage.qml"))
            }

            MenuItem {
                text: qsTr("Takeoff")
                onClicked: AutostartManager.takeoff()
            }
        }


        header: PageHeader {
            title: qsTr("Active Apps")
        }

        delegate: ListItem {
            id: delegate

            width: listView.width
            contentHeight: Theme.itemSizeLarge

            RemorseItem { id: remorse }

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Move up")
                    onClicked: AutostartManager.activeApps().moveUp(index)
                }
                MenuItem {
                    text: qsTr("Move down")
                    onClicked: AutostartManager.activeApps().moveDown(index)
                }
                MenuItem {
                    text: qsTr("Disable")
                    onClicked: remorse.execute(delegate, qsTr("Disabling autostart of app"), function() {
                        autostart = false
                        AutostartManager.activeApps().removeApp(index)
                        AutostartManager.applyChanges()
                    })
                }
                MenuItem {
                    text: qsTr("Edit");
                    onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/EditAppDialog.qml"), {app: AutostartManager.activeApps().app(index)})
                }
            }

            Image {
                id: appIcon

                x: Theme.horizontalPageMargin
                height: parent.height - 2 * Theme.paddingSmall
                width: parent.height - 2 * Theme.paddingSmall

                anchors.verticalCenter: parent.verticalCenter

                source: "image://theme/" + icon
            }

            Item {
                id: spacer

                anchors.left: appIcon.right
                width:Theme.paddingMedium
                height: 1

            }

            Column {
                id: data

                anchors.left: spacer.right
                anchors.verticalCenter: appIcon.verticalCenter

                Label{
                    id: text
                    width: parent.width
                    text: name
                    color: pressed ? Theme.secondaryHighlightColor:Theme.highlightColor
                    font.pixelSize: Theme.fontSizeMedium
                }
                Label{
                    text: package_name
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count == 0
            text: qsTr("No active apps")
            hintText: qsTr("Pull down to add apps in manager")
        }

        VerticalScrollDecorator {}
    }
}
