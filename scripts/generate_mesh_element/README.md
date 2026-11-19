# Mesh element generator

`generate_mesh_element.py` creates the boilerplate required to add a new element
(e.g. a tetrahedron) to the VCLib mesh, and registers it in the files that list
all the elements. It works on the headers of `vclib/core/include/vclib/mesh`.

> The `MeshInfo` class (`vclib/space/complex/mesh_info.h`), the mesh presets
> (`vclib/meshes`) and the algorithms must be updated by hand.

## Usage

Run it from any directory:

```shell
python3 scripts/generate_mesh_element/generate_mesh_element.py <element_name> \
    [-n NAMESPACE] [-p PLURAL] [-r REQUIRED...] [-c COMPONENTS...] [-vn N]
```

- `element_name`: name of the element, in snake case (e.g. `tetrahedron`,
  `poly_edge`);
- `-n/--namespace`: namespace of the element components (`vcl::<namespace>`).
  Default: first four characters of the name (`tetr`);
- `-p/--plural`: plural of the name, used for the names of the container
  member functions (`tetrahedra()`, `addTetrahedra()`, ...) and of the concepts
  (`HasTetrahedra`). Default: the name followed by an `s`;
- `-r/--required`: components that every element must have. `bit_flags` is
  always required;
- `-c/--components`: other components that the element can use. The ones that
  can be optional get both the plain and the `Optional` aliases, and the
  container gets the `enable*`, `disable*` and `is*Available` member functions;
- `-vn/--vertex_count`: adds the (required) `vertex_references` component, with
  the given number of vertices (`-1` for a dynamic number). It enables the
  `add<Element>(vertices...)` member functions of the container;
- `--mesh_dir`: use another `vclib/mesh` directory (e.g. a copy of it).

Supported components: `adjacent_edges`, `adjacent_faces`, `adjacent_vertices`,
`bit_flags`, `color`, `custom_components`, `material_index`, `normal`,
`position`, `principal_curvature`, `quality`, `tangent`, `tex_coord`,
`vertex_references`. Components with a fixed size other than the vertex
references (e.g. adjacencies with a fixed number of elements) and the
face-specific ones (wedges, polygon and triangle bit flags) must be added by
hand.

Example:

```shell
python3 scripts/generate_mesh_element/generate_mesh_element.py tetrahedron \
    -p tetrahedra -vn 4 \
    -c adjacent_faces adjacent_vertices color custom_components normal quality
```

## What is generated

New files (in `vclib/mesh`):

- `elements/<element>.h`: the element proxy class, the `<Element>Components`
  type wrapper and the `<Element>Concept` concept;
- `elements/<element>_components.h`: the aliases of the components in the
  `vcl::<namespace>` namespace;
- `containers/<element>_container.h`: the element container;
- `requirements/mesh_concepts/<element>.h`: the `Has<Element><Components>`
  mesh concepts;
- `views/elements/<element>.h`: the `views::<elements>` view.

Updated files:

- `elements/base/base.h`: `ElemId` and `ELEMENT_ENUM_STRINGS`;
- `containers/base/element_container.h`: `<Element>ContainerConcept`;
- `traits.h`: `<Element>Type` and `Const<Element>Type`;
- `requirements/mesh_concepts/base.h`: `Has<Elements>` and
  `<Element>MeshConcept`;
- `requirements/contracts.h`: `require<Element>Compactness`, and the
  `enableIf*Optional` and `require*` functions of the components;
- the aggregator headers `containers.h`, `elements.h`, `views/elements.h` and
  `requirements/mesh_concepts.h`.

Running the script twice for the same element does not duplicate the
registrations, but it overwrites the generated files.

## Templates

The files in `templates` are the sources of the generated code. The tokens
`%EL%`, `%EL_UC%`, `%EL_C%`, ... (see `Element.tokens()` in
`generators/common.py`) are replaced with the name of the element in the
various cases, and the `//@if <flag>`, `//@else`, `//@endif` lines enable or
disable parts of a template. The snippets of the components used by
`elements/<element>_components.h` are in `templates/components`.

When the code of the mesh elements changes, the templates must be kept in sync.
A simple check is to regenerate an existing element in a copy of the `mesh`
directory and to compare the result with the original:

```shell
cp -r vclib/core/include/vclib/mesh /tmp/mesh
python3 scripts/generate_mesh_element/generate_mesh_element.py edge -vn 2 \
    -c adjacent_edges adjacent_faces color custom_components normal quality \
    --mesh_dir /tmp/mesh
diff -r vclib/core/include/vclib/mesh /tmp/mesh
```

The only differences expected are in the formatting and in the messages of some
`static_assert`.
