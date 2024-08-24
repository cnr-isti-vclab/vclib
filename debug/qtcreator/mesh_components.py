from dumper import *

def is_vertical_component(value):
    compValue = value.members(True)[0]
    return compValue.type.templateArgument(4)

def qdump__vcl__comp__Coordinate(d, value):
    if is_vertical_component(value):
        d.putValue('Vertical Component')
    else:
        d.putItem(value["mData"]["mData"])
        # d.putExpandable()
        # d.putNumChild(1)
        # if d.isExpanded():
        #     with Children(d):
        #         d.putSubItem("Coordinate", value["mData"]["mData"])
