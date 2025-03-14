# Spatial and other Data Structures     {#core_space}

VCLib provides several Data Structures, mostly spatial, that can be include trough `#include<vclib/space.h>`.

## Spatial Data Structures

Most of the VCLib spatial data structures can be used in a N-Dimensional space, with the exception of some specific classes.
Generally, they are templated over a a type `Scalar`, which is the Scalar that the class is going to store the data, and an unsigned integer number `N` that represent the dimension.

For example, the `Point` class is templated over `Scalar` and `N`. Specializations are then provided, like `Point3d`, which is a `Point<double, 3>`.

A list of all the spatial data structures provided by this library can be found in the @ref space_core and @ref space_complex groups.

## Utility and Miscellaneous Data Structures

Several other utility data structures are provided by VCLib. They can be used by including `#include<vclib/miscellaneous.h>`, and they are listed in the @ref miscellaneous group.
