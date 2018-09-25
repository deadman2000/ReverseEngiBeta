import QtQuick 2.7

Item {
    width: list.contentItem.childrenRect.width
    height: parent.height

    property real textPadding: G.hexLetter.width / 2

    property real symWidth: G.hexLetter.width * 2
    property real cellWidth: G.hexLetter.width * 3

    function symPos(coord)  // Координаты символа
    {
        return Qt.point(Math.round(coord.x * (G.hexLetter.width*3) + textPadding),
                        Math.round(coord.y * G.rowHeight - contentY))
    }

    function symAt(point)  // Символ по координатам
    {
        var x = point.x + list.contentX - textPadding
        var y = point.y + list.contentY

        // Номер символа в строке
        var sym = Math.round(x / (G.hexLetter.width*3) - 0.5)
        if (sym > 15) sym = 15

        var it = list.itemAt(x, y)
        if (it === null) return null
        return Qt.point(sym, it.rowIndex)
    }

    property alias model: list.model
    property alias contentY: list.contentY

    SectionDrawer {
        parent: list.contentItem
    }

    ListView {
        id: list
        anchors.fill: parent
        interactive: false
        spacing: 0

        header: Item { height: listTopPadding; width: 1 }

        delegate:
            HexText {
                text: hex
                height: G.rowHeight
                leftPadding: textPadding
                rightPadding: textPadding

                property int rowIndex: index
            }

        footer: Item { height: list.height; width: 1 }
    }

    CursorRect { }

    SelectionArea { }
}
