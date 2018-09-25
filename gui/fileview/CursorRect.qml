import QtQuick 2.7

Rectangle {
    property real padding: 1

    width: symWidth - padding * 2
    height: 2

    readonly property point pos: symPos(cursor.pos)
    onPosChanged: cursorAnimation.restart()

    x: pos.x
    y: pos.y + 1 + G.rowHeight - height - 1 - 2

    SequentialAnimation on color {
        id: cursorAnimation
        loops: Animation.Infinite
        running: true
        ColorAnimation { to: Qt.rgba(0,0,0,0.87); duration: 100 }
        PauseAnimation { duration: 500 }
        ColorAnimation { to: Qt.rgba(0,0,0,0);  duration: 100 }
        PauseAnimation { duration: 500 }
    }
}
