.import "main.js" as MainLogic
//.import "vertex.js" as VertexLogic

var edgeRoot = null;

var vertexLeft = null;
var vertexRight = null;
var directed = false;

function setRootEdge(edge)
{
    edgeRoot = edge;
}

function destroy()
{
    MainLogic.deleteEdge(edgeRoot)
    vertexLeft.removeEdge(edgeRoot);
    vertexRight.removeEdge(edgeRoot);
    MainLogic.graph.slotRemoveEdge(edgeRoot.numEdge, vertexLeft.numVertex, vertexRight.numVertex, false)
    edgeRoot.destroy();
}

function getWidth(x1, y1, x2, y2)
{
    var w = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
    return w;
}

function getAngle(x1, y1, x2, y2)
{
    var angle = (y2-y1) / (x2-x1);

    angle = Math.atan(angle) * 180 / Math.PI;

    if(x1 > x2 && y1 < y2) angle = 180 + angle;
    if(x1 > x2 && y1 > y2) angle = 180 + angle;
    if(x1 > x2 && y1 === y2) angle = 180;

    return angle;
}

function setVertices(left, right)
{
    vertexLeft = left;
    vertexRight = right;
}

function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++ ) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

function lightingEdge()
{
    MainLogic.clearLightingEdges();

    edgeRoot.lightingEdge(true);
    MainLogic.addLightingEdge(edgeRoot);
}

function swapVertex()
{
    var swap = vertexLeft;
    vertexLeft = vertexRight;
    vertexRight = swap;
}

function checkEdge(vertex)
{

}
