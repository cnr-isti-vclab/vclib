from dumper import *
from mesh_components import *


def comp_names():
    return [
        'ParentMeshPointer',
        'Position',
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


def dump_element(d, value, str_value):
    elemValue = value.members(True)[0]
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
    d.putValue(str_value)
    if d.isExpanded():
        with Children(d, len(compList), maxNumChild=10000):
            for i in d.childRange():
                d.putSubItem(compListNames[i], compList[i])


def qdump__vcl__Vertex(d, value):
    dump_element(d, value, 'vcl::Vertex<...>')


def qdump__vcl__Face(d, value):
    dump_element(d, value, 'vcl::Face<...>')


def qdump__vcl__Edge(d, value):
    dump_element(d, value, 'vcl::Edge<...>')
