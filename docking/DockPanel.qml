import QtQuick 2.7
import QtQuick.Layouts 1.3

DockContainer {
    property alias title: titleLabel.text
    default property alias data: content.data

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 24
            color: '#424242'
            visible: title.length > 0
            z: 1

            Text {
                id: titleLabel
                color: '#FFFFFF'
                x: 4
                y: 6
            }
        }

        Item {
            id: content
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
