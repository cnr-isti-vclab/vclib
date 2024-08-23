from dumper import *


def qdump__vcl__trimesh__Vertex(d, value):
    vertex = value.members(True)[0]
    d.putItem(vertex)
