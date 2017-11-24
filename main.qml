import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "docking"
import "fileview"
import "interpreter"
import "structure"

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 768
    title: qsTr("Hello World")

    property int rowHeight: 32
    property int listTopPadding: 8

    function addressToPoint(address) {
        var y = Math.floor(address / 16)
        return Qt.point(address - y * 16, y)
    }

    function pointToAddress(point) {
        return point.y * 16 + point.x
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: qsTr("File")
            }

            ToolButton {
                text: qsTr("Edit")
            }

            ToolButton {
                text: qsTr("Tools")
            }

            ToolButton {
                text: qsTr("Window")
            }

            ToolButton {
                text: qsTr("Help")
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    footer: ToolBar { // StatusBar
        height: 24

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Item { width: 16; height: 1 }

            Label {
                Layout.fillWidth: true
                text: currentFile.document.fileName
            }

            Label {
                text: "Selection: " + currentFile.selection.size
            }

            Item { width: 16; height: 1 }

            Label {
                text: "Position: " + currentFile.cursor.offset
            }

            Item { width: 16; height: 1 }
        }
    }

    Component.onCompleted: {
        currentFile.openFile('c:/Windows/explorer.exe')
    }

    property var currentFile: fileView

    DockSplit {
        anchors.fill: parent
        orientation: Qt.Horizontal

        DockSplit {
            orientation: Qt.Vertical
            dockWidth: 300

            StructureView {
            }

            Interpreter {
                dockHeight: 300
            }
        }

        FileView {
            id: fileView
        }
    }

    DropArea {
        anchors.fill: parent
        onEntered: {
            if (drag.hasUrls){
                // TODO проверку на isFile и fileExists
                drag.accept(Qt.CopyAction);
            }
        }
        onDropped: {
            if (drop.hasUrls){
                currentFile.openFile(drop.urls)
            }
        }
    }
}
