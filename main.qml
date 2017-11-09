import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import ReverseEngi 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Hello World")

    id: fileItem

    header: ToolBar {
        Flow {
            id: flow
            width: parent.width
        }
    }

    HexText {
        id: hexLetter
        text: ' '
    }

    FileModel {
        id: fileModel
    }

    Component.onCompleted: fileModel.openFile('c:/Windows/RtlExUpd.dll')

    property AddressRange selection: AddressRange
    {
        color: Qt.rgba(1, 0, 0, 0.1)
        borderColor: Qt.rgba(1, 0, 0)
        borderWidth: 2
    }

    function addressToPoint(address)
    {
        var y = Math.floor(address / 16)
        return Qt.point(address - y * 16, y)
    }

    function pointToAddress(point)
    {
        return point.y * 16 + point.x
    }

    property int rowHeight: 32
    property int listTopPadding: 8

    property int topRow: 0
    property int listContentY: topRow * rowHeight - listTopPadding
    readonly property real scrollPosition: topRow / (fileModel.rows - rowsInScreen)

    readonly property int rowsInScreen: listsItem.height / rowHeight

    function setScroll(value) {
        topRow = value * (fileModel.rows - rowsInScreen)
    }

    function shiftContentY(value){
        topRow = Math.max(Math.min(topRow + value, fileModel.rows - rowsInScreen), 0)
    }

    function pageUp(){
        shiftContentY(-rowsInScreen)
    }

    function pageDown(){
        shiftContentY(rowsInScreen)
    }

    property int pressAddress: -1

    Item {
        id: listsItem
        anchors.fill: parent
        focus: true

        MouseArea {
            anchors.fill: parent

            onWheel: {
                if (wheel.angleDelta.y < 0)
                    shiftContentY(3)
                else
                    shiftContentY(-3)

                wheel.accepted = true
            }
        }

        Row {
            anchors.fill: parent
            spacing: 0

            Item {
                width: 16
                height: 1
            }

            AddressList {
                model: fileModel
                contentY: listContentY
            }

            Item {
                width: 32
                height: 1
            }

            HexList {
                model: fileModel
                contentY: listContentY
            }

            Item {
                width: 32
                height: 1
            }

            TextList {
                model: fileModel
                contentY: listContentY
            }
        }

        Keys.onPressed: {
            switch (event.key)
            {
            case Qt.Key_Return:
            case Qt.Key_Enter:
                console.log('enter')
                break

            case Qt.Key_Escape:
                selection.reset()
                break

            case Qt.Key_Left:
                cursor.moveLeft()
                break
            case Qt.Key_Right:
                cursor.moveRight()
                break
            case Qt.Key_Up:
                cursor.moveUp()
                break
            case Qt.Key_Down:
                cursor.moveDown()
                break
            case Qt.Key_PageUp:
                cursor.movePageUp()
                break
            case Qt.Key_PageDown:
                cursor.movePageDown()
                break
            case Qt.Key_Home:
                cursor.moveBegin()
                break
            case Qt.Key_End:
                cursor.moveEnd()
                break;

            default:
                return;
            }

            event.accepted = true
        }

        ScrollBar {
            id: scrollBar
            size: Math.max( rowsInScreen / fileModel.rows, 16 / height)
            visible: size < 1
            anchors.right: parent.right
            height: parent.height
            width: 16
            policy: ScrollBar.AlwaysOn
            interactive: true

            Binding {
                target: scrollBar
                property: "position"
                value: scrollPosition * (1 - scrollBar.size)
                when: !scrollBar.pressed
            }

            onPositionChanged: if (pressed) setScroll(position / (1 - size))
        }
    }

    QtObject {
        id: cursor
        property int offset: 0
        property point pos: addressToPoint(offset)
        onPosChanged: ensureVisible()

        function moveLeft(){
            if (offset == 0) return
            offset--
        }

        function moveRight(){
            if (offset == fileModel.size) return
            offset++
        }

        function moveUp(){
            offset = Math.max(0, offset - 16)
        }

        function moveDown(){
            offset = Math.min(fileModel.size - 1, offset + 16)
        }

        function movePageUp(){
            offset = Math.max(0, offset - 16 * rowsInScreen)
        }

        function movePageDown(){
            offset = Math.min(fileModel.size - 1, offset + 16 * rowsInScreen)
        }

        function moveBegin(){
            offset = 0
        }

        function moveEnd(){
            offset = fileModel.size - 1
        }

        function ensureVisible(){
            if (pos.y < topRow)
                topRow = pos.y
            else if (pos.y >= topRow + rowsInScreen)
                topRow = pos.y - rowsInScreen + 1
        }
    }

    DropArea {
        anchors.fill: parent
        onEntered: {
            if (drag.hasUrls){
                // TODO проверку на isFile и fileExists
                drag.accept(Qt.CopyAction);
            }
        }
        onDropped: {
            if (drop.hasUrls){
                fileModel.openFile(drop.urls)
            }
        }
    }
}
