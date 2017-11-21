import QtQuick 2.0

Rectangle {
    id: button
    width: 24
    height: 24

    color: mouse.pressed ? Qt.rgba(0,0,0,0.2) : mouse.containsMouse ? Qt.rgba(1,1,1,0.1) : 'transparent'

    property alias icon: image.source

    signal clicked

    Image {
        id: image
        sourceSize: Qt.size(24, 24)
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: button.clicked()
    }
}
