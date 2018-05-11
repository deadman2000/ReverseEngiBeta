import QtQuick 2.7

Item {
    anchors.fill: parent

    property real borderWidth: 1
    property real borderPadding: 1

    Repeater {
        model: document.sections
        delegate: SectionRect {
            section: modelData
        }
    }

    SectionRect {
        section: selection
    }
}
