import QtQuick 2.7
import QtQuick.Layouts 1.3

Item {
    id: container
    property int dockHeight: 0
    property int dockWidth: 0

    height: dockHeight
    width: dockWidth

    Layout.fillHeight: dockHeight == 0
    Layout.fillWidth: dockWidth == 0

    property var splitter: parent && parent.splitter ? parent.splitter : null
    property bool splitterHorizontal: splitter && splitter.orientation == Qt.Horizontal

    Rectangle {
        id: dragRect

        z: 1000
        enabled: !!splitter
        visible: enabled

        color: 'red'

        //anchors.right: splitterHorizontal ? parent.right : undefined
        //anchors.bottom: splitterHorizontal ? undefined : parent.bottom
        x: parent.width - width
        width: splitterHorizontal ? 4 : parent.width
        height: splitterHorizontal ? parent.height : 4

        MouseArea {
            anchors.fill: parent
            cursorShape: splitterHorizontal ? Qt.SplitHCursor : Qt.SplitVCursor

            drag.target: dragRect
            drag.axis: Drag.XAxis
            drag.minimumX: 0
            drag.maximumX: container.parent.width
        }
    }
}
