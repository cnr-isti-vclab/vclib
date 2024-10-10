from dumper import *

def is_vertical_component(value):
    compValue = value.members(True)[0]
    return compValue.type.templateArgument(4)


def is_optional_component(value):
    compValue = value.members(True)[0]
    return compValue.type.templateArgument(4) and compValue.type.templateArgument(5)


def dump_simple_component(d, value):
    if is_optional_component(value):
        d.putValue('Optional Component')
    elif is_vertical_component(value):
        d.putValue('Vertical Component')
    else:
        d.putItem(value["mData"]["mData"])
        # d.putExpandable()
        # d.putNumChild(1)
        # if d.isExpanded():
        #     with Children(d):
        #         d.putSubItem("Coordinate", value["mData"]["mData"])


def qdump__vcl__comp__Coordinate(d, value):
    dump_simple_component(d, value)


def qdump__vcl__comp__Normal(d, value):
    dump_simple_component(d, value)


def qdump__vcl__comp__Color(d, value):
    dump_simple_component(d, value)
