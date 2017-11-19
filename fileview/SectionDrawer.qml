import QtQuick 2.7

Canvas { // Отрисовка секций файла
    anchors.fill: parent

    Connections {
        target: fileView
        onTopRowChanged: requestPaint()
    }

    Connections {
        target: selection
        onChanged: requestPaint()
    }

    property int paddingX: 0
    property int paddingY: 0

    function drawSection(ctx, section, style)
    {
        if (!style) style = section.style

        var start = addressToPoint(section.begin)
        var end   = addressToPoint(section.end)

        if (start.y > topRow + rowsInScreen || end.y < topRow)
            return

        ctx.fillStyle = style.color
        ctx.strokeStyle = style.borderColor
        ctx.lineWidth = style.borderWidth

        ctx.resetTransform();
        if (style.borderWidth % 2 == 1) // Workaround
            ctx.translate(0.5, 0.5)

        var px = paddingX
        var py = paddingY

        // Координаты блока
        var startPos = symPos(start)
        var sx = startPos.x - px
        var sy = startPos.y - py
        var endPos   = symPos(end)
        var erx = endPos.x + symWith + px

        if (start.y === end.y){
            // Выделение на одной строке
            ctx.rect(sx, sy, erx - sx, rowHeight + py*2)
        } else {
            var s1 = symPos(Qt.point(15, start.y)) // Координаты краев
            var e1 = symPos(Qt.point(0, end.y))
            var s1x = s1.x + symWith + px
            var e1x = e1.x - px
            var ey = endPos.y - py

            if (start.y === end.y - 1 && start.x > end.x) {
                // Выделение на двух строках в разных прямоугольниках
                /*  |              |sP       |s1x
                    |e1x    eP|erx           |     */

                ctx.rect(sx, sy, s1x - sx, rowHeight + py*2)
                ctx.rect(e1x, ey, erx - e1x, rowHeight + py*2)
            } else {
                // Выделение в несколько строк одним 8-угольником
                /*  |        |sP             |s1x
                    |e1x            eP|erx   |     */

                var eby = endPos.y + rowHeight + py
                var sby = startPos.y + rowHeight + py

                ctx.beginPath()

                ctx.moveTo(sx, sy);
                ctx.lineTo(s1x, sy);
                ctx.lineTo(s1x, ey);
                ctx.lineTo(erx, ey);
                ctx.lineTo(erx, eby);
                ctx.lineTo(e1x, eby);
                ctx.lineTo(e1x, sby);
                ctx.lineTo(sx, sby);

                ctx.closePath();
            }
        }

        ctx.fill();
        ctx.stroke();
    }

    onPaint: {
        var ctx = getContext("2d");
        ctx.reset()

        if (selection.isSet)
            drawSection(ctx, selection)

        for (var i=0; i<document.sectionCount; ++i)
            drawSection(ctx, document.section(i))
    }
}
