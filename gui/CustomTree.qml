import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Flickable {
    id: tree
    property alias model: rootRepeater.model
    property var selectedNode

    property int rowHeight: 24
    property int rowSpacing: 0
    property int iconSize: 24

    property bool allowDrag: false

    ScrollBar.vertical: ScrollBar { }
    contentHeight: contentColumn.height

    signal treeChanged
    signal doubleClicked

    boundsBehavior: Flickable.StopAtBounds

    function expandAll() {
        for (var i=0; i<rootRepeater.count; i++)
            rootRepeater.itemAt(i).expand()
    }

    //Component.onCompleted: expandAll()

    Rectangle {
        id: dropPlacer
        z: 2
        height: 2
        color: Material.accent
        radius: 2
        visible: false

        property var target: null
        function bind(item, target)
        {
            var tx = 0, ty = 0
            if (target === 1) { tx = 32; ty = item.height }
            else if (target === 2) ty = item.height

            var p = tree.mapFromItem(item, tx, ty)
            x = p.x
            y = p.y
            width = item.width - tx
        }

        Rectangle {
            width: 2
            radius: 2
            height: 8
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            color: Material.accent
        }

        Rectangle {
            width: 2
            radius: 2
            height: 8
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            color: Material.accent
        }
    }

    Column {
        id: contentColumn
        spacing: 0
        width: parent.width

        readonly property int childLevel: 0

        Item {
            width: 1
            height: 4
        }

        Repeater {
            id: rootRepeater
            delegate: rowComponent
        }

        Item {
            width: 1
            height: 4
        }
    }

    Component {
        id: dragProxy

        RowLayout {
            id: row
            spacing: rowSpacing
            height: rowHeight
            visible: proxyVisible

            Drag.active: true
            Drag.hotSpot.x: row.width/2
            Drag.hotSpot.y: row.height/2

            property var node: proxyNode

            Image {
                sourceSize: Qt.size(iconSize, iconSize)
                source: node.icon
                opacity: 0.54
                visible: !!node.icon
            }

            Text {
                leftPadding: 8
                text: node.text
                Layout.fillWidth: true
                font.pixelSize: 13
                color: Material.foreground
            }
        }
    }

    Component {
        id: rowComponent

        Column {
            property bool rowExpanded: false
            readonly property int rowLevel: parent ? parent.childLevel : 0
            onRowExpandedChanged: {
                if (!expanderLoader.item) expanderLoader.sourceComponent = expanderComponent
                node.expanded = rowExpanded
            }

            function expand() {
                if (node.rowCount() === 0) return;

                rowExpanded = true
            }

            function expandAll() {
                if (node.rowCount() === 0) return;

                rowExpanded = true
                expanderLoader.item.expandAll()
            }

            function toggle() {
                if (node.rowCount() === 0) return;

                rowExpanded = !rowExpanded
            }

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined

            MouseArea {
                id: rowMouse
                height: rowHeight
                width: parent.width
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                property bool held: false

                drag.target: held ? dragTarget.item : undefined
                drag.axis: Drag.XAndYAxis
                drag.threshold: 8
                pressAndHoldInterval: 1

                drag.onActiveChanged: dropPlacer.visible = drag.active

                onPressAndHold: if (tree.allowDrag) held = true
                onPressed: select()
                onReleased: {
                    if (drag.target)
                        drag.target.Drag.drop()
                    held = false
                }
                onDoubleClicked: {
                    if (mouse.button === Qt.LeftButton){
                        if (node.rowCount() > 0)
                            toggle()
                        else
                            tree.doubleClicked()
                    }
                }

                DropArea {
                    id: dropArea
                    anchors.fill: parent
                    onDropped: {
                        dropPlacer.visible = false

                        var source = drop.source.node // Кого перетаскиваем
                        if (source === node) return

                        if (dropTarget == 0)
                            source.moveBefore(node)
                        else if (dropTarget == 1)
                            node.append(source)
                        else
                            source.moveAfter(node)
                        treeChanged();
                    }

                    property real upRange
                    property real bottomRange
                    onEntered: {
                        var source = drag.source.node // Кого перетаскиваем
                        if (node.canAppend(source)){
                            upRange = height / 3
                            bottomRange = 2 * height / 3
                        } else {
                            upRange = height / 2
                            bottomRange = height / 2
                        }
                    }

                    property int dropTarget: containsDrag ? ((drag.y < upRange) ? 0 : ((drag.y >= bottomRange) ? 2 : 1)) : -1
                    onDropTargetChanged: {
                        if (!containsDrag) return
                        var source = drag.source.node // Кого перетаскиваем
                        dropPlacer.bind(rowItem, dropTarget)
                    }

                    onContainsDragChanged: if (containsDrag) dropPlacer.bind(rowItem, dropTarget)
                }

                function select() {
                    selectedNode = node
                }

                Rectangle { // Background
                    color: selectedNode === node ? Qt.rgba(0, 0, 0, 0.1) : 'transparent'
                    anchors.fill: parent
                }

                RowLayout {
                    spacing: rowSpacing
                    height: rowHeight
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: rowLevel * iconSize

                    Item {
                        width: iconSize; height: iconSize

                        Image {
                            visible: node.count > 0
                            sourceSize: Qt.size(iconSize, iconSize)
                            source: 'qrc:icons/ic_arrow_drop_down_black_24px.svg'
                            opacity: 0.54
                            rotation: rowExpanded ? 0 : -90
                            Behavior on rotation { PropertyAnimation { duration: 100 } }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: toggle()
                            }
                        }
                    }

                    Item {
                        id: rowItem
                        Layout.fillWidth: true
                        height: parent.height

                        RowLayout {
                            anchors.fill: parent
                            spacing: rowSpacing

                            Image {
                                sourceSize: Qt.size(iconSize, iconSize)
                                source: node.icon
                                opacity: 0.54
                                visible: !!node.icon
                            }

                            Text {
                                leftPadding: 8
                                text: node.text
                                Layout.fillWidth: true
                                font.pixelSize: 13
                                color: Material.foreground
                            }
                        }

                        Loader {
                            id: dragTarget
                            sourceComponent: rowMouse.held ? dragProxy : undefined

                            property var proxyNode: node
                            property bool proxyVisible: rowMouse.drag.active

                            states: State {
                                when: rowMouse.held
                                ParentChange { target: dragTarget.item; parent: tree }
                            }
                        }
                    }
                }
            }

            Loader { // Загрузчик для раскрывающейся области
                id: expanderLoader
                width: parent.width
                height: item ? item.height : 0
                clip: true

                readonly property bool expanded: rowExpanded
                readonly property int nextLevel: rowLevel + 1
                readonly property var subNode: node
            }
        }
    }

    Component {
        id: expanderComponent

        Item {
            id: subtreeContent
            width: parent.width
            height: expanded && loaded ? subContentColumn.height : 0
            Behavior on height { NumberAnimation { duration: 150 } }

            property bool loaded: false
            Component.onCompleted: loaded = true

            function expandAll() {
                for (var i=0; i<expanderRepeater.count; i++)
                    expanderRepeater.itemAt(i).expand()
            }

            Column {
                id: subContentColumn
                spacing: 0
                width: parent.width
                readonly property int childLevel: nextLevel

                Repeater {
                    id: expanderRepeater
                    model: subNode
                    delegate: rowComponent
                }
            }
        }
    }
}
