import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2

TreeViewStyle {
    textColor: Material.foreground
    highlightedTextColor: Material.foreground

    headerDelegate: Rectangle {
        height: Math.round(textItem.implicitHeight * 1.2)

        Text {
            id: textItem
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: styleData.textAlignment
            anchors.leftMargin: horizontalAlignment === Text.AlignLeft ? 12 : 1
            anchors.rightMargin: horizontalAlignment === Text.AlignRight ? 8 : 1
            text: styleData.value
            elide: Text.ElideRight
            color: textColor
            renderType: Text.NativeRendering
            font.pixelSize: 12
        }

        Rectangle {
            width: 1
            height: parent.height - 2
            y: 1
            color: "#ccc"
        }

        Rectangle {
            y: parent.height - 1
            width: parent.width
            height: 1
            color: "#ccc"
        }
    }

    branchDelegate: Item {
        width: indentation
        height: 20

        Image {
            anchors.centerIn: parent
            source: 'qrc:icons/ic_arrow_drop_down_black_24px.svg'
            sourceSize: Qt.size(24, 24)
            opacity: 0.54
            rotation: styleData.isExpanded ? 0 : -90
            Behavior on rotation { PropertyAnimation { duration: 100 } }
        }
    }

    itemDelegate: Item {
            height: 20
            property int implicitWidth: label.implicitWidth + 20

            Text {
                id: label
                objectName: "label"
                width: parent.width - x - (horizontalAlignment === Text.AlignRight ? 8 : 1)
                x: (styleData.hasOwnProperty("depth") && styleData.column === 0) ? 0 :
                   horizontalAlignment === Text.AlignRight ? 1 : 8
                horizontalAlignment: styleData.textAlignment
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 1
                elide: styleData.elideMode
                text: styleData.value !== undefined ? styleData.value.toString() : ""
                color: styleData.textColor
                renderType: Text.NativeRendering
                font.pixelSize: 12
            }
        }

    rowDelegate: Rectangle {
            height: 20
            color: styleData.selected ? '#f5f5f5' : 'white'

            Rectangle {
                visible: styleData.selected
                width: 3
                height: parent.height
                color: Material.accent
            }
        }
}
