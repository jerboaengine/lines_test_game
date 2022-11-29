import QtQuick 2.15

GridView {
    id: grid
    property var mainParent: parent
    flow: GridView.FlowTopToBottom

    property int size: parent.width < parent.height ? parent.width : parent.height
    property int cellSize: size / 9
    property int delegateSize: cellSize + 1
    property int selectedRow: - 1
    property int selectedColumn: - 1
    property bool ballSelected: false
    property bool movAnimationRunning: false

    width: size
    height: size

    cellWidth: cellSize
    cellHeight: cellSize

    model: gridItemModel

    function getBallPositionOnField(column, row, cellWidth) {
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

        width: delegateSize
        height: delegateSize

        onClicked: {
            if (selectedRow === row && selectedColumn === column || type === 0) {
                if (ballSelected && type === 0) {
                    agent.moveRequest(selectedColumn, selectedRow, column, row);
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

        Ball {
            id: ball
            visible: type > 0
            size: delegateSize
            ballColor: getBallColor(type)

            PropertyAnimation {
                id: riseAnim
                easing.type: Easing.OutCirc
                target: ball
                property: "scale"
                from: 0
                to: 1
                duration: 300
                running: source === "born"
            }

            PropertyAnimation {
                id: fallAnim
                easing.type: Easing.OutCirc
                target: ball
                property: "scale"
                from: 1
                to: 0
                duration: 300
                running: source === "remove" && !moveAnimatio.running

                onRunningChanged: {
                    if (!running) {
                        gridItemModel.forgetBall(column, row)
                        ball.scale = 1
                    }
                }
            }

            SequentialAnimation {
                id: jumpAnim
                loops: Animation.Infinite
                running: selectedRow === row && selectedColumn === column
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
                    target: grid
                    ballSelected: true
                }
            }
        }
    }

    Ball {
        visible: false
        id: moveableBall
        ballColor: "red"
        size: delegateSize
        property int moveToX: 0
        property int moveToY: 0
        property var finishAnimateCallback

        PathAnimation {
            id: moveAnimatio
            target:moveableBall
            path: Path {
                PathCurve { x: moveableBall.moveToX; y: moveableBall.moveToY}
            }

            onRunningChanged: {
                if (!running) {
                    moveableBall.finishAnimateCallback()
                }
            }
        }
    }

    function playMoveBallAnimation(columnFrom, rowFtom, columnTo, rowTo, typeBall, finishAnimateCallback) {
        var posFrom = getBallPositionOnField(columnFrom, rowFtom, cellSize)
        moveableBall.x = posFrom.x
        moveableBall.y = posFrom.y
        moveableBall.ballColor = getBallColor(typeBall)
        var posTo = getBallPositionOnField(columnTo, rowTo, cellSize)
        moveableBall.moveToX = posTo.x
        moveableBall.moveToY = posTo.y
        moveAnimatio.start()
        moveableBall.finishAnimateCallback = finishAnimateCallback
    }


    Connections {
        target: agent
        function onMoveEvent(columnFrom, rowFtom, columnTo, rowTo) {
            var typeMovedBall = gridItemModel.getTypeBall(columnFrom, rowFtom);
            moveableBall.visible = true
            playMoveBallAnimation(columnFrom, rowFtom, columnTo, rowTo, typeMovedBall, () => {
                moveableBall.visible = false
                gridItemModel.insertBall(columnTo, rowTo, typeMovedBall)
            })
            gridItemModel.removeBall(columnFrom, rowFtom);
        }

        function onBornEvent(column, row, type) {
            gridItemModel.addNewBall(column, row, type)
        }

        function onDeathEvent(column, row) {
            gridItemModel.markAsRemoveBall(column, row)
        }
    }
}
