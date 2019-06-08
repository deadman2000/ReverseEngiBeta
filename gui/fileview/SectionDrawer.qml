import QtQuick 2.12

Item {
    id: drawer
    anchors.fill: parent

    property real borderWidth: 1
    property real borderPadding: 1

    Repeater {
        id: sectionProps

        model: document.sections
        delegate: SectionProp {
            section: modelData
        }
    }

    SectionProp {
        id: selectionProp
        section: selection
    }

    Canvas { // Отрисовка границы секции
        id: canvas
        anchors.fill: parent
        renderStrategy: Canvas.Threaded

        Connections {
            target: list
            onContentYChanged: canvas.requestPaint()
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset()
            ctx.resetTransform()

            var i;
            for (i in document.sections){
                var prop = sectionProps.itemAt(i)

                canvas.drawSection(ctx, document.sections[i], prop);
            }

            drawSection(ctx, selection, selectionProp);
        }

        function drawSection(ctx, section, prop) {
            if (!section.isSet || prop.opacity === 0) return;

            var startCell = prop.startCell
            var endCell = prop.endCell

            var bw = section.selected ? 2 : borderWidth
            var bp = borderPadding + bw / 2

            ctx.strokeStyle = prop.borderColor
            ctx.lineWidth = bw
            ctx.fillStyle = prop.fillColor

            var yShift = startCell.y * G.rowHeight - list.contentY;
            if (yShift > canvas.height) return;

            var start = startCell
            var end = endCell
            var firstRect = {
                x: start.x * cellWidth,
                y: 0 + yShift,
                width: endCell.y === startCell.y ? cellWidth * (endCell.x - startCell.x + 1) // One line
                                                 : (16 - startCell.x) * cellWidth, // First line
                height: G.rowHeight
            };
            var lastRect = {
                x: 0,
                y: (endCell.y - startCell.y) * G.rowHeight + yShift,
                width: (endCell.x + 1) * cellWidth,
                height: G.rowHeight
            };

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
                    var s6y = firstRect.y + firstRect.height + bp

                    ctx.moveTo(s0x, s0y); // 0
                    ctx.lineTo(s1x, s0y); // 1
                    if (s1x != s3x) {
                        ctx.lineTo(s1x, s2y); // 2
                        ctx.lineTo(s3x, s2y); // 3
                    }
                    ctx.lineTo(s3x, s4y); // 4
                    ctx.lineTo(s5x, s4y); // 5
                    if (s5x != s0x) {
                        ctx.lineTo(s5x, s6y); // 6
                        ctx.lineTo(s0x, s6y); // 7
                    }
                }
            }

            ctx.closePath();
            ctx.fill();
            ctx.stroke();
        }
    }
}
