import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

import org.nubecula.takeoff 1.0

ApplicationWindow
{
    initialPage: Component { ActiveAppsPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: defaultAllowedOrientations

    Component.onCompleted: AutostartManager.refresh()
}
