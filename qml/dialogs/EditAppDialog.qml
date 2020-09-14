import QtQuick 2.2
import Sailfish.Silica 1.0

import org.nubecula.harbour.takeoff 1.0

Dialog {
    property var app

    Column {
        width: parent.width

        DialogHeader { title: qsTr("Edit start command") }

        TextField {
            id: cmdField
            width: parent.width
            placeholderText: qsTr("Enter start command")
            label: qsTr("Start command")

            text: {
                if (app.startCmdCustom.length)
                    return app.startCmdCustom
                else
                    return app.startCmd
            }
        }

        Item {
            height: Theme.paddingMedium
            width: 1
        }

        ButtonLayout {
            Button {
                text: qsTr("Execute")
                onClicked: AutostartManager.execute(cmdField.text)
            }

            Button {
                text: qsTr("Reset")
                onClicked: cmdField.text = app.startCmd
            }
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            if (app.startCmd === cmdField.text)
                app.startCmdCustom = ""
            else
                app.startCmdCustom = cmdField.text

            AutostartManager.applyChanges()
        }
    }
}
