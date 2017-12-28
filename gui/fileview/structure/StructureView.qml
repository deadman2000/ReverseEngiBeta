import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import "../../docking"
import "../.."

DockPanel {
    title: "Structure"

    buttons: [
        DockButton {
            icon: 'qrc:icons/ic_folder_open_white_24px.svg'
            onClicked: fileDialog.open()
        },
        DockButton {
            icon: 'qrc:icons/ic_save_white_24px.svg'
            //onClicked: saveStructure()
        },
        DockButton {
            icon: 'qrc:icons/ic_add_white_24px.svg'
            onClicked: createBlock()
        }
    ]

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        //folder: shortcuts.home
        nameFilters: ["Structure files (*.json)"]
        onAccepted: {
            currentFile.document.loadStructure(fileUrl)
            currentFile.structure = G.createStructure(currentFile.document);
        }
    }

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
        dialog.fillForm(tree.selectedNode.data())
        dialog.show()
    }

    function changeBlock(typeId, attrs)
    {
        if (tree.selectedNode)
            tree.selectedNode.change(typeId, attrs)
        updateView()
    }

    function addBlock(typeId, attrs)
    {
        if (tree.selectedNode)
            tree.selectedNode.add(typeId, attrs)
        else
            currentFile.structure.add(typeId, attrs)
        updateView()
    }

    function deleteBlock()
    {
        tree.selectedNode.remove()
        updateView()
    }

    function updateView()
    {
        currentFile.structure.update()
    }

    CustomTree {
        id: tree
        anchors.fill: parent
        model: currentFile.structure
        onTreeChanged: updateView()

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            propagateComposedEvents: true
            onPressed: {
                mouse.accepted = false
                ctxMenu.x = mouse.x
                ctxMenu.y = mouse.y
                ctxMenu.open()
            }
        }

        onDoubleClicked: editBlock()
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
