import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.harbour.takeoff 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    SilicaFlickable {
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
        }
    }

    onStatusChanged: {
        if (status != PageStatus.Deactivating) return;

        if (startDelayField.acceptableInput) {
            AutostartManager.setStartDelay(startDelayField.text)
            AutostartManager.applyChanges()
        }
    }
}
