import QtQuick 2.7

Item {
    width: list.contentItem.childrenRect.width
    height: parent.height

    property alias model: list.model
    property alias contentY: list.contentY

    function rowAt(point)  // Строка по координатам
    {
        var x = point.x + list.contentX
        var y = point.y + list.contentY

        var it = list.itemAt(x, y)
        if (it === null) return null
        return it.rowIndex
    }

    ListView {
        id: list
        interactive: false
        anchors.fill: parent
        spacing: 0

        header: Item { height: listTopPadding; width: 1 }

        delegate:
            HexText {
                text: address
                height: rowHeight

                property int rowIndex: index
            }
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            var row = rowAt(mouse)
            if (row !== null) {
                var address = row * 16
                pressAddress = address
                cursor.offset = address
            } else {
                pressAddress = -1
                selection.reset()
            }
        }

        onPositionChanged: {
            if (!pressed) return
            if (mouse.x < 0 || mouse.x > width) return

            var row = rowAt(mouse)
            if (row === null) return

            var address = row * 16 + 15
            selection.begin = Math.min(pressAddress, address)
            selection.end = Math.max(pressAddress, address)
        }
    }
}
