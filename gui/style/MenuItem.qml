import QtQuick 2.9
import QtQuick.Templates 2.2 as T
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Controls.Material.impl 2.2

T.MenuItem {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                             Math.max(contentItem.implicitHeight,
                                      indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 24
    topPadding: 4
    bottomPadding: 4
    spacing: 16
    font.pixelSize: 13

    indicator: CheckIndicator {
        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        visible: control.checkable
        control: control
    }

    contentItem: Text {
        leftPadding: control.checkable && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.checkable && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.enabled ? control.Material.foreground : control.Material.hintTextColor
        elide: Text.ElideRight
        visible: control.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 24
        color: control.highlighted ? control.Material.listHighlightColor : "transparent"

        Ripple {
            width: parent.width
            height: parent.height

            clip: visible
            pressed: control.pressed
            anchor: control
            active: control.down || control.visualFocus || control.hovered
            color: control.Material.rippleColor
        }
    }

    default property list<Item> contentData

    onTriggered: if (contentData.length > 0) {
                   //menuLoader.item.open()
               }

    onHoveredChanged: {
        if (contentData.length > 0) {
          if (hovered) {
              menuLoader.item.delayOpen()
          } else {
              menuLoader.item.stop()
              menuLoader.item.close()
          }
        }
    }

    Loader {
        id: menuLoader
        active: contentData.length > 0
        sourceComponent: menuComp
        onLoaded: item.contentData = control.contentData
    }

    Component {
        id: menuComp
        Menu {
            id: subMenu
            x: control.width
            transformOrigin: Item.TopLeft

            function delayOpen() {
                openTimer.start()
            }

            function stop() {
                openTimer.stop()
            }

            Timer {
                id: openTimer
                interval: 200
                onTriggered: subMenu.open()
            }
        }
    }
}
