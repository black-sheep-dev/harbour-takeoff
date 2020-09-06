import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

import org.nubecula.takeoff 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Reset")
                onClicked: AutostartManager.reset()
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: AutostartManager.refresh()
            }
        }

        id: listView

        header: PageHeader {
            title: qsTr("Manage Apps")
        }

        model: AutostartManager.apps()
        anchors.fill: parent
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

                Column{
                    id: data

                    width: parent.width - appIcon.width - activateSwitch.width

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

                Switch {
                    id: activateSwitch

                    Component.onCompleted: checked = autostart

                    onCheckedChanged: autostart = checked
                }
            }
        }
        VerticalScrollDecorator {}
    }
}
