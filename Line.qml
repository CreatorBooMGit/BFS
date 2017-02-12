import QtQuick 2.0
import QtGraphicalEffects 1.0
//import QtQuick.Controls 2.1
import QtQuick.Controls 1.4
import "qrc:/js/line.js" as LineLogic

Rectangle {
    id: root

    property real x1: 0
    property real y1: 0

    property real x2: 0
    property real y2: 0

    property int numEdge : -1

    property bool edgeLighting : false

    property string colorStart : "#B5DEB5"
    property string colorDefault : "red"
    property string colorLighting : "#0DBCE8"

    signal vertexChangedPosition(var vertex)
    signal setVertices(var vertexLeft, var vertexRight)
    signal checkEdge(var vertex)
    signal lightingEdge(var enable)
    signal destroySignal()

    onCheckEdge: {
        if((LineLogic.vertexLeft === vertex || LineLogic.vertexRight === vertex))
            return false;
        return true;
    }

    onVertexChangedPosition: {
        if(LineLogic.vertexLeft === vertex)
        {
            root.x1 = vertex.x + vertex.width / 2;
            root.y1 = vertex.y + vertex.height / 2;
        }
        if(LineLogic.vertexRight === vertex)
        {
            root.x2 = vertex.x + vertex.width / 2;
            root.y2 = vertex.y + vertex.height / 2;
        }
    }

    onSetVertices: {
        LineLogic.setVertices(vertexLeft, vertexRight)
    }

    onLightingEdge: {
        edgeLighting = enable;
    }

    onDestroySignal: {
        LineLogic.destroy()
    }


    x: x1
    y: y1
    height: root.edgeLighting ? 3 : 2
    transformOrigin: Item.Left;
    opacity: 0.5

    width: LineLogic.getWidth(x1, y1, x2, y2)
    rotation: LineLogic.getAngle(x1, y1, x2, y2)

    onRotationChanged: {
//        console.log("rotation:", rotation)
//        if(root.rotation > 90 && root.rotation < 270)
//            text.anchors.top = root.bottom
//        else
//            text.anchors.bottom = root.top
    }

    Component.onCompleted: {
        LineLogic.setRootEdge(root)
    }

    LinearGradient
    {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(root.width, 5)
        gradient: Gradient {
            GradientStop { position: 0.0; color: colorStart }
            GradientStop { position: 0.7; color: root.edgeLighting ? root.colorLighting : root.colorDefault }
//            GradientStop { position: 0.7; color: LineLogic.getRandomColor() }
        }
    }

    smooth: true
    antialiasing: true


    MouseArea {
        id: rootMouseArea

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.ArrowCursor
        acceptedButtons: Qt.RightButton | Qt.LeftButton

        onClicked: {
            LineLogic.lightingEdge()
//            console.log("dadadaa")
            if(mouse.button & Qt.RightButton)
            {
//                console.log("dadadaa2")
                contextEdge.popup()
            }
        }
    }

    Text {
        id: text

        anchors {
            horizontalCenter: root.horizontalCenter
            bottom: root.top
        }

        rotation: (root.rotation > 90 && root.rotation < 270) ? 180 : 0;

        text: "L" + numEdge
    }

    Menu {
        id: contextEdge

        MenuItem {
            text: "Удалить"
            onTriggered: {
                LineLogic.destroy()
            }
        }
        MenuItem {
            text: "Направление"

            //            Menu {
            //                id: directionMenu

            //                MenuItem {
            //                    text: "Направленный"
            //                    checkable: true
            //                    onTriggered: {
            //                        LineLogic.directed = checked
            //                        direction.enabled = checked
            //                    }
            //                }
            //                MenuItem {
            //                    id: direction

            //                    text: {
            //                        if(LineLogic.directed)
            //                            LineLogic.vertexLeft.numVertex + " -> " + LineLogic.vertexRight.numVertex
            //                        else
            //                            LineLogic.vertexLeft.numVertex + " <-> " + LineLogic.vertexRight.numVertex
            //                    }
            //                    onTriggered: LineLogic.swapVertex()
            //                }
            //            }
        }
    }
}

