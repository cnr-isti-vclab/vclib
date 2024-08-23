from dumper import *
from mesh_components import *


def comp_names():
    return [
        'ParentMeshPointer',
        'Coordinate',
        'VertexReferences',
        'TriangleBitFlags',
        'PolygonBitFlags',
        'BitFlags',
        'Normal',
        'Color',
        'Quality',
        'AdjacentFaces',
        'AdjacentVertices',
        'WedgeTexCoords',
        'WedgeColors',
        'TexCoord',
        'PrincipalCurvature',
        'CustomComponents']


def qdump__vcl__Vertex(d, value):
    elemValue = value.members(True)[0]
    coord = None
    compList = []
    compListNames = []

    for name in comp_names():
        for base in elemValue.members(True):
            if name in base.name:
                compList.append(base)
                compListNames.append('[' + name + ']')
                break

    d.putExpandable()
    d.putNumChild(len(compList))
    d.putValue("vcl::Vertex<...>")
    if d.isExpanded():
        with Children(d, len(compList), maxNumChild=10000):
            for i in d.childRange():
                d.putSubItem(compListNames[i], compList[i])
