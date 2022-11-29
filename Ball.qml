import QtQuick 2.15

Item {
    property string ballColor: "orange"
    property int size: 40
    width: size
    height: size

    Rectangle {
        anchors.centerIn: parent
        property int sizeRect: size * 0.8
        width: sizeRect
        height: sizeRect
        color: ballColor
        radius: width / 0.5
    }

    Image {
        property int sizeRect: size * 0.95
        width: sizeRect
        height: sizeRect
        anchors.centerIn: parent
        source: "ball.png"
    }
}
