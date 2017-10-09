import QtQuick 2.0
import QtQuick.Layouts 1.3

Row {
    spacing: 4
    height: 32

    HexText {
        text: ' 0000 0000 '
    }

    Rectangle {
        color: 'black'
        width: 2
        height: parent.height
    }

    HexText {
        text: ' 00 99 00 0A BC DE F0 00  01 02 03 04 05 06 07 08 '
    }

    Rectangle {
        color: 'black'
        width: 2
        height: parent.height
    }

    HexText {
        text: '................'
    }
}
