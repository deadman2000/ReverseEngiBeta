import QtQuick 2.0
import QtQuick.Layouts 1.3

Row {
    spacing: 0
    height: 32

    property alias addressText: addressLabel.text
    property alias hexText: hexLabel.text
    property alias asciiText: textLabel.text

    Item {
        width: 16
        height: 1
    }

    HexText {
        id: addressLabel
        text: '0000 0000'
    }

    Item {
        width: 32
        height: 1
    }

    HexText {
        id: hexLabel
        text: '00 99 00 0A BC DE F0 00  01 02 03 04 05 06 07 08'
    }

    Item {
        width: 32
        height: 1
    }

    HexText {
        id: textLabel
        text: '................'
    }
}
