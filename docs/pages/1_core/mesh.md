# VCLib Meshes     {#core_mesh}

The Mesh data structure of the VCLib core module is a generic data structure that can be customized to fit the user needs. The Mesh is composed of Elements, that are the basic building blocks of the Mesh, and Components, that are the data that the Elements store. The Mesh is organized in Containers, that store the Elements and the Components of the Mesh.

To explain how Mesh customization is possible, we first need to define the entities involved that compose a VCLib Mesh, and to do that we analyze the TriMesh example:

```mermaid
flowchart TB
    subgraph TM["<b>TriMesh</b>"]
        direction TB
        subgraph VC["<b>VertexContainer</b>"]
            direction LR
            V0["Vertex 0"] ~~~ V1["Vertex 1"] ~~~ Vdots["..."] ~~~ VN["Vertex n"]
        end
        subgraph FC["<b>FaceContainer</b>"]
            direction LR
            F0["Face 0"] ~~~ F1["Face 1"] ~~~ Fdots["..."] ~~~ FM["Face m"]
        end
        subgraph MC["<b>Mesh Components</b>"]
            direction LR
            subgraph BB["BoundingBox"]
                BBD["vcl::Box3"]
            end
            subgraph NM["Name"]
                NMD["std::string"]
            end
            subgraph TMX["TransformMatrix"]
                TMXD["vcl::Matrix44"]
            end
            MCdots["..."]
        end
    end

    classDef mesh fill:#dae8fc,stroke:#6c8ebf,stroke-width:2px,color:#000
    classDef container fill:#f8cecc,stroke:#b85450,stroke-width:2px,color:#000
    classDef element fill:#d5e8d4,stroke:#82b366,stroke-width:2px,color:#000
    classDef comp fill:#fff2cc,stroke:#d6b656,stroke-width:2px,color:#000
    classDef data fill:#ffe6cc,stroke:#d79b00,stroke-width:1.5px,color:#000

    class TM mesh
    class VC,FC container
    class V0,V1,Vdots,VN,F0,F1,Fdots,FM element
    class MC,BB,NM,TMX,MCdots comp
    class BBD,NMD,TMXD data
```

In this simplified scheme of the TriMesh, there are several entities involved:

  - The overall Mesh data structure (blue);
  - Elements (green);
  - Element Containers (red);
  - Components (yellow) and the data they store internally (orange);

The overall Mesh data structure is composed of two Containers of Vertex and Face Elements, and some other Components that store some data. 
Looking the Elements in detail, we see that each Element is composed of Components: for example, a Vertex could be organized as follows:

```mermaid
flowchart TB
    subgraph V["<b>Vertex</b>"]
        direction LR
        subgraph Pos["Position"]
            PosD["vcl::Point3"]
        end
        subgraph Flg["BitFlags"]
            FlgD["vcl::BitSet"]
        end
        subgraph Nrm["Normal"]
            NrmD["vcl::Point3"]
        end
        subgraph Clr["Color"]
            ClrD["vcl::Color"]
        end
        Vdots["..."]
    end

    classDef element fill:#d5e8d4,stroke:#82b366,stroke-width:2px,color:#000
    classDef comp fill:#fff2cc,stroke:#d6b656,stroke-width:2px,color:#000
    classDef data fill:#ffe6cc,stroke:#d79b00,stroke-width:1.5px,color:#000

    class V element
    class Pos,Flg,Nrm,Clr,Vdots comp
    class PosD,FlgD,NrmD,ClrD data
```

In this case, the Vertex Element has for example the Position, Flags, Normal and Color components, each one of them exposes its data trough appropriate member functions.

The data of a Mesh is stored in a *Structure of Arrays* (SoA): each Container keeps, for every Component of its Elements, a contiguous array that contains the values of that Component for all the Elements of the Container (e.g. all the positions of the vertices are contiguous in memory). An Element (e.g. `mesh.vertex(0)`) is just a lightweight proxy: it stores the index of the Element and a reference to its Container, and exposes the member functions of all the Components of the Element.

To understand how these entities are organized and implemented, we suggest to take a look at the @subpage mesh_components_page, @subpage mesh_elements_page and @subpage mesh_containers_page pages.

The next sections will introduce some concepts that you should know before using the VCLib Meshes.

### Components

As you may have already noticed, components can be part of both Elements and Meshes:

  - **Element Components** (defined in the `vcl::comp` namespace and aliased in `vcl::vert`, `vcl::face`, and `vcl::edge`) describe the per-element attributes whose data is stored in the SoA arrays of the Element's Container, and expose their accessors on the Element proxies;
  - **Mesh Components** (defined in the `vcl::mesh` namespace, e.g. vcl::mesh::BoundingBox3, vcl::mesh::Color, vcl::mesh::CustomComponents, vcl::mesh::Materials, vcl::mesh::Name, vcl::mesh::TransformMatrix) are inherited directly by the vcl::Mesh class and store mesh-wide data directly inside the Mesh instance.

When an attribute makes sense both at the Element level and at the Mesh level, the corresponding Element and Mesh components share the same `vcl::CompId` and expose the same member function interface. For example, vcl::comp::Color (used by Elements) and vcl::mesh::Color (used by the Mesh to assign a uniform color) both expose the `color()` member function:

```cpp
AMeshType m; // A Mesh type that has vcl::mesh::Color and vertices with vcl::vert::Color

m.color() = vcl::Color::Red;

auto v = m.vertex(0); // Get a vertex proxy from the mesh

v.color() = vcl::Color::Green; // The Vertex Element of AMeshType has the Color component
```

