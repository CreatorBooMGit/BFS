.import "main.js" as MainLogic

var edges = [];
var countEdges = 0;

var vertexRoot = null;

var xLast = 0, yLast = 0;

function setVertexRoot(vertex)
{
    vertexRoot = vertex;
    xLast = vertexRoot.x;
    yLast = vertexRoot.y;
}

function addEdge(edge){
    edges.push(edge);
    console.log("add edge:", edge);
    console.log("edges:", edges);
}

function deleteEdge(edge)
{

}

function checkEdge(vertex)
{
    console.log("edges(vertex):", edges)
    console.log("edges(vertex).length:", edges.length)
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
