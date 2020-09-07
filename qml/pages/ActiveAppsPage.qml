import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.takeoff 1.0

Page {
    id: page

    // The effective value will be restricted by ApplicationWindow.allowedOrientations
    allowedOrientations: Orientation.All

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Manage")
                onClicked: pageStack.push(Qt.resolvedUrl("ManagerPage.qml"))
            }
        }

        id: listView

        header: PageHeader {
            title: qsTr("Active Apps")
        }

        anchors.fill: parent

        model: AutostartManager.activeApps()

        delegate: BackgroundItem {
            id: delegate

            width: parent.width
            height: Theme.itemSizeLarge
            contentHeight: Theme.itemSizeLarge

            Row {
                width: parent.width - 2 * x
                x: Theme.horizontalPageMargin
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: appIcon

                    height: parent.height - 2 * Theme.paddingSmall
                    width: parent.height - 2 * Theme.paddingSmall
                    anchors.verticalCenter: parent.verticalCenter

                    source: "image://theme/" + icon
                }

                Item {
                    id: spacer

                    width:Theme.paddingMedium
                    height:1

                }

                Column {
                    id: data

                    width: parent.width - appIcon.width

                    anchors.verticalCenter: appIcon.verticalCenter

                    Label{
                        id: text
                        width: parent.width
                        elide: Text.ElideRight
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

            onClicked: console.log("Clicked " + index)
        }
        VerticalScrollDecorator {}
    }
}
