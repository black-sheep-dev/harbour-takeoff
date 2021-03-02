import QtQuick 2.0
import Sailfish.Silica 1.0
import QtGraphicalEffects 1.0

import org.nubecula.harbour.takeoff 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Reset")
                onClicked: AutostartManager.reset()
            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: AutostartManager.refresh()
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        Column {
            id: header
            width: parent.width

            PageHeader {
                title: qsTr("Manage Apps")
            }

            SearchField {
                id: searchField
                width: parent.width

                onTextChanged: filterModel.setFilterFixedString(text)
            }
        }

        SilicaListView {
            id: listView

            width: parent.width
            anchors.top: header.bottom
            anchors.bottom: parent.bottom

            clip: true

            model: AppListSortFilterModel {
                id: filterModel
                sortRole: AppListModel.NameRole
                filterRole: AppListModel.NameRole

                sourceModel: AutostartManager.apps()
            }


            delegate: ListItem {
                id: delegate

                width: parent.width
                contentHeight: Theme.itemSizeLarge

                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Show desktop file");
                        onClicked: pageStack.push(Qt.resolvedUrl("DesktopFileViewerPage.qml"), {
                                                      title: model.name,
                                                      content: AutostartManager.desktopFileContent(model.desktopFile)
                                                  })
                    }
                }

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

                        source: icon

                        Image {
                            visible: jailed

                            anchors.right: parent.right
                            anchors.bottom: parent.bottom

                            source: "image://theme/icon-s-secure"
                        }
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
                            text: packageName
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall

                        }
                    }

                    Switch {
                        visible: !jailed

                        id: activateSwitch

                        Component.onCompleted: checked = autostart

                        onCheckedChanged: {
                            autostart = checked
                        }
                    }
                }

                onClicked: if (!jailed) activateSwitch.checked = !activateSwitch.checked

            }
            VerticalScrollDecorator {}
        }
    }

    onStatusChanged: if (status === PageStatus.Deactivating) AutostartManager.applyChanges()
}
