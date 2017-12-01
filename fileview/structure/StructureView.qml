import QtQuick 2.7
import QtQuick.Controls 1.4 as C1
import QtQuick.Controls 2.2
import QtQml.Models 2.2
import QtQuick.Controls.Material 2.2
import "../../docking"
import "../.."

DockPanel {
    title: "Structure"

    buttons: [
        DockButton {
            icon: 'qrc:icons/ic_add_white_24px.svg'
            onClicked: createBlock()
        }
    ]

    property var __editDialogComp: null
    function createEditDialog()
    {
        if (!__editDialogComp)
            __editDialogComp = Qt.createComponent('EditBlockDialog.qml')
        return __editDialogComp.createObject(window);
    }

    function createBlock()
    {
        createEditDialog().show()
    }

    function editBlock()
    {
        var dialog = createEditDialog()
        var obj = tree.model.getBlock(tree.selection.currentIndex)
        dialog.fillForm(obj)
        dialog.show()
    }

    function deleteBlock()
    {
        tree.model.deleteBlock(tree.selection.currentIndex)
    }

    C1.TreeView {
        id: tree
        anchors.fill: parent
        model: currentFile.document.structure
        style: CustomTreeStyle { }
        //frameVisible: false

        selection: ItemSelectionModel {
            model: tree.model
            onCurrentChanged: model.selectBlock(currentIndex)
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onPressed: {
                var ind = tree.indexAt(mouse.x, mouse.y)
                tree.selection.setCurrentIndex(ind, 0x10);

                ctxMenu.x = mouse.x
                ctxMenu.y = mouse.y
                ctxMenu.open()
            }
        }

        onDoubleClicked: editBlock()

        C1.TableViewColumn {
            title: "Type"
            role: "type"
            width: 64
        }

        C1.TableViewColumn {
            title: "Name"
            role: "name"
            width: 128
        }

        C1.TableViewColumn {
            title: "Value"
            role: "value"
        }

    }

    Menu {
        id: ctxMenu

        MenuItem {
            text: "Add block"
            onTriggered: createBlock()
        }

        MenuItem {
            text: "Edit block"
            onTriggered: editBlock()
        }

        MenuItem {
            text: "Delete"
            onTriggered: deleteBlock()
        }
    }
}
