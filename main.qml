import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0
import QtQuick.Window 2.0
import "qrc:/js/main.js" as MainLogic

//ApplicationWindow {
//    visible: true
//    width: 640
//    height: 480

    Rectangle {
        id: root
//        width: 640
//        height: 480
        anchors.fill: parent

        property int chooseItemState : 0

        Component.onCompleted: {
            MainLogic.setRoot(root)
        }

        Action {
            id: cancelAction

            shortcut: "Esc"
            onTriggered: {
                MainLogic.cancel()
            }
        }
        Action {
            id: cursorAction

            shortcut: "Ctrl+1"
            onTriggered: {
                MainLogic.cursorChoose()
            }
        }
        Action {
            id: vertexAction

            shortcut: "Ctrl+2"
            onTriggered: {
                MainLogic.vertexChoose()
            }
        }
        Action {
            id: edgeAction

            shortcut: "Ctrl+3"
            onTriggered: {
                MainLogic.edgeChoose()
            }
        }

        onChooseItemStateChanged: {
            if(chooseItemState !== 0) cursorPanel.border.width = 0;
            if(chooseItemState !== 1) vertexPanel.border.width = 0;
            if(chooseItemState !== 2) linePanel.border.width = 0;
            if(chooseItemState === 0) cursorPanel.border.width = 2;
            if(chooseItemState === 1) vertexPanel.border.width = 2;
            if(chooseItemState === 2) linePanel.border.width = 2;
        }

        Rectangle {
            id: choosePanel

            width: 29
            color: "#ffffff"

            anchors {
                top: root.top
                bottom: root.bottom
                left: root.left
            }

            Column {
                id: columnPanel
                anchors.fill: parent

                Rectangle {
                    id: cursorPanel

                    height: 29
                    color: "#00000000"

                    anchors {
                        left: columnPanel.left
                        right: columnPanel.right
                    }

                    radius: 2

                    border.color: "#50000000"
                    border.width: 0

                    Image {
                        id: cursorPanelImage
                        source: "qrc:/images/cursor.png"
                        sourceSize.width: 25
                        sourceSize.height: 25
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: cursorPanelMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            MainLogic.cursorChoose()
                        }
                        onEntered: {
                            if(root.chooseItemState !== 0)
                                cursorPanel.border.width = 1
                        }
                        onExited: {
                            if(root.chooseItemState !== 0)
                                cursorPanel.border.width = 0
                        }
                    }
                }

                Rectangle {
                    id: vertexPanel

                    height: 29
                    color: "#00000000"

                    anchors {
                        left: columnPanel.left
                        right: columnPanel.right
                    }

                    radius: 2

                    border.color: "#50000000"
                    border.width: 0

                    Image {
                        id: vertexPanelImage
                        source: "qrc:/images/circle.png"
                        sourceSize.width: 25
                        sourceSize.height: 25
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: vertexPanelMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            MainLogic.vertexChoose()
                        }
                        onEntered: {
                            if(root.chooseItemState !== 1)
                                vertexPanel.border.width = 1
                        }
                        onExited: {
                            if(root.chooseItemState !== 1)
                                vertexPanel.border.width = 0
                        }
                    }
                }

                Rectangle {
                    id: linePanel

                    height: 29
                    color: "#00000000"

                    anchors {
                        left: columnPanel.left
                        right: columnPanel.right
                    }

                    radius: 2

                    border.color: "#50000000"
                    border.width: 0

                    Image {
                        id: linePanelImage
                        source: "qrc:/images/line.png"
                        sourceSize.width: 25
                        sourceSize.height: 25
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        id: linePanelMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            MainLogic.edgeChoose()
                        }
                        onEntered: {
                            if(root.chooseItemState !== 2)
                                linePanel.border.width = 1
                        }
                        onExited: {
                            if(root.chooseItemState !== 2)
                                linePanel.border.width = 0
                        }
                    }
                }
            }
        }

        Rectangle {
            id: line
            x: 29

            width: 1
            color: "#868686"
            visible: true
            border.width: 0

            anchors {
                top: root.top
                bottom: root.bottom
                left: choosePanel.right
            }
        }

        Rectangle {
            id: scene

//            color: "gray"

            anchors {
                top: root.top
                bottom: root.bottom
                left: line.right
                right: root.right
            }

            Component.onCompleted: {
                MainLogic.setScene(scene)
                MainLogic.setSceneMouseArea(sceneMouseArea)
            }

            MouseArea {
                id: sceneMouseArea

                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton

                onClicked: {
                    console.log("6")
                    if(mouse.button & Qt.LeftButton)
                    {
                        if(chooseItemState === 0)
                        {
                            MainLogic.clearLightingEdges()
                        }
                        if(chooseItemState === 1)
                        {
                            MainLogic.addVertex();
                        }
                    }
                    if(mouse.button & Qt.RightButton)
                    {
                        if(chooseItemState === 2)
                        {
                            MainLogic.addAllEdge();
                        }
                    }
                }
            }
        }
    }
//}
