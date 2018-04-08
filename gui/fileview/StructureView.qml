import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import "../docking"
import ".."

DockPanel {
    title: "Structure"

    /*buttons: [
        DockButton {
            icon: 'qrc:icons/ic_folder_open_white_24px.svg'
            onClicked: fileDialog.open()
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
    }*/

    function updateView()
    {
        if (currentFile.structure)
            currentFile.structure.update()
    }

    CustomTree {
        id: tree
        anchors.fill: parent
        model: currentFile.structure
        onTreeChanged: updateView()
        onDoubleClicked: currentFile.selectRange(tree.selectedNode.range())
    }
}
