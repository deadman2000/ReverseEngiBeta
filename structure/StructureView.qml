import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import "../docking"
import ".."

DockPanel {
    title: "Structure"

    buttons: [
        DockButton {
            icon: 'qrc:icons/ic_add_white_24px.svg'
            onClicked: openEditDialog()
        }
    ]

    property var __editDialogComp: null
    function openEditDialog()
    {
        if (!__editDialogComp)
            __editDialogComp = Qt.createComponent('EditBlockDialog.qml')
        var dialog = __editDialogComp.createObject(window);
        dialog.show()
    }

    TreeView {
        id: tree
        anchors.fill: parent
        model: currentFile.document.structure
        style: CustomTreeStyle { }
        //frameVisible: false

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
                // TODO Context menu
            }
        }

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
