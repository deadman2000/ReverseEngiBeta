import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ColumnLayout {
    TextField {
        id: tfSize
        Layout.fillWidth: true
        placeholderText: 'Size'
        validator: IntValidator { }
    }

    function validate(){
        return tfSize.acceptableInput
    }

    function attrs(){
        return {
            "size": parseInt(tfSize.text)
        }
    }

    function fillForm(obj)
    {
        tfSize.text = obj['size']
    }
}
