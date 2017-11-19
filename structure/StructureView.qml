import QtQuick 2.0
import QtQuick.Controls 1.4
import "../docking"

DockPanel {
    title: "Structure"

    TreeView {
        anchors.fill: parent

        TableViewColumn {
            title: "Name"
            role: "name"
        }

        model: ListModel {
            ListElement {
                name: "Apple"
            }
            ListElement {
                name: "Orange"
            }
            ListElement {
                name: "Banana"
            }
        }
    }
}
