import QtQuick 2.9
import QtQuick.Templates 2.2 as T
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

T.ToolBar {
    id: control

    leftPadding: 8

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentHeight + topPadding + bottomPadding)

    contentWidth: contentItem.implicitWidth || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    Material.foreground: Material.toolTextColor

    spacing: 16

    background: Rectangle {
        implicitHeight: 24
        color: control.Material.toolBarColor
    }
}
