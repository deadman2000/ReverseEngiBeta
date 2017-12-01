import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout {
    ComboBox {
        id: cbSize
        flat: true
        Layout.fillWidth: true
        model: ["byte", "short", "int", "long"]
    }

    function getSize()
    {
        switch (cbSize.currentIndex)
        {
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default: console.error('not implemented size'); return -1;
        }
    }

    ComboBox {
        id: cbSigned
        flat: true
        Layout.fillWidth: true
        model: [qsTr("Signed"), qsTr("Unsigned")]
    }

    ComboBox {
        id: cbEndian
        flat: true
        Layout.fillWidth: true
        model: [qsTr("Big-Endian"), qsTr("Little-Endian")]
    }

    function validate(){
        return true;
    }

    function attrs(){
        return {
            "size": getSize(),
            "signed": cbSigned.currentIndex == 0,
            "bigendian": cbEndian.currentIndex == 0
        }
    }

    function fillForm(obj)
    {
        var size = obj['size']
        switch (size)
        {
        case 1: cbSize.currentIndex = 0; break;
        case 2: cbSize.currentIndex = 1; break;
        case 4: cbSize.currentIndex = 2; break;
        case 8: cbSize.currentIndex = 3; break;
        }

        cbSigned.currentIndex = obj['signed'] ? 0 : 1
        cbEndian.currentIndex = obj['bigendian'] ? 0 : 1
    }
}
