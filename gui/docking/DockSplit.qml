import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

SplitView  {
    id: splitter

    handleDelegate: Rectangle {
        color: splitter.orientation == Qt.Vertical ? 'transparent' : 'black'
        width: 1
        height: 1
    }

    property int dockHeight: 0
    property int dockWidth: 0

    height: dockHeight
    width: dockWidth

    Layout.fillHeight: dockHeight == 0
    Layout.fillWidth: dockWidth == 0

    Layout.minimumHeight: 100
    Layout.minimumWidth: 100
}
