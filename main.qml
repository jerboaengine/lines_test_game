import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    width: 480
    height: 640
    visible: true
    title: "color lines test demo"

    PlayingField {
        id: playingField

    }

    Text {
        id: scoreTitle
        y: playingField.height + 10
        x: 10
        font.pixelSize: 30

        RotationAnimator on rotation {
            id: animation
            from: 0;
            to: 360;
            duration: 300
            running: false
        }
    }

    Text {
        id: gameOverMes
        color: "orange"
        font.bold: true
        anchors.centerIn: playingField
        text: "Game Over"
        font.pixelSize: 60
        visible: false
    }

    Button {
        height: 100
        width: 300
        font.pixelSize: 30
        text: "New Game"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        onClicked: agent.newGameRequest()
    }

    Connections {
        target: agent
        function onUpdateScoreEvent(score) {
            var scoreText = "Socore: " + score
            if (scoreTitle.text !== scoreText) {
                scoreTitle.text = scoreText
                animation.start()
            }

            if (score === 0) {
                gameOverMes.visible = false
            }
        }

        function onGameOverEvent() {
            gameOverMes.visible = true
        }
    }
}
