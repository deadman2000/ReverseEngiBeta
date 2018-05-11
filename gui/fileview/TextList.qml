import QtQuick 2.7

Item {
    width: list.contentItem.childrenRect.width
    height: parent.height

    property real symWidth: G.hexLetter.width
    property real cellWidth: symWidth

    function symPos(coord)
    {
        return Qt.point(Math.round(coord.x * symWidth),
                        Math.round(coord.y * rowHeight - list.contentY))
    }

    function symAt(point)  // Символ по координатам
    {
        var x = point.x + list.contentX
        var y = point.y + list.contentY

        // Номер символа в строке
        var sym = Math.round(x / G.hexLetter.width - 0.5)
        if (sym > 15) sym = 15

        var it = list.itemAt(x, y)
        if (it === null) return null
        return Qt.point(sym, it.rowIndex)
    }

    property alias model: list.model
    property alias contentY: list.contentY

    SectionDrawer {
        parent: list.contentItem
        borderPadding: 0
    }

    ListView {
        id: list
        interactive: false
        anchors.fill: parent
        spacing: 0

        header: Item { height: listTopPadding; width: 1 }

        delegate:
            HexText {
                text: fileText
                height: rowHeight

                property int rowIndex: index
            }

        footer: Item { height: list.height; width: 1 }
    }

    CursorRect { }

    SelectionArea { }
}