The Components of an Element can be of two categories:

  - Persistent: the data of the Component is always available;
  - Optional: the data of the Component can be enabled and disabled at runtime, allowing to avoid memory usage for components that are not needed in some contexts.

For example, the Vertex Element of the vcl::TriMesh class has some persistent components (Position, Flags, Normal, the components that are used more often), and some optional (Color, Adjacent Faces, Adjacent Vertices, Texture Coordinates, the ones that are used only on some contexts). The Container of the Elements stores the arrays of the persistent components, and the arrays of the optional components only when they are enabled. For example, assuming that we enable only the Adjacent Faces of the Vertex Element, the Vertex Container will store the arrays of Flags, Positions, Normals and Adjacent Faces, but not the ones of the other optional components:

```mermaid
flowchart LR
    VP["<b>Vertex(i) Proxy</b><br/>mParent = &VertexContainer<br/>mIndex = i"]

    subgraph VC["<b>VertexContainer (Structure of Arrays)</b>"]
        direction TB
        subgraph PC["<b>Persistent Components</b>"]
            direction TB
            BF["<b>BitFlags</b>: [ f₀ | ... | <b>fᵢ</b> | ... | fₙ ]"]
            POS["<b>Position</b>: [ p₀ | ... | <b>pᵢ</b> | ... | pₙ ]"]
            NRM["<b>Normal</b>: [ n₀ | ... | <b>nᵢ</b> | ... | nₙ ]"]
        end
        subgraph OC["<b>Optional Components</b>"]
            direction TB
            AF["<b>AdjacentFaces (enabled)</b>: [ af₀ | ... | <b>afᵢ</b> | ... | afₙ ]"]
            CLR["<b>Color (disabled)</b>: [ ]"]
            AV["<b>AdjacentVertices (disabled)</b>: [ ]"]
            TC["<b>TexCoord (disabled)</b>: [ ]"]
        end
    end

    VP -->|"v.flags()"| BF
    VP -->|"v.position()"| POS
    VP -->|"v.normal()"| NRM
    VP -->|"v.adjFaces()"| AF

    classDef container fill:#f8cecc,stroke:#b85450,stroke-width:2px,color:#000
    classDef element fill:#d5e8d4,stroke:#82b366,stroke-width:2px,color:#000
    classDef comp fill:#fff2cc,stroke:#d6b656,stroke-width:1.5px,color:#000
    classDef enabled fill:#ffe6cc,stroke:#d79b00,stroke-width:1.5px,color:#000
    classDef disabled fill:#f5f5f5,stroke:#999999,stroke-dasharray: 5 5,color:#666

    class VC container
    class VP element
    class PC,OC comp
    class BF,POS,NRM,AF enabled
    class CLR,AV,TC disabled
```

For the user, the usage of persistent or optional components is the same. The only thing that the user must check is wether a Component is Optional, and in that case, enable that component before using it. Every VCLib Container provides a list of member functions for all the Components that may be Optional for certain Elements (note: these member functions are available *only when the component is actually optional*). These member functions are:

  - `m.is<Element><Component>Available()`: returns a boolean indicating whether the `Component` of the `Element` is available (it is always `true` if the Component is persistent, and it is `true` for an optional Component only if it is enabled);
  - `m.enable<Element><Component>()`: enables the optional `Component` of the `Element`;
  - `m.disable<Element><Component>()`: disables and frees the memory of the optional `Component` of the `Element`.

In the example before, to use the `AdjacentFaces` component of the `Vertex` element of a mesh `m`, you should call first:

```cpp
m.enableVertexAdjacentFaces();
```

The same operations can be performed in a generic way, using the identifiers of the Element and of the Component:

```cpp
m.enableElementComponent<vcl::ElemId::VERTEX, vcl::CompId::ADJACENT_FACES>();

bool available =
    m.isElementComponentAvailable<vcl::ElemId::VERTEX, vcl::CompId::ADJACENT_FACES>();
```

### Indices

Elements of a Mesh that refer to other Elements (e.g. the vertices of a Face, or the adjacent faces of a Vertex) do not store pointers, but the *indices* of the referred Elements in their Containers. The type of these indices is a template argument of the Containers (`uint` for the built-in Meshes, `uint64_t` for the ones with the `64` suffix, e.g. vcl::TriMesh64), and it is available as `IndexType` in Meshes, Containers and Elements. The null (missing) reference is represented by `MeshType::NULL_INDEX`.

The position of a reference inside an Element (e.g. the `i`-th vertex of a Face) is instead a small number, that does not depend on the size of the Mesh, and it is always a `uint`:

```cpp
vcl::TriMesh m;
// ...
vcl::TriMesh::IndexType vi = m.face(0).vertexIndex(1); // index of the vertex in the vertex container
vcl::uint pos = m.face(0).indexOfVertex(vi);           // position of the vertex in the face (1);
                                                       // vcl::UINT_NULL if not found
```

Since Elements are proxies that store the Container they belong to, they can be freely copied and passed by value, and they remain valid if the Container is reallocated. They are not valid anymore if the Mesh that contains them is destroyed, moved or copied over. A default constructed Element, or the Element returned for a null reference, is not valid, and it converts to `false`.

### Deleting Elements

Elements are deleted by flagging them (`m.deleteVertex(i)`, `m.deleteFace(i)`, ...), and they remain in the Container until the Mesh (or a single Container) is compacted (`m.compact()`). The iteration over the Elements, and over the Component views, skips by default the deleted ones. Some functions require the Mesh to be compact: in that case, they check it and throw a vcl::MissingCompactnessException if it is not.
