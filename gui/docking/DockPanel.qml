import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

DockContainer {
    property alias title: titleLabel.text
    property alias contentItem: content
    default property alias contentData: content.data

    property alias buttons: buttonsRow.children

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 24
            color: '#424242'
            //color: Material.accent
            visible: title.length > 0
            z: 1

            RowLayout {
                anchors.fill: parent
                spacing: 0
                anchors.leftMargin: 4
                anchors.rightMargin: 4

                Text {
                    id: titleLabel
                    color: 'white'
                    Layout.fillWidth: true
                    font.pixelSize: 12
                    renderType: Text.NativeRendering
                }

                RowLayout {
                    id: buttonsRow
                }
            }
        }

        Item {
            id: content
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
