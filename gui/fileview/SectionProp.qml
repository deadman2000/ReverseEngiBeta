import QtQuick 2.12

Item {
    visible: false

    readonly property var startCell: addressToPoint(section.begin)
    readonly property var endCell: addressToPoint(section.end)

    property var section
    opacity: section.visible ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 100 } }
    onOpacityChanged: canvas.requestPaint()

    property color fillColor: section.shadow ? Qt.rgba(0, 0, 0, 0.02 * opacity) : G.blendAlpha(section.style.color, opacity)
    Behavior on fillColor { ColorAnimation { duration: 100 } }
    onFillColorChanged: canvas.requestPaint()

    property color borderColor: section.shadow ? Qt.rgba(0, 0, 0, 0.5 * opacity) : G.blendAlpha(section.style.borderColor, opacity)
    Behavior on borderColor { ColorAnimation { duration: 100 } }
    onBorderColorChanged: canvas.requestPaint()

    Connections {
        target: section
        onChanged: canvas.requestPaint()
    }
}
