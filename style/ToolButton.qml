import QtQuick 2.9
import QtQuick.Templates 2.2 as T
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

T.ToolButton {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             contentItem.implicitHeight + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 6

    font.pixelSize: 12

    contentItem: Text {
        text: control.text
        font: control.font
        color: !control.enabled ? control.Material.hintTextColor :
                control.checked || control.highlighted ? control.Material.accent : control.Material.foreground
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Ripple {
        implicitWidth: 48
        implicitHeight: 48

        readonly property bool square: control.contentItem.implicitWidth <= control.contentItem.implicitHeight

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        clip: !square
        width: square ? parent.height / 2 : parent.width
        height: square ? parent.height / 2 : parent.height
        pressed: control.pressed
        anchor: control
        active: control.enabled && (control.down || control.visualFocus || control.hovered)
        color: control.Material.rippleColor
    }
}
