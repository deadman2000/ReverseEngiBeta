import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import "docking"
import "fileview"
import "fileview/interpreter"

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 768
    title: qsTr("Hello World")

    property int listTopPadding: 8

    function addressToPoint(address) {
        var y = Math.floor(address / 16)
        return Qt.point(address - y * 16, y)
    }

    function pointToAddress(point) {
        return point.y * 16 + point.x
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
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
                currentFile.openFile(drop.urls[0])
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
                visible: currentFile.selection.size > 0
                text: "Selection: " + currentFile.selection.size
            }

            Item { width: 16; height: 1 }

            Label {
                text: "Position: " + currentFile.cursor.offset
            }

            Item { width: 16; height: 1 }
        }
    }
}
