import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import ReverseEngi 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Hello World")

    header: ToolBar {
        Flow {
            id: flow
            width: parent.width

        }
    }

    HexText {
        id: hexLetter
        text: ' '
    }

    FileModel {
        id: fileModel
    }

    Component.onCompleted: fileModel.setFilePath('c:/Windows/explorer.exe')

    Item {
        id: bgrItem
        anchors.fill: parent

        Row {
            anchors.fill: parent
            spacing: 0

            Item { width: 16; height: 1 }

            Item { width: hexLetter.width * 9; height: 1 }

            Item { width: 32 - hexLetter.width/2; height: 1 }

            Rectangle {
                id: colHex1
                width: hexLetter.width * 4 * 3
                height: bgrItem.height
                color: Qt.rgba(0,0,0,0.05)
            }

            Item { width: hexLetter.width * 4 * 3; height: 1 } // col2

            Item { width: hexLetter.width; height: 1 }

            Rectangle {
                id: colHex3
                width: hexLetter.width * 4 * 3
                height: bgrItem.height
                color: Qt.rgba(0,0,0,0.05)
            }

            Item { id: colHex4; width: hexLetter.width * 4 * 3; height: 1 }

            Item { width: 32 - hexLetter.width/2; height: 1 }

            Item {
                id: colAscii
                width: hexLetter.width * 16
                height: 1
            }
        }
    }

    property bool selection: false
    property point selStart
    property point selEnd

    function getSym(col, x, letterWidth)
    {
        if (x >= col.x && x < col.x + col.width * 2)
            return Math.round( (x - col.x) / letterWidth - 0.5 )
        return -1
    }

    function hexAt(x, y)
    {
        x = x + hexList.contentX; y = y + hexList.contentY
        var sym = getSym(colHex1, x, hexLetter.width * 3)
        if (sym < 0)
        {
            sym = getSym(colHex3, x, hexLetter.width * 3)
            if (sym < 0) return null
            sym += 8
        }

        var it = hexList.itemAt(x, y)
        if (it === null) return null
        return Qt.point(sym, it.index)
    }

    function textAt(x, y)
    {
        x = x + hexList.contentX; y = y + hexList.contentY
        var sym = getSym(colAscii, x, hexLetter.width)
        if (sym < 0) return null

        var it = hexList.itemAt(x, y)
        if (it === null) return null
        return Qt.point(sym, it.index)
    }

    function hexPos(coord)
    {
        var x
        if (coord.x < 8)
            x = colHex1.x + coord.x * hexLetter.width * 3
        else
            x = colHex3.x + (coord.x - 8) * hexLetter.width * 3

        return Qt.point(x, coord.y * 32)
    }

    function textPos(coord)
    {
        return Qt.point(colAscii.x + coord.x * hexLetter.width, coord.y * 32)
    }

    function setAlpha(color, alpha)
    {
        return Qt.rgba(color.r, color.g, color.b, alpha)
    }

    Canvas {
        id: sectionDrawer
        anchors.fill: parent
        visible: false

        function drawSectionPoly(ctx, startPos, endPos, boundLeft, boundRight, symWidth)
        {
            ctx.moveTo(startPos.x, startPos.y);
            ctx.lineTo(boundRight,startPos.y);
            ctx.lineTo(boundRight, endPos.y);
            ctx.lineTo(endPos.x + symWidth, endPos.y);
            ctx.lineTo(endPos.x + symWidth, endPos.y + hexLetter.height);
            ctx.lineTo(boundLeft, endPos.y + hexLetter.height);
            ctx.lineTo(boundLeft, startPos.y + hexLetter.height);
            ctx.lineTo(startPos.x, startPos.y + hexLetter.height);
        }

        function drawSection(ctx, start, end, color, fillColor)
        {
            if (!fillColor) fillColor = setAlpha(color, 0.1)

            ctx.fillStyle    = fillColor
            ctx.strokeStyle  = color
            ctx.lineWidth = 2

            if (start.x + start.y * 16 > end.x + end.y * 16){
                var t = end
                end = start
                start = t
            }

            var startPos = hexPos(start)
            var endPos   = hexPos(end)

            var startTextPos = textPos(start)
            var endTextPos   = textPos(end)

            if (start.y === end.y){
                ctx.rect(startPos.x, startPos.y, endPos.x + hexLetter.width * 3 - startPos.x, hexLetter.height)

                ctx.rect(startTextPos.x, startTextPos.y, endTextPos.x + hexLetter.width - startTextPos.x, hexLetter.height)
            } else if (start.y === end.y - 1 && start.x > end.x) {
                ctx.rect(startPos.x, startPos.y, colHex4.x + colHex4.width - startPos.x, hexLetter.height)
                ctx.rect(colHex1.x, endPos.y, endPos.x + hexLetter.width * 3 - colHex1.x, hexLetter.height)

                ctx.rect(startTextPos.x, startTextPos.y, (16-start.x) * hexLetter.width, hexLetter.height)
                ctx.rect(colAscii.x, endTextPos.y, endTextPos.x + hexLetter.width - colAscii.x, hexLetter.height)
            } else {
                ctx.beginPath()
                drawSectionPoly(ctx, startPos, endPos, colHex1.x, colHex4.x + colHex4.width, hexLetter.width * 3)
                ctx.closePath();
                drawSectionPoly(ctx, startTextPos, endTextPos, colAscii.x, colAscii.x + colAscii.width, hexLetter.width)
                ctx.closePath();
            }

            ctx.fill();
            ctx.stroke();
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset()

            if (selection && selEnd.x >= 0)
                drawSection(ctx, selStart, selEnd, Qt.rgba(1,0,0,1))
        }
    }

    ListView {
        id: hexList
        anchors.fill: parent
        model: fileModel
        //model: 50
        delegate: HexRow {
            addressText: address
            //property int index: modelData
            //property int index: type
        }
        interactive: false

        ScrollBar.vertical: ScrollBar { id: scrollBar }

        header: Item { width: 1; height: 16 }
        footer: Item { width: 1; height: hexList.height / 2 }

        MouseArea {
            anchors.fill: parent
            onWheel: {
                if (wheel.angleDelta.y < 0)
                    scrollBar.increase()
                else
                    scrollBar.decrease()
                wheel.accepted = true
            }

            property bool selectText: false

            onPressed: {
                var p = hexAt(mouse.x, mouse.y)
                if (!p)
                    p = textAt(mouse.x, mouse.y)

                if (p) {
                    selectText = true
                    selStart = p
                    selEnd = Qt.point(-1,-1)
                    selection = true
                } else {
                    selStart = Qt.point(-1,-1)
                    selEnd = Qt.point(-1,-1)
                    selection = false
                }

                sectionDrawer.requestPaint()
            }

            onPositionChanged: {
                if (!selection) return

                var p = hexAt(mouse.x, mouse.y)
                if (!p) {
                    p = textAt(mouse.x, mouse.y)
                    if (!p) return
                }

                if (selEnd !== p){
                    selEnd = p
                    sectionDrawer.requestPaint()
                }
            }

            onReleased: {
                selection = false
            }
        }
    }
}
