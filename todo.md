# VCLib ToDo list

- Mesh:
  - Components:

  - Elements:
    - [ ] set properly elements concepts
    - [ ] set constructors for Edge element
    - [ ] set constructors for HalfEdge element
  - Containers:
    - [ ] set properly container concepts
  - Mesh:
    - [ ] customComponents and verticalComponents members should be dynamic, and do not static check on the type of element
    - [ ] generic importReferences (avoid importVertexReferences, importFaceReferences, etc. on containers and components)
  
    - [ ] manage clean() for all components of mesh, not only element containers
    - [ ] import dcel from mesh: update adjacent faces e wedges 
- Samplers:
  - [ ] add resize member function to allow parallel algorithms
- Algorithms:
  - [ ] implement housdorff
  - [ ] update adjacent faces optimized for dcel
- I/O:
  - [ ] move appropriately load/save functions outside "vcl::io" namespace 
  - [ ] implement STL importer/exporter  
- External:
  - [ ] make render functions using bgfx
- Documentation:
  - [ ] Make documentation using only doxygen
  - [ ] Explain appropriately the structure of the library
  
