"""Description of the components that can be used by a mesh element."""

from dataclasses import dataclass
from typing import Optional


@dataclass(frozen=True)
class ComponentInfo:
    name: str  # snake case, it is also the name of the header file
    upper_camel: str  # e.g. AdjacentFaces
    comp_id: str  # CompId value, e.g. ADJACENT_FACES
    optional: bool = False  # it can be an optional component (disabled at runtime)
    # 'generic' -> HasElementComponent, 'adjacency' -> HasElementAdjacentX,
    # 'custom' -> HasElementCustomComponents
    kind: Optional[str] = None
    # Title of the section of contracts.h that contains the functions of this
    # component
    contracts_section: Optional[str] = None


_LIST = [
    ComponentInfo('adjacent_edges', 'AdjacentEdges', 'ADJACENT_EDGES', True,
                  'adjacency', 'Adjacencies: AdjacentEdges'),
    ComponentInfo('adjacent_faces', 'AdjacentFaces', 'ADJACENT_FACES', True,
                  'adjacency', 'Adjacencies: AdjacentFaces'),
    ComponentInfo('adjacent_vertices', 'AdjacentVertices', 'ADJACENT_VERTICES',
                  True, 'adjacency', 'Adjacencies: AdjacentVertices'),
    ComponentInfo('bit_flags', 'BitFlags', 'BIT_FLAGS'),
    ComponentInfo('color', 'Color', 'COLOR', True, 'generic', 'Colors'),
    ComponentInfo('custom_components', 'CustomComponents', 'CUSTOM_COMPONENTS',
                  False, 'custom', 'Custom Components'),
    ComponentInfo('material_index', 'MaterialIndex', 'MATERIAL_INDEX', True,
                  'generic', 'Material Index'),
    ComponentInfo('normal', 'Normal', 'NORMAL', True, 'generic', 'Normals'),
    ComponentInfo('position', 'Position', 'POSITION'),
    ComponentInfo('principal_curvature', 'PrincipalCurvature',
                  'PRINCIPAL_CURVATURE', True, 'generic',
                  'Principal Curvatures'),
    ComponentInfo('quality', 'Quality', 'QUALITY', True, 'generic',
                  'Qualities'),
    ComponentInfo('tangent', 'Tangent', 'TANGENT', True, 'generic', 'Tangents'),
    ComponentInfo('tex_coord', 'TexCoord', 'TEX_COORD', True, 'generic',
                  'TexCoords'),
    ComponentInfo('vertex_references', 'VertexReferences', 'VERTEX_REFERENCES'),
]

COMPONENTS = {c.name: c for c in _LIST}

# Components for which the VCL_COMMON_CONTAINER_COMPONENT_METHODS macro already
# generates the enable/disable/isAvailable member functions in the containers.
COMMON_CONTAINER_COMPONENTS = ('color', 'normal', 'quality')

# Order of the sections of mesh_concepts/<element>.h
MESH_CONCEPTS_ORDER = (
    'normal', 'color', 'tex_coord', 'adjacent_faces', 'adjacent_edges',
    'adjacent_vertices', 'quality', 'principal_curvature', 'tangent',
    'material_index', 'custom_components')

MESH_CONCEPTS_TITLES = {
    'normal': 'Normals', 'color': 'Colors', 'tex_coord': 'TexCoords',
    'adjacent_faces': 'Adjacencies', 'quality': 'Quality',
    'principal_curvature': 'PrincipalCurvature', 'tangent': 'Tangent',
    'material_index': 'MaterialIndex', 'custom_components': 'Custom Components'
}


def get_component(name):
    if name not in COMPONENTS:
        raise SystemExit(
            f"Unknown component '{name}'. Supported components: "
            + ", ".join(sorted(COMPONENTS)))
    return COMPONENTS[name]
