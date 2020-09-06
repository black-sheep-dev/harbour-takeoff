import QtQuick 2.0
import Sailfish.Silica 1.0

import org.nubecula.takeoff 1.0

CoverBackground {

    Image {
        source: "qrc:///icons/logo-white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        sourceSize.width: Theme.iconSizeExtraLarge * 2
        sourceSize.height: Theme.iconSizeExtraLarge * 2
        opacity: 0.1
    }

}
