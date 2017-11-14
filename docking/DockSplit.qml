import QtQuick 2.7
import QtQuick.Layouts 1.3

DockContainer {
    id: splitter
    property int orientation: Qt.Horizontal
    default property alias children: grid.children

    GridLayout {
        id: grid
        anchors.fill: parent
        rowSpacing: 0
        columnSpacing: 0
        flow: orientation == Qt.Horizontal ? GridLayout.LeftToRight : GridLayout.TopToBottom

        property alias splitter: splitter
    }
}
