.pragma library
.import QtQuick 2.7 as QML

var root = null
var graph = null
var scene = null
var sceneMouseArea = null

var vertices = []
var choosedVertex = null

var edges = []
var edgesLighting = []
var edgesChoosed = []

var tmpLeftVertexToNewEdge = null
var tmpRightVertexToNewEdge = null

var lineComponent = null

var newItemState = 0

Array.prototype.remove = function(value) {
    var idx = this.indexOf(value);
    if (idx !== -1) {
        return this.splice(idx, 1);
    }
    return false;
}

function setRoot(_root) {
    root = _root
}

function setGraph(_graph) {
    graph = _graph
}

function setScene(_scene) {
    scene = _scene
}

function setSceneMouseArea(_sceneMouseArea) {
    sceneMouseArea = _sceneMouseArea
}

function setNewItemState(state) {
    newItemState = state
    root.chooseItemState = state
//    console.log(root.chooseItemState)
}

function getNewItemState() {
    return newItemState
}

function addVertex() {
    var component = Qt.createComponent("qrc:/Vertex.qml")
    if (component.status === QML.Component.Ready) {
        var childRec = component.createObject(scene)
        childRec.x = sceneMouseArea.mouseX - childRec.width / 2
        childRec.y = sceneMouseArea.mouseY - childRec.height / 2
        childRec.numVertex = vertices.length + 1
        vertices.push(childRec)
        graph.slotAddVertex(childRec)
    }
}

function deleteVertex(vertex) {
    clearLightingEdges()
    vertices.remove(vertex)
    reloadNumberVertex(vertex.numVertex)
    graph.slotRemoveVertex(vertex.numVertex)
}

function setChoosedVertex(vertex) {
    choosedVertex = vertex
}

function reloadNumberVertex(numVertex)
{
    for(var i = 0; i < vertices.length; i++)
        if(vertices[i].numVertex > numVertex)
            vertices[i].numVertex--;
}

function addEdge(vertex) {
    if (tmpLeftVertexToNewEdge === null)
        tmpLeftVertexToNewEdge = vertex
    else if (tmpRightVertexToNewEdge === null)
        //            if(tmpLeftVertexToNewEdge.checkEdge(tmpRightVertexToNewEdge))
        if(tmpLeftVertexToNewEdge !== vertex)
        {
            tmpRightVertexToNewEdge = vertex
            var childRec = createEdge()
            tmpLeftVertexToNewEdge.addEdge(childRec)
            tmpRightVertexToNewEdge.addEdge(childRec)
            edges.push(childRec)
            graph.slotAddEdge(childRec, tmpLeftVertexToNewEdge.numVertex,
                              tmpRightVertexToNewEdge.numVertex, false)
            clearAddEdge()
        }
}

function deleteEdge(edge)
{
    edges.remove(edge)
    edgesChoosed.remove(edge)
    edge.lightingEdge(false)
    edgesLighting.remove(edge)
    reloadNumberEdge(edge.numEdge)
}

function reloadNumberEdge(numEdge)
{
    for(var i = 0; i < edges.length; i++)
        if(edges[i].numEdge > numEdge)
            edges[i].numEdge--;
}

function clearAddEdge() {
    tmpLeftVertexToNewEdge = null
    tmpRightVertexToNewEdge = null
}

function createEdge() {
    if (tmpLeftVertexToNewEdge !== null && tmpRightVertexToNewEdge !== null) {
        lineComponent = Qt.createComponent("qrc:/Line.qml")
        if (lineComponent.status === QML.Component.Ready) {
            var childRec = lineComponent.createObject(scene)
            childRec.x1 = tmpLeftVertexToNewEdge.x + tmpLeftVertexToNewEdge.width / 2
            childRec.y1 = tmpLeftVertexToNewEdge.y + tmpLeftVertexToNewEdge.height / 2
            childRec.x2 = tmpRightVertexToNewEdge.x + tmpRightVertexToNewEdge.width / 2
            childRec.y2 = tmpRightVertexToNewEdge.y + tmpRightVertexToNewEdge.height / 2
            childRec.numEdge = edges.length + 1
            childRec.setVertices(tmpLeftVertexToNewEdge,
                                 tmpRightVertexToNewEdge)
            return childRec
        }
    }
}

function addLightingEdge(edge) {
    edgesLighting.push(edge)
}

function clearLightingEdges() {
    console.log(edges.length)
    for (var i = 0; i < edgesLighting.length; i++) {
        edgesLighting[i].lightingEdge(false)
    }

    edgesLighting = []
}

function setEdgeChecked(edge) {
    edgeChecked = edge
}

function getEdgeChecked() {
    return edge
}

function addAllEdge() {
//    console.log("vertices.length:", vertices.length)
    for (var i = 0; i < vertices.length; i++) {
        for (var j = i; j < vertices.length; j++) {
            if (vertices[i] !== vertices[j]) {
                //                console.log("pc");
                addEdge(vertices[i])
                addEdge(vertices[j])
            } else
                continue
        }
    }
}

function cancel() {
    if (newItemState === 2) {
        if (tmpLeftVertexToNewEdge !== null) {
            clearAddEdge()
            return
        }
    }
    cursorChoose()
}

function cursorChoose() {
    clearAddEdge()
    setNewItemState(0)
    sceneMouseArea.cursorShape = Qt.ArrowCursor
}

function vertexChoose() {
    clearAddEdge()
    setNewItemState(1)
    sceneMouseArea.cursorShape = Qt.CrossCursor
}

function edgeChoose() {
    setNewItemState(2)
    sceneMouseArea.cursorShape = Qt.ArrowCursor
}

function deleteAllVertices()
{
    for(var i = 0; i < vertices.length; i++)
    {
        vertices[i].destroySignal();
    }
}
