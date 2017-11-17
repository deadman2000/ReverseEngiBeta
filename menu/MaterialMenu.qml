import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

// https://material.io/guidelines/components/menus.html#menus-specs

Menu {
    background: Pane {
        Material.background: "white"
        implicitWidth: 200
        implicitHeight: 24
        Material.elevation: 8
    }
}
