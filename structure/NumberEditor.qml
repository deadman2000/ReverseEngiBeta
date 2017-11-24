import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout {
    ComboBox {
        flat: true
        Layout.fillWidth: true
        model: ["byte", "short", "int", "long"]
    }

    ComboBox {
        flat: true
        Layout.fillWidth: true
        model: [qsTr("Signed"), qsTr("Unsigned")]
    }

    ComboBox {
        flat: true
        Layout.fillWidth: true
        model: [qsTr("Big-Endian"), qsTr("Little-Endian")]
    }
}
