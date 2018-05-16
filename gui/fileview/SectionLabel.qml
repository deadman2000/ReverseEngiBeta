import QtQuick 2.0

Rectangle {
    opacity: 0.9
    color: Qt.rgba(1,1,1)
    height: label.height + 4
    width: label.width + height
    radius: height/2

    property alias text: label.text

    Text {
        id: label
        anchors.centerIn: parent
    }
}
