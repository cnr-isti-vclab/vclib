# VCLib ToDo list

- Concepts:
  - [ ] complete space data structures concepts
  - [ ] remove clang-format on and off when [this issue](https://github.com/llvm/llvm-project/issues/59412) will be fixed
- Mesh:
  - Elements:
    - [ ] set properly elements concepts
  - Containers:
    - Element container should provide fast compact function, that does not preserve elements order
  - Mesh:
    - [ ] manage clean() for all components of mesh, not only element containers 
  - Utils:
    - [ ] MeshInfo should not use its own enums for elements and components
- Exceptions:
  - [ ] MissingTriangularRequirementException and the other ones should be grouped in a single FaceSizeException class
- Iterators:
  - [ ] make just one StaticGridIterator class that can be both const and non-const (avoid code duplication)
- IO:
  - [ ] Manage read and write colors from integral to non integral types
- Algorithms:
  - [ ] take views arguments instead of vectors (e.g. for selection) - allows to be more general on algorithms
  - [ ] Implement exporter to VCG
  - [ ] Implement importer/exporter from buffer
  - [ ] Shuffle algorithms should be all made using views
  - [ ] Smooth algorithms should be made using loggers
  - Point Sampling:
    - [ ] uniform all the function, that should take an out argument of birthVertices/birthFaces
    - [ ] weights input argument of all the function should be a range
- Space:
  - [ ] improve documentation for Grid data structures
- Misc:
  - [ ] implement non-static member functions for FileInfo class
- External:
  - [ ] make render functions using bgfx
  - [ ] remove zip-iterator library when c++23 zip-view will be supported
- Documentation:
  - [ ] How to implement a user component
