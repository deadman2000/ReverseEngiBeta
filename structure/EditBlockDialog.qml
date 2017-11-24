import QtQuick 2.7
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Window {
    id: dialog
    title: "Edit block"
    width: 400
    height: 400
    modality: Qt.WindowModal

    Component.onCompleted: typeEditor.updateContent()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 32

            TextField {
                placeholderText: "Name"
                Layout.fillWidth: true
            }

            ComboBox {
                id: cbType
                flat: true
                Layout.fillWidth: true
                model: ["Number", "Text", "Blob", "Sector"]
                onActivated: typeEditor.updateContent()
            }

            Loader {
                id: typeEditor
                Layout.fillWidth: true

                function updateContent()
                {
                    switch (cbType.currentIndex)
                    {
                    case 0:
                        source = 'NumberEditor.qml'
                        break;
                    case 1:
                        source = 'TextEditor.qml'
                        break;
                    case 2:
                        source = 'BlobEditor.qml'
                        break;
                    case 3:
                        source = 'SectorEditor.qml'
                        break;
                    default:
                        source = ''
                        break;
                    }
                }

            }

            Item {
                Layout.fillHeight: true
                width: 1
            }
        }

        DialogButtonBox {
            Layout.fillWidth: true
            standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel
            onAccepted: if (validate()) dialog.close()
            onRejected: dialog.close()
        }
    }

    function validate()
    {
        return true;
    }
}
