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

    property bool _editMode: false

    property var structure: currentFile.structure

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 32

            TextField {
                id: teName
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
                onLoaded: if (formData) item.fillForm(formData)

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
            onAccepted: {
                if (!validate() || !typeEditor.item.validate()) return;

                var typeId = cbType.currentIndex
                var attr = typeEditor.item.attrs()
                attr['name'] = teName.text.trim()

                if (_editMode)
                    changeBlock(typeId, attr)
                else
                    addBlock(typeId, attr)
                dialog.close()
            }
            onRejected: dialog.close()
        }
    }

    function validate()
    {
        return teName.text.trim().length > 0
    }

    property var formData: null

    function fillForm(obj)
    {
        formData = obj
        _editMode = true
        teName.text = obj['name']
        cbType.currentIndex = obj['type']
        typeEditor.updateContent()
        typeEditor.item.fillForm(obj)
    }
}
