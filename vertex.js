.import "main.js" as MainLogic

var edges = [];
var countEdges = 0;

var vertexRoot = null;

var xLast = 0, yLast = 0;

Array.prototype.remove = function(value) {
    var idx = this.indexOf(value);
    if (idx !== -1) {
        return this.splice(idx, 1);
    }
    return false;
}

function setVertexRoot(vertex)
{
    vertexRoot = vertex;
    xLast = vertexRoot.x;
    yLast = vertexRoot.y;
}

function addEdge(edge){
    edges.push(edge);
//    console.log("add edge:", edge);
//    console.log("edges:", edges);
}

function removeEdge(edge)
{
    edges.remove(edge)
//    console.log(edges)
}

function destroy()
{
//    console.log("edges.length:", edges.length)
//    console.log("edges:", edges)
    for(var i = 0; i < edges.length; )
        edges[i].destroySignal()

    MainLogic.deleteVertex(vertexRoot)
    vertexRoot.destroy()
}

function checkEdge(vertex)
{
//    console.log("edges(vertex):", edges)
//    console.log("edges(vertex).length:", edges.length)
    for(var i = 0; i < edges.length; i++)
        if(!edges[i].checkEdge(vertex))
            return false;
    return true;
}

function changedPosition()
{
    if(Math.abs(xLast - vertexRoot.x) < 5 && Math.abs(yLast - vertexRoot.y) < 5)
        return;

    for(var i = 0; i < edges.length; i++)
    {
        edges[i].vertexChangedPosition(vertexRoot);
//        console.log("send");
    }

    xLast = vertexRoot.x;
    yLast = vertexRoot.y;
}

function lightingEdges()
{
    MainLogic.clearLightingEdges();
    for(var i = 0; i < edges.length; i++)
    {
        edges[i].lightingEdge(true);
        MainLogic.addLightingEdge(edges[i]);
    }
}
