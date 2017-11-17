import QtQuick 2.0
import QtQuick.Controls 1.4
import "../docking"

// https://dhtmlx.com/blog/wp-content/uploads/2016/05/dhx-ms-suite-b8502.png
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
