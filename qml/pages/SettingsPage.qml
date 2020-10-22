import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.takeoff 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Reset to default values")
                onClicked: AutostartManager.libraryAPI().resetSettings()
            }
            MenuItem {
                enabled: AutostartManager.libraryAPI().active
                text: qsTr("Update library")
                onClicked: AutostartManager.libraryAPI().updateLibrary()
            }
        }

        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width:parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr("Startup Delay")
            }

            TextField {
                id: startDelayField
                width: parent.width

                label: qsTr("Delay in seconds")

                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator {
                    bottom: 0
                    top: 120
                }

                text: AutostartManager.startDelay
            }

            SectionHeader {
                text: qsTr("Library API")
            }

            TextSwitch {
                id: apiAutoUseSwitch
                text: qsTr("Use library commands by default")
                description: qsTr("Activating this option will overwrite your custom command settings! If disabled you can activate library command for every app individually.")

                Component.onCompleted: checked = AutostartManager.libraryAPI().autoUse
            }

            TextSwitch {
                id: apiActiveSwitch
                text: qsTr("Use library API")
                description: qsTr("Use the library from remote host to get good start commands for your apps. Be aware that this could be a potential security risk because the commands will be exectuted on your device!")

                Component.onCompleted: checked = AutostartManager.libraryAPI().active
            }

            TextSwitch {
                id: apiAutoUpdateSwitch

                enabled: apiActiveSwitch.checked

                text: qsTr("Automatic library updates")
                description: qsTr("Activating this option will update the library automatically on app start.")

                Component.onCompleted: checked = AutostartManager.libraryAPI().autoUpdate
            }

            TextField {
                id: urlField
                width: parent.width

                enabled: apiActiveSwitch.checked

                label: qsTr("Library url")
                placeholderText: qsTr("Enter library url")

                text: AutostartManager.libraryAPI().url

                inputMethodHints: Qt.ImhUrlCharactersOnly
            }
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        AutostartManager.libraryAPI().active = apiActiveSwitch.checked
        AutostartManager.libraryAPI().autoUpdate = apiAutoUpdateSwitch.checked
        AutostartManager.libraryAPI().autoUse = apiAutoUseSwitch.checked
        AutostartManager.libraryAPI().url = urlField.text
        AutostartManager.libraryAPI().saveSettings()

        if (startDelayField.acceptableInput) {
            AutostartManager.setStartDelay(startDelayField.text)
            AutostartManager.applyChanges()
        }
    }
}
