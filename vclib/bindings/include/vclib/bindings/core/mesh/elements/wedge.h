// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_ELEMENTS_WEDGE_H
#define VCL_BINDINGS_CORE_MESH_ELEMENTS_WEDGE_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<FaceConcept FaceType, typename... Options>
void initWedge(pybind11::class_<FaceType, Options...>& c)
{
    using WedgeType          = vcl::WedgeType<FaceType>;
    using WedgeContainerType = comp::WedgeContainerProxy<FaceType>;

    namespace py = pybind11;

    // 1. Define Wedge as an inner class of Face (vclib.<Mesh>.Face.Wedge)
    pybind11::class_<WedgeType> w(c, "Wedge");

    w.def("index", [](const WedgeType& self) { return self.index(); });
    w.def(
        "parent_face",
        [](WedgeType& self) { return self.parentFace(); },
        py::keep_alive<0, 1>());
    w.def(
        "vertex",
        [](WedgeType& self) { return self.vertex(); },
        py::keep_alive<0, 1>());
    w.def("vertex_index", [](const WedgeType& self) {
        return self.vertexIndex();
    });

    w.def("__repr__", [](WedgeType& self) {
        return "<Wedge corner " + std::to_string(self.index()) + " of Face " +
               std::to_string(self.parentFace().index()) + ">";
    });

    if constexpr (comp::HasWedgeColors<FaceType>) {
        w.def(
            "color",
            [](WedgeType& self) -> auto& {
                if (!self.isColorAvailable()) {
                    throw MissingComponentException(
                        "Component 'WEDGE_COLORS' is not available on this "
                        "face.");
                }
                return self.color();
            },
            py::return_value_policy::reference_internal);
        w.def("set_color", [](WedgeType& self, const Color& col) {
            if (!self.isColorAvailable()) {
                throw MissingComponentException(
                    "Component 'WEDGE_COLORS' is not available on this face.");
            }
            self.color() = col;
        });
        w.def("is_color_available", [](const WedgeType& self) {
            return self.isColorAvailable();
        });
    }

    if constexpr (comp::HasWedgeTexCoords<FaceType>) {
        w.def(
            "tex_coord",
            [](WedgeType& self) -> auto& {
                if (!self.isTexCoordAvailable()) {
                    throw MissingComponentException(
                        "Component 'WEDGE_TEX_COORDS' is not available on this "
                        "face.");
                }
                return self.texCoord();
            },
            py::return_value_policy::reference_internal);
        w.def("set_tex_coord", [](WedgeType& self, const TexCoordd& tc) {
            if (!self.isTexCoordAvailable()) {
                throw MissingComponentException(
                    "Component 'WEDGE_TEX_COORDS' is not available on this "
                    "face.");
            }
            self.texCoord() = tc;
        });
        w.def("is_tex_coord_available", [](const WedgeType& self) {
            return self.isTexCoordAvailable();
        });
    }

    if constexpr (comp::HasWedgeNormals<FaceType>) {
        w.def(
            "normal",
            [](WedgeType& self) -> auto& {
                if (!self.isNormalAvailable()) {
                    throw MissingComponentException(
                        "Component 'WEDGE_NORMALS' is not available on this "
                        "face.");
                }
                return self.normal();
            },
            py::return_value_policy::reference_internal);
        w.def("set_normal", [](WedgeType& self, const Point3d& n) {
            if (!self.isNormalAvailable()) {
                throw MissingComponentException(
                    "Component 'WEDGE_NORMALS' is not available on this face.");
            }
            self.normal() = n;
        });
        w.def("is_normal_available", [](const WedgeType& self) {
            return self.isNormalAvailable();
        });
    }

    // 2. Define WedgeContainerProxy as an inner class (_WedgesRange)
    if (!registeredTypes.contains(typeid(WedgeContainerType))) {
        pybind11::class_<WedgeContainerType> wcp(c, "_WedgesRange");

        wcp.def("__len__", [](const WedgeContainerType& self) {
            return self.size();
        });

        wcp.def(
            "__getitem__",
            [](WedgeContainerType& self, int i) {
                int s = static_cast<int>(self.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                return self[static_cast<uint>(i)];
            },
            py::keep_alive<0, 1>());

        wcp.def("__bool__", [](const WedgeContainerType& self) {
            return !self.empty();
        });

        wcp.def(
            "__iter__",
            [](WedgeContainerType& self) {
                return py::make_iterator(self.begin(), self.end());
            },
            py::keep_alive<0, 1>());

        if constexpr (comp::HasWedgeColors<FaceType>) {
            using WedgeColorsView =
                decltype(std::declval<FaceType&>().wedges().colors());
            if (!registeredTypes.contains(typeid(WedgeColorsView))) {
                pybind11::class_<WedgeColorsView> cv(c, "_WedgeColorsRange");
                cv.def("__len__", [](const WedgeColorsView& v) {
                    return v.size();
                });
                cv.def(
                    "__getitem__",
                    [](WedgeColorsView& v, int i) -> auto& {
                        int s = static_cast<int>(v.size());
                        if (i < 0) {
                            i += s;
                        }
                        if (i < 0 || i >= s) {
                            throw py::index_error();
                        }
                        return v[static_cast<uint>(i)];
                    },
                    py::return_value_policy::reference_internal);
                cv.def("__bool__", [](const WedgeColorsView& v) {
                    return !v.empty();
                });
                cv.def(
                    "__iter__",
                    [](WedgeColorsView& v) {
                        return py::make_iterator(v.begin(), v.end());
                    },
                    py::keep_alive<0, 1>());
                registeredTypes.insert(typeid(WedgeColorsView));
            }
            wcp.def(
                "colors",
                [](WedgeContainerType& self) {
                    if (!self.isColorsAvailable()) {
                        throw MissingComponentException(
                            "Component 'WEDGE_COLORS' is not available on this "
                            "face.");
                    }
                    return self.colors();
                },
                py::keep_alive<0, 1>());
            wcp.def("is_colors_available", [](const WedgeContainerType& self) {
                return self.isColorsAvailable();
            });
        }

        if constexpr (comp::HasWedgeTexCoords<FaceType>) {
            using WedgeTexCoordsView =
                decltype(std::declval<FaceType&>().wedges().texCoords());
            if (!registeredTypes.contains(typeid(WedgeTexCoordsView))) {
                pybind11::class_<WedgeTexCoordsView> tv(
                    c, "_WedgeTexCoordsRange");
                tv.def("__len__", [](const WedgeTexCoordsView& v) {
                    return v.size();
                });
                tv.def(
                    "__getitem__",
                    [](WedgeTexCoordsView& v, int i) -> auto& {
                        int s = static_cast<int>(v.size());
                        if (i < 0) {
                            i += s;
                        }
                        if (i < 0 || i >= s) {
                            throw py::index_error();
                        }
                        return v[static_cast<uint>(i)];
                    },
                    py::return_value_policy::reference_internal);
                tv.def("__bool__", [](const WedgeTexCoordsView& v) {
                    return !v.empty();
                });
                tv.def(
                    "__iter__",
                    [](WedgeTexCoordsView& v) {
                        return py::make_iterator(v.begin(), v.end());
                    },
                    py::keep_alive<0, 1>());
                registeredTypes.insert(typeid(WedgeTexCoordsView));
            }
            wcp.def(
                "tex_coords",
                [](WedgeContainerType& self) {
                    if (!self.isTexCoordsAvailable()) {
                        throw MissingComponentException(
                            "Component 'WEDGE_TEX_COORDS' is not available on "
                            "this face.");
                    }
                    return self.texCoords();
                },
                py::keep_alive<0, 1>());
            wcp.def(
                "is_tex_coords_available", [](const WedgeContainerType& self) {
                    return self.isTexCoordsAvailable();
                });
        }

        if constexpr (comp::HasWedgeNormals<FaceType>) {
            using WedgeNormalsView =
                decltype(std::declval<FaceType&>().wedges().normals());
            if (!registeredTypes.contains(typeid(WedgeNormalsView))) {
                pybind11::class_<WedgeNormalsView> nv(c, "_WedgeNormalsRange");
                nv.def("__len__", [](const WedgeNormalsView& v) {
                    return v.size();
                });
                nv.def(
                    "__getitem__",
                    [](WedgeNormalsView& v, int i) -> auto& {
                        int s = static_cast<int>(v.size());
                        if (i < 0) {
                            i += s;
                        }
                        if (i < 0 || i >= s) {
                            throw py::index_error();
                        }
                        return v[static_cast<uint>(i)];
                    },
                    py::return_value_policy::reference_internal);
                nv.def("__bool__", [](const WedgeNormalsView& v) {
                    return !v.empty();
                });
                nv.def(
                    "__iter__",
                    [](WedgeNormalsView& v) {
                        return py::make_iterator(v.begin(), v.end());
                    },
                    py::keep_alive<0, 1>());
                registeredTypes.insert(typeid(WedgeNormalsView));
            }
            wcp.def(
                "normals",
                [](WedgeContainerType& self) {
                    if (!self.isNormalsAvailable()) {
                        throw MissingComponentException(
                            "Component 'WEDGE_NORMALS' is not available on "
                            "this face.");
                    }
                    return self.normals();
                },
                py::keep_alive<0, 1>());
            wcp.def("is_normals_available", [](const WedgeContainerType& self) {
                return self.isNormalsAvailable();
            });
        }

        registeredTypes.insert(typeid(WedgeContainerType));
    }

    // 3. Define methods on FaceType
    c.def(
        "wedge",
        [](FaceType& self, uint i) {
            if (i >= self.vertexCount()) {
                throw py::index_error();
            }
            return self.wedge(i);
        },
        py::keep_alive<0, 1>());

    c.def(
        "wedge_mod",
        [](FaceType& self, int i) {
            if (self.vertexCount() == 0) {
                throw py::index_error("Face has no vertices");
            }
            return self.wedgeMod(i);
        },
        py::keep_alive<0, 1>());

    c.def(
        "wedges",
        [](FaceType& self) { return self.wedges(); },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_ELEMENTS_WEDGE_H
