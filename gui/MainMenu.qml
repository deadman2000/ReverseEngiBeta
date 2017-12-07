import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ToolBar {
    default property alias children: row.children

    RowLayout {
        id: row
        anchors.fill: parent
    }
}
