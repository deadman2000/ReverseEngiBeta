import QtQuick 2.7
import QtQuick.Layouts 1.3

Item {
    property int dockHeight: 0
    property int dockWidth: 0

    height: dockHeight
    width: dockWidth

    Layout.fillHeight: dockHeight == 0
    Layout.fillWidth: dockWidth == 0

    Layout.minimumHeight: 100
    Layout.minimumWidth: 100
}
