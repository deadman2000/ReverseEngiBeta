import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Flickable {
    id: tree
    property alias model: rootRepeater.model
    property var selectedRow

    property int rowHeight: 24
    property int rowSpacing: 0
    property int iconSize: 24

    property bool _dragMode: false

    ScrollBar.vertical: ScrollBar { }
    contentHeight: contentColumn.height

    boundsBehavior: Flickable.StopAtBounds

    function expandAll() {
        for (var i=0; i<rootRepeater.count; i++)
            rootRepeater.itemAt(i).expand()
    }

    Component.onCompleted: expandAll()

    Rectangle {
        id: dropPlacer
        z: 2
        height: 2
        color: Material.accent
        radius: 2
        visible: false

        property var target: null
        function bind(item, toUp)
        {
            var p = tree.mapFromItem(item, 0, toUp ? 0 : item.height)
            x = p.x
            y = p.y
            width = item.width
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
            onRowExpandedChanged: if (!expanderLoader.item) expanderLoader.sourceComponent = expanderComponent

            function expand() {
                if (!node.children || !node.children.hasChilds) return;

                rowExpanded = true
            }

            function expandAll() {
                if (!node.children || !node.children.hasChilds) return;

                rowExpanded = true
                expanderLoader.item.expandAll()
            }

            function toggle() {
                if (!node.children || !node.children.hasChilds) return;

                rowExpanded = !rowExpanded
            }

            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined

            MouseArea {
                id: rowMouse
                height: rowHeight
                width: parent.width

                property bool held: false

                drag.target: held ? dragTarget.item : undefined
                drag.axis: Drag.XAndYAxis
                drag.threshold: 8
                pressAndHoldInterval: 1

                drag.onActiveChanged: dropPlacer.visible = drag.active

                onPressAndHold: held = true
                onPressed: select()
                onReleased: {
                    drag.target.Drag.drop()
                    held = false
                }
                onDoubleClicked: toggle()

                DropArea {
                    id: dropArea
                    anchors.fill: parent
                    onDropped: {
                        dropPlacer.visible = false

                        var source = drop.source.node
                        if (source === node) return
                        //console.log('moving', source.text, isUp ? 'before' : 'after', node.text)

                        if (isUp)
                            source.moveBefore(node)
                        else
                            source.moveAfter(node)
                    }

                    property bool isUp: drag.y < height / 2
                    onIsUpChanged: if (containsDrag) dropPlacer.bind(rowItem, isUp)
                    onContainsDragChanged: if (containsDrag) dropPlacer.bind(rowItem, isUp)
                }

                function select() {
                    selectedRow = rowMouse
                }

                Rectangle { // Background
                    color: selectedRow === rowMouse ? Qt.rgba(0, 0, 0, 0.1) : 'transparent'
                    anchors.fill: parent
                }

                RowLayout {
                    spacing: rowSpacing
                    height: rowHeight
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: rowLevel * 32

                    Item {
                        width: iconSize; height: iconSize

                        Image {
                            visible: node.children && node.children.hasChilds

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
                readonly property var subNodeChildren: node.children
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
                    model: subNodeChildren
                    delegate: rowComponent
                }
            }
        }
    }
}
