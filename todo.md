# VCLib ToDo list

- Mesh:
  - [ ] generic updateReferences (avoid updateVertexReferences, updateFaceReferences, etc. on containers and components)
  - [ ] complete generic compact function when the updateReferences will be ready
  - [ ] remove duplicate code inside mesh class, use pack expansion
  - [ ] manage clean() for all components of mesh, not only element containers
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
  
