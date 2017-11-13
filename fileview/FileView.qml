import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ReverseEngi 1.0

Item {
    id: fileView

    property AddressRange selection: AddressRange
    {
        color: Qt.rgba(1, 0, 0, 0.1)
        borderColor: Qt.rgba(1, 0, 0)
        borderWidth: 2
    }

    property alias fileModel: fileModel
    property alias cursor: cursor

    FileModel {
        id: fileModel
    }

    function openFile(path)
    {
        cursor.offset = 0
        selection.reset()
        fileModel.openFile(path)
    }

    property int topRow: 0
    property int listContentY: topRow * rowHeight - listTopPadding
    readonly property real scrollPosition: topRow / (fileModel.rows - rowsInScreen)

    readonly property int rowsInScreen: height / rowHeight


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

    function markSelection()
    {
        if (!selection.isSet) return

        fileModel.addSection(selection.begin, selection.end)
        selection.reset()
    }

    function copyToClipboard()
    {
        fileModel.copyToClipboard(selection.begin, selection.end);
    }

    property int pressAddress: -1

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton

        onWheel: {
            if (wheel.angleDelta.y < 0)
                shiftContentY(3)
            else
                shiftContentY(-3)

            wheel.accepted = true
        }

        onClicked: {
            if (mouse.button == Qt.RightButton) {
                menu.x = mouse.x
                menu.y = mouse.y
                menu.open()
                mouse.accepted = true
            }
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
        if (event.modifiers == Qt.NoModifier){
            switch (event.key)
            {
            case Qt.Key_Return:
            case Qt.Key_Enter:
                markSelection()
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
        } else if (event.modifiers == Qt.ControlModifier){
            switch (event.key)
            {
            case Qt.Key_C:
                copyToClipboard();
            }
        }

        event.accepted = true
    }

    QtObject {
        id: cursor
        property int offset: 0
        readonly property point pos: addressToPoint(offset)
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

    Menu {
        id: menu

        MenuItem {
            text: "Mark selection"
            enabled: selection.isSet
            onTriggered: markSelection()
        }

        MenuItem {
            text: "Copy"
            enabled: selection.isSet
            onTriggered: copyToClipboard()
        }

        MenuItem {
            text: "Paste"
            onTriggered: console.log('Paste')
        }

        MenuSeparator { }

        MenuItem {
            text: "More Stuff"
            onTriggered: moreMenu.open()

            Menu {
                x: parent.width
                id: moreMenu
                MenuItem {
                    text: "Do Nothing"
                }
            }
        }
    }
}
