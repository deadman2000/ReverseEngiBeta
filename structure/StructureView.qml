import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import "../docking"

DockPanel {
    title: "Structure"

    buttons: [
        DockButton {
            icon: 'qrc:icons/ic_add_white_24px.svg'
            onClicked: dialog.open()
        }
    ]

    TreeView {
        id: tree
        anchors.fill: parent
        model: currentFile.document.structure

        selection: ItemSelectionModel {
            model: tree.model
            onCurrentChanged: currentFile.document.selectBlock(currentIndex)
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onPressed: {
                var ind = tree.indexAt(mouse.x, mouse.y)
                tree.selection.setCurrentIndex(ind, 0x10);
            }
        }

        /*headerDelegate: Item {
            Text {
                text: styleData.value
                horizontalAlignment: styleData.textAlignment
                width: parent.width
            }
            height: 32
        }

        itemDelegate: Item {
            Text {
                //anchors.verticalCenter: parent.verticalCenter
                color: styleData.textColor
                elide: styleData.elideMode
                text: styleData.value
            }
        }

        rowDelegate: Item {
            height: 24
        }*/

        TableViewColumn {
            title: "Name"
            role: "display"
        }

        TableViewColumn {
            title: "Value"
            role: "value"
        }
    }
}
