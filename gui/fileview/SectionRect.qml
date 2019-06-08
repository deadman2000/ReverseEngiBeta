import QtQuick 2.12

Item {
    id: rect
    property var section

    property var startCell: addressToPoint(section.begin)
    property var endCell: addressToPoint(section.end)

    visible: section.isSet

    opacity: section.visible ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 100 } }

    width: parent.width
    y: startCell.y * G.rowHeight
    height: (endCell.y - startCell.y + 1) * G.rowHeight

    property int _containsMouseCnt: 0
    readonly property bool containsMouse: _containsMouseCnt > 0

    Rectangle {
        id: topRow
        color: section.shadow ? Qt.rgba(0, 0, 0, 0.02) : section.style.color
        Behavior on color { ColorAnimation { duration: 100 } }

        x: startCell.x * cellWidth
        y: 0
        width: endCell.y === startCell.y ? cellWidth * (endCell.x - startCell.x + 1) // One line
                                         : (16 - startCell.x) * cellWidth // First line

        height: G.rowHeight

        MouseArea {
            anchors.fill: parent
            visible: rect.visible && section.visible
            propagateComposedEvents: true
            onClicked: {
                console.log('clicked1', section.name)
                mouse.accepted = false
            }

            hoverEnabled: true
            onEntered: _containsMouseCnt += 1
            onExited: _containsMouseCnt -= 1
        }
    }

    Rectangle {
        id: middleRow
        color: section.shadow ? Qt.rgba(0, 0, 0, 0.02) : section.style.color
        Behavior on color { ColorAnimation { duration: 100 } }

        visible: endCell.y - startCell.y > 1
        x: 0
        y: G.rowHeight
        width: 16 * cellWidth
        height: (endCell.y - startCell.y - 1) * G.rowHeight

        MouseArea {
            anchors.fill: parent
            visible: rect.visible && section.visible
            propagateComposedEvents: true
            onClicked: {
                console.log('clicked2', section.name)
                mouse.accepted = false
            }

            hoverEnabled: true
            onEntered: _containsMouseCnt += 1
            onExited: _containsMouseCnt -= 1
        }
    }

    Rectangle {
        id: bottomRow
        color: section.shadow ? Qt.rgba(0, 0, 0, 0.02) : section.style.color
        Behavior on color { ColorAnimation { duration: 100 } }

        visible: endCell.y !== startCell.y
        x: 0
        y: (endCell.y - startCell.y) * G.rowHeight
        width: (endCell.x + 1) * cellWidth
        height: G.rowHeight

        MouseArea {
            anchors.fill: parent
            visible: rect.visible && section.visible
            propagateComposedEvents: true
            onClicked: {
                console.log('clicked3', section.name)
                mouse.accepted = false
            }

            hoverEnabled: true
            onEntered: _containsMouseCnt += 1
            onExited: _containsMouseCnt -= 1
        }
    }

    Canvas { // Отрисовка границы секции
        id: canvas
        anchors.fill: parent
        visible: true

        Connections {
            target: section
            onChanged: canvas.requestPaint()
        }

        property color color: section.shadow ? Qt.rgba(0,0,0, 0.5) : section.style.borderColor
        Behavior on color { ColorAnimation { duration: 100 } }
        onColorChanged: canvas.requestPaint()

        onPaint: {
            if (canvas.height > 200) {
                console.log('Height:', canvas.height);
                console.log(section.name);
                return;
            }
            var ctx = getContext("2d");
            ctx.reset()
            ctx.resetTransform()

            var bw = section.selected ? 2 : borderWidth
            var bp = borderPadding + bw / 2

            ctx.strokeStyle = canvas.color
            ctx.lineWidth = bw

            var start = startCell
            var end = endCell
            var firstRect = topRow
            var lastRect = bottomRow

            ctx.beginPath()

            if (start.y === end.y){
                // Выделение на одной строке
                ctx.rect(firstRect.x + bp, firstRect.y + bp, firstRect.width - bp*2, firstRect.height - bp*2)
            } else {
                if (start.y === end.y - 1 && start.x > end.x) {
                    // Выделение на двух строках в разных прямоугольниках
                    /*  |               _________|
                        |_________     |_________|
                        |_________|              | */

                    ctx.rect(firstRect.x + bp, firstRect.y + bp, firstRect.width - bp*2, firstRect.height - bp*2)
                    ctx.rect(lastRect.x + bp, lastRect.y + bp, lastRect.width - bp*2, lastRect.height - bp*2)
                } else {
                    // Выделение в несколько строк одним 8-угольником
                    /*  |        0_______________|1
                       6|________|               |
                        |         7      3 ______|2
                        |_________________|      |
                       5|                  4     |  */

                    var s0x = firstRect.x + bp
                    var s0y = firstRect.y + bp
                    var s1x = firstRect.x + firstRect.width - bp
                    var s2y = lastRect.y - bp
                    var s3x = lastRect.width - bp
                    var s4y = lastRect.y + lastRect.height - bp
                    var s5x = bp
                    var s6y = firstRect.height + bp

                    ctx.moveTo(s0x, s0y);
                    ctx.lineTo(s1x, s0y);
                    ctx.lineTo(s1x, s2y);
                    ctx.lineTo(s3x, s2y);
                    ctx.lineTo(s3x, s4y);
                    ctx.lineTo(s5x, s4y);
                    ctx.lineTo(s5x, s6y);
                    ctx.lineTo(s0x, s6y);
                }
            }

            ctx.closePath();
            ctx.stroke();
        }
    }

    readonly property var bindTarget: middleRow.visible ? middleRow : topRow
}
