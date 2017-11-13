import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import "fileview"

ApplicationWindow {
    id: window
    visible: true
    width: 1024
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

    footer: ToolBar { // StatusBar
        height: 24

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Item { width: 16; height: 1 }

            Label {
                Layout.fillWidth: true
                text: currentFile.fileModel.fileName
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
        fileView.openFile('c:/Windows/explorer.exe')
    }

    property var currentFile: fileView

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillHeight: true
            width: 300

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    color: 'gray'
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Item {
                    Layout.fillWidth: true
                    height: 4

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.SplitVCursor
                    }
                }

                Interpreter {
                    Layout.fillWidth: true
                }
            }
        }

        FileView {
            id: fileView
            focus: true

            Layout.fillWidth: true
            Layout.fillHeight: true
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
                fileView.openFile(drop.urls)
            }
        }
    }
}
