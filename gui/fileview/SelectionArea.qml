import QtQuick 2.12

MouseArea {
    anchors.fill: parent
    acceptedButtons: Qt.LeftButton
    propagateComposedEvents: true
    hoverEnabled: true

    onPressed: {
        var p = symAt(mouse)
        if (p) {
            var address = pointToAddress(p)
            pressAddress = address
            cursor.offset = address
        } else {
            pressAddress = -1
            selection.reset()
        }
    }

    onDoubleClicked: {
        var p = symAt(mouse)
        if (p) {
            var address = pointToAddress(p)
            console.log(address)
        }
    }

    onPositionChanged: {
        if (mouse.x < 0 || mouse.x > width) return

        var p = symAt(mouse)
        if (!p) return

        var address = pointToAddress(p)

        if (pressed) {
            selection.begin = Math.min(pressAddress, address)
            selection.end = Math.max(pressAddress, address)
        } else {
            var section = currentFile.structure.getByAddress(address)
            if (section) {
                label.showSection(section)
            } else {
                label.visible = false
            }
        }
    }

    SectionLabel {
        id: label
    }
}
