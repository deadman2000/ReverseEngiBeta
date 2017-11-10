import QtQuick 2.7
import QtQuick.Window 2.3
import QtQuick.Controls 1.4
import ReverseEngi 1.0

Window {
    width: 300
    height: 300
    x: 0
    y: 0

    TableView {
        anchors.fill: parent

        TableViewColumn {
            role: "name"
            title: "Type"
            width: 100
        }
        TableViewColumn {
            role: "value"
            title: "Value"
            width: 100
        }

        model: InterpreterModel {
            model: fileModel
            offset: cursor.offset
        }
    }

    /*ListView {
        anchors.fill: parent
        model: G.instruments.interpreters
        delegate: Row {
            Text {
                text: modelData.name
            }

            Text {
                text: modelData.value()
            }
        }
    }*/
}
