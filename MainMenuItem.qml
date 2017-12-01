import QtQuick 2.7
import QtQuick.Controls 2.2

ToolButton {
    id: button
    default property list<MenuItem> contentData

    onPressed: if (contentData.length > 0) {
                   menuLoader.item.open()
               }

    Loader {
        id: menuLoader
        active: contentData.length > 0
        sourceComponent: menuComp
        onLoaded: item.contentData = button.contentData
    }

    Component {
        id: menuComp
        Menu {
            id: subMenu
            transformOrigin: Item.TopLeft
            enter: Transition {
                NumberAnimation { property: "y"; from: 0; to: button.height; easing.type: Easing.Linear; duration: 100 }
                NumberAnimation { property: "scale"; from: 0.9; to: 1.0; easing.type: Easing.OutQuint; duration: 220 }
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; easing.type: Easing.OutCubic; duration: 150 }
            }
        }
    }
}
