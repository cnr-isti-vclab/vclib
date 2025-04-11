# Components     {#mesh_components_page}

A Component is a C++ class that stores some data, and exposes a list of member functions that allow to manipulate it. An example of Component is the  vcl::comp::Color class: it stores a vcl::Color, and exposes some member functions that allows to access that color: vcl::comp::Color::color().

Components are combined with each other to form more complex structures, which provide all the member functions of the components they are composed of. These structures can be Elements or Meshes: VCLib Components are written in order to be as generic as possible, and in order to be used properly in all the possible contexts. 

Components are listed in the @ref components group, and they are defined under the vcl::comp namespace.