# VCLib ToDo list

- General:
  - [ ] Enumerations should be declared in a struct to avoid name collisions
- Concepts:
  - [ ] complete space data structures concepts
  - [ ] remove clang-format on and off when [this issue](https://github.com/llvm/llvm-project/issues/59412) will be fixed
- Mesh:
  - Elements:
    - [ ] set properly elements concepts
  - Containers:
    - Element container should provide fast compact function, that does not preserve elements order
  - Components:
    - references to elements should be available using ELEMENT_ID
  - Mesh:
    - [ ] manage clean() for all components of mesh, not only element containers
  - Utils:
    - [ ] MeshInfo should not use its own enums for elements and components
- Exceptions:
  - [ ] MissingTriangularRequirementException and the other ones should be grouped in a single FaceSizeException class
- Iterators:
  - [ ] make just one StaticGridIterator class that can be both const and non-const (avoid code duplication)
  - [ ] move bipartite graph iterators from space to iterators (see space todo list)
- IO:
  - [ ] Manage read and write colors from integral to non integral types
- Algorithms:
  - [ ] Separate Mesh algorithms from core(?) algorithms
  - [ ] Take views arguments instead of vectors- allows to be more general on algorithms
    - [ ] core/fitting
    - [ ] core/stat
  - [ ] Implement exporter to VCG
  - [ ] Implement importer/exporter from buffer
  - [ ] Shuffle algorithms should be all made using views
  - [ ] Shuffle algorithms should take as input a random_device instead of a boolean 'deterministic', and they should log the seed at debug level
  - [ ] Smooth algorithms should be made using loggers
  - Point Sampling:
    - [ ] uniform all the function, that should take an out argument of birthVertices/birthFaces
    - [ ] weights input argument of all the function should be a range
- Space:
  - [ ] improve documentation for Grid data structures
  - [ ] move bipartite graph iterators from space to iterators (see iterators todo list)
- Misc:
  - [ ] implement non-static member functions for FileInfo class
- External:
  - [ ] remove zip-iterator library when c++23 zip-view will be supported
  - [ ] look for a json header only library that is compatible with c++20 modules
- Documentation:
  - [ ] How to implement a user component