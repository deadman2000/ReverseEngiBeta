import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
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

    CustomTree {
        id: tree
        anchors.fill: parent
        model: currentFile.structure
        onSelectedNodeChanged: currentFile.selectRange(selectedNode)
        onDoubleClicked: currentFile.focusRange(selectedNode.range())
    }
}
