import QtQuick 2.0
import QtQuick.Layouts 1.3

Row {
    spacing: 0
    height: 32

    property alias addressText: addressLabel.text
    property alias hexText: hexLabel.text
    property alias asciiText: textLabel.text

    property int rowIndex

    Item {
        width: 16
        height: 1
    }

    HexText {
        id: addressLabel
    }

    Item {
        width: 32
        height: 1
    }

    HexText {
        id: hexLabel
    }

    Item {
        width: 32
        height: 1
    }

    HexText {
        id: textLabel
    }
}
