import QtQuick 2.15

GridView {
    id: grid
    property var mainParent: parent
    flow: GridView.FlowTopToBottom

    property int size: parent.width < parent.height ? parent.width : parent.height
    property int cellSize: size / 9
    property int selectedRow: - 1
    property int selectedColumn: - 1
    property int moveFromRow: - 1
    property int moveFromColumn: - 1
    property int moveToRow: - 1
    property int moveToColumn: - 1
    property bool ballSelected: false

    width: size
    height: size

    cellWidth: cellSize
    cellHeight: cellSize

    model: gridItemModel

    function getBallPositionToFiled(row, column, cellWidth) {
        return {x: column * cellWidth, y: row * cellWidth}
    }

    function getBallColor(type) {
        switch (type) {
        case 1:
            return "red";
        case 2:
            return "green";
        case 3:
            return "blue";
        case 4:
            return "purple";
        default:
            return "black";
        }
    }

    delegate: MouseArea {
        id: delegateItem
        required property int type
        required property string source
        required property int row
        required property int column
        property bool ballSelected: false

        property int cellSize: cellWidth + 1

        width: cellSize
        height: cellSize

        onSourceChanged: {
            if (source === "born") {
                riseAnim.start()
            } else  if (source === "remove") {
                fallAnim.start()
            }
        }

        onBallSelectedChanged: {
            grid.ballSelected = ballSelected
            if (ballSelected) {
                jumpAnim.start()
            } else {
                jumpAnim.stop()
                handleMoveEvent()
            }
        }

        function handleMoveEvent() {
            if (moveFromRow < 0) return
            if (moveFromColumn < 0) return
            if (moveToRow < 0) return
            if (moveToColumn < 0) return

            moveAnimation.prepareAndStart(moveToColumn, moveToRow, moveFromColumn, moveFromRow)

            moveFromRow = -1
            moveFromColumn = -1
            moveToRow = -1
            moveToColumn = -1
        }

        onClicked: {
            if (selectedRow === row && selectedColumn === column || type === 0) {
                if (grid.ballSelected && type === 0) {
                    moveFromRow = selectedRow
                    moveFromColumn = selectedColumn
                    moveToRow = row
                    moveToColumn = column
                }

                selectedRow = -1
                selectedColumn = -1
            } else {
                selectedRow = row
                selectedColumn = column
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "#f7f7f7"
            border.width: 1
            border.color: "#959595"
        }

        Item {
            id: ball
            visible: type > 0
            width: cellSize
            height: cellSize

            Rectangle {
                anchors.centerIn: parent
                property int sizeRect: parent.width * 0.8
                width: sizeRect
                height: sizeRect
                color: getBallColor(type)
                radius: width / 0.5
                clip: true
            }

            Image {
                property int sizeRect: parent.width * 0.95
                width: sizeRect
                height: sizeRect
                anchors.centerIn: parent
                source: "ball.png"
            }

            PropertyAnimation {
                id: riseAnim
                easing.type: Easing.OutCirc
                target: ball
                property: "scale"
                from: 0
                to: 1
                duration: 300
                running: false
            }

            PropertyAnimation {
                id: fallAnim
                easing.type: Easing.OutCirc
                target: ball
                property: "scale"
                from: 1
                to: 0
                duration: 300
                running: false

                onRunningChanged: {
                    if (!running) {
                        gridItemModel.forgetBall(column, row)
                        ball.scale = 1
                    }
                }
            }

            PathAnimation {
                id: moveAnimation
                property int moveToX: 0
                property int moveToY: 0

                property int toColumn: 0
                property int toRow: 0
                property int fromColumn: 0
                property int fromRow: 0

                duration: 300
                running: false
                target: ball

                function prepareAndStart(moveToColumn, moveToRow, moveFromColumn, moveFromRow) {
                    toColumn = moveToColumn
                    toRow = moveToRow
                    fromColumn = moveFromRow
                    fromColumn = moveFromColumn
                    fromRow = moveFromRow

                    var posTo = getBallPositionToFiled(moveToRow, moveToColumn, cellWidth)
                    moveToX = posTo.x
                    moveToY = posTo.y

                    var posFrom = getBallPositionToFiled(row, column, cellWidth)
                    ball.parent = mainParent
                    ball.y = posFrom.y
                    ball.x = posFrom.x
                    start()
                }

                path: Path {
                    PathCurve { x: moveAnimation.moveToX; y: moveAnimation.moveToY}
                }

                onRunningChanged: {
                    if (!running) {
                        ball.parent = delegateItem
                        ball.y = 0
                        ball.x = 0
                        gridItemModel.moveBall(fromColumn, fromRow, toColumn, toRow);
                        gridItemModel.updateData()
                    }
                }
            }

            SequentialAnimation {
                id: jumpAnim
                loops: Animation.Infinite
                running: false
                PropertyAnimation {target: ball; property: "y"; easing.type: Easing.OutBack; from: 5; to: - 5; duration: 300}
                PropertyAnimation {target: ball; property: "y"; to: 5; from: - 5; duration: 300}

                onRunningChanged: {
                    if (!running) {
                        ball.x = 0
                        ball.y = 0
                    }
                }
            }

            states: State {
                when: selectedRow === delegateItem.row && selectedColumn === delegateItem.column
                PropertyChanges {
                    target: delegateItem
                    ballSelected: true
                }
            }
        }
    }
}
