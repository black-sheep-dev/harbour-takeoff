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

            enabled: !useLibraryCmdSwitch.checked

            width: parent.width
            placeholderText: qsTr("Enter start command")
            label: qsTr("Start command")

            text: {
                if (app.startCmdCustom.length > 0)
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
                onClicked: {
                    cmdField.text = app.startCmd
                    useLibraryCmdSwitch.checked = false
                }
            }
        }

        Item {
            height: Theme.paddingMedium
            width: 1
        }

        TextSwitch {
            enabled: app.startCmdLibrary.length > 0
            visible: !AutostartManager.libraryAPI().autoUse

            id: useLibraryCmdSwitch

            text: qsTr("Use start command from library")
            description: qsTr("If you have problems with an app not starting, you can use a start command from the app library.")

            checked: app.useLibraryStartCmd

            onCheckedChanged: {
                if (checked)  {
                    cmdField.text = app.startCmdLibrary
                } else {
                    if (app.startCmdCustom.length > 0)
                        cmdField.text = app.startCmdCustom
                    else
                        cmdField.text = app.startCmd
                }
            }
        }
    }

    onDone: {
        if (result == DialogResult.Accepted) {
            if (app.startCmd === cmdField.text) {
                app.startCmdCustom = ""
            } else {
                if (cmdField.text !== app.startCmdLibrary)
                    app.startCmdCustom = cmdField.text
            }

            app.useLibraryStartCmd = useLibraryCmdSwitch.checked
            AutostartManager.applyChanges()
        }
    }
}
