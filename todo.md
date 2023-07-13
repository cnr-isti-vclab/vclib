# VCLib ToDo list

- Mesh:
  - Concepts:
    - [ ] complete space data structures concepts
    - [ ] implement per edge concepts
  - Components:
    - [ ] Make the Component class use the CRTP and remove from all the components the isEnabled member function 
          (and, in general, all the members that use this)
    - [ ] the setVertices, setAdj*s that take as input a vector should take as input a range
    - [ ] write and comment all the empty constructors of the components
  - Elements:
    - [ ] set properly elements concepts
    - [ ] set constructors for Edge element
  - Containers:
    - [ ] set properly container concepts
  - Mesh:
    - [ ] manage clean() for all components of mesh, not only element containers
    - [ ] compactness should use uint and UINT_NULL instead of int and negative numbers
- Algorithms:
  - [ ] take views arguments instead of vectors (e.g. for selection) - allows to be more general on algorithms
  - Point Sampling:
    - [ ] uniform all the function, that should take an out argument of birthVertices/birthFaces
    - [ ] weights input argument of all the function should be a range
- Space:
  - [ ] implement ArrayConcept that works also with Eigen matrices in 2D (see export_to_matrix in algorithms)
- External:
  - [ ] make render functions using bgfx
- Documentation:
  - [ ] Explain appropriately the structure of the library
