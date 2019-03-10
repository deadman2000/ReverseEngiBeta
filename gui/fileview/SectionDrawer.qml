import QtQuick 2.12

Item {
    id: drawer
    anchors.fill: parent

    property real borderWidth: 1
    property real borderPadding: 1

    z: 1000

    Repeater {
        model: document.sections
        delegate: SectionRect {
            section: modelData
            z: containsMouse ? 1 : 0

            SectionLabel {
                id: label
                text: section.name
                anchors.centerIn: bindTarget
                visible: containsMouse //|| (section.name.length > 0 && bindTarget.width >= width)
            }
        }
    }

    SectionRect {
        section: selection
    }
}
