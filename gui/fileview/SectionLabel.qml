import QtQuick 2.12

Rectangle {
    opacity: 0.9
    color: Qt.rgba(1,1,1)
    height: label.height + 4
    width: label.width + height
    radius: height/2
    visible: false

    Text {
        id: label
        anchors.centerIn: parent
    }

    function showSection(section) {
        label.text = section.name
        visible = true
    }
}
