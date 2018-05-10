import QtQuick 2.7

Item {
    anchors.fill: parent

    Repeater {
        model: document.sections
        delegate: Rectangle {
            color: modelData.style.color
            width: 10
            height: 10
        }
    }
}
