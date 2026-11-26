// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/complex.h>
#include <vclib/space/core.h>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <concepts>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

namespace vcl::bind {

/**
 * @brief GIL-safe wrapper around pybind11::object for storing arbitrary Python
 * objects in VCLib's std::any-based CustomComponents.
 */
struct PyCustomObject
{
    pybind11::object obj;

    PyCustomObject() : obj(pybind11::none()) {}

    explicit PyCustomObject(pybind11::object o) : obj(std::move(o)) {}

    PyCustomObject(const PyCustomObject& other)
    {
        if (Py_IsInitialized()) {
            pybind11::gil_scoped_acquire gil;
            obj = other.obj;
        }
    }

    PyCustomObject(PyCustomObject&& other) noexcept
    {
        if (Py_IsInitialized()) {
            pybind11::gil_scoped_acquire gil;
            obj = std::move(other.obj);
        }
    }

    PyCustomObject& operator=(const PyCustomObject& other)
    {
        if (this != &other && Py_IsInitialized()) {
            pybind11::gil_scoped_acquire gil;
            obj = other.obj;
        }
        return *this;
    }

    PyCustomObject& operator=(PyCustomObject&& other) noexcept
    {
        if (this != &other && Py_IsInitialized()) {
            pybind11::gil_scoped_acquire gil;
            obj = std::move(other.obj);
        }
        return *this;
    }

    ~PyCustomObject()
    {
        if (Py_IsInitialized()) {
            pybind11::gil_scoped_acquire gil;
            obj = pybind11::object();
        }
        else {
            // Avoid Py_DECREF after interpreter finalization (e.g. late static teardown).
            (void) obj.release();
        }
    }
};

namespace detail {

// Maps C++ integral scalars to fixed-width NumPy-friendly types so char/uint
// map cleanly to int8/uint32 rather than platform-dependent byte/char dtypes.
template<typename T>
struct NumpyScalarType
{
    using type = T;
};

template<>
struct NumpyScalarType<char>
{
    using type = std::int8_t;
};

template<>
struct NumpyScalarType<unsigned char>
{
    using type = std::uint8_t;
};

template<>
struct NumpyScalarType<short>
{
    using type = std::int16_t;
};

template<>
struct NumpyScalarType<unsigned short>
{
    using type = std::uint16_t;
};

template<>
struct NumpyScalarType<int>
{
    using type = std::int32_t;
};

template<>
struct NumpyScalarType<uint>
{
    using type = std::uint32_t;
};

template<typename T>
using NumpyScalarTypeT = typename NumpyScalarType<T>::type;

template<typename T>
inline constexpr bool isCustomComponentScalarV =
    std::is_same_v<T, char> || std::is_same_v<T, unsigned char> ||
    std::is_same_v<T, short> || std::is_same_v<T, unsigned short> ||
    std::is_same_v<T, int> || std::is_same_v<T, uint> ||
    std::is_same_v<T, float> || std::is_same_v<T, double>;

template<typename T>
inline constexpr bool isCustomComponentPointV =
    std::is_same_v<T, Point2d> || std::is_same_v<T, Point3d> ||
    std::is_same_v<T, Point4d>;

/**
 * @brief Central runtime dispatcher mapping a stored std::type_index to a
 * templated callable across all supported C++ pool types and PyCustomObject.
 *
 * Includes all 8 MeshInfo::DataType scalars so columns created by C++
 * algorithms (e.g. uint 'birthVertex') or PLY I/O work seamlessly from Python.
 */
template<typename Fn>
decltype(auto) dispatchCustomComponentType(std::type_index ti, Fn&& fn)
{
    if (ti == typeid(char))
        return fn.template operator()<char>();
    else if (ti == typeid(unsigned char))
        return fn.template operator()<unsigned char>();
    else if (ti == typeid(short))
        return fn.template operator()<short>();
    else if (ti == typeid(unsigned short))
        return fn.template operator()<unsigned short>();
    else if (ti == typeid(int))
        return fn.template operator()<int>();
    else if (ti == typeid(uint))
        return fn.template operator()<uint>();
    else if (ti == typeid(float))
        return fn.template operator()<float>();
    else if (ti == typeid(double))
        return fn.template operator()<double>();
    else if (ti == typeid(std::string))
        return fn.template operator()<std::string>();
    else if (ti == typeid(Point2d))
        return fn.template operator()<Point2d>();
    else if (ti == typeid(Point3d))
        return fn.template operator()<Point3d>();
    else if (ti == typeid(Point4d))
        return fn.template operator()<Point4d>();
    else if (ti == typeid(Color))
        return fn.template operator()<Color>();
    else if (ti == typeid(PyCustomObject))
        return fn.template operator()<PyCustomObject>();

    throw BadCustomComponentTypeException(
        "Unsupported custom component C++ type '" + std::string(ti.name()) +
        "' in Python bindings.");
}

/**
 * @brief Resolves a Python type (int, float, str, Point2/3/4, Color, object) or
 * MeshInfo.DataType into the corresponding C++ std::type_index for creation or
 * type checking.
 */
inline std::type_index resolveCustomComponentTypeIndex(
    pybind11::handle compType)
{
    namespace py = pybind11;

    if (py::isinstance<MeshInfo::DataType>(compType)) {
        switch (compType.cast<MeshInfo::DataType>()) {
        case MeshInfo::DataType::CHAR: return typeid(char);
        case MeshInfo::DataType::UCHAR: return typeid(unsigned char);
        case MeshInfo::DataType::SHORT: return typeid(short);
        case MeshInfo::DataType::USHORT: return typeid(unsigned short);
        case MeshInfo::DataType::INT: return typeid(int);
        case MeshInfo::DataType::UINT: return typeid(uint);
        case MeshInfo::DataType::FLOAT: return typeid(float);
        case MeshInfo::DataType::DOUBLE: return typeid(double);
        case MeshInfo::DataType::NONE:
            throw std::invalid_argument(
                "MeshInfo.DataType.NONE cannot be used as a custom component "
                "type.");
        }
    }

    PyObject* ptr = compType.ptr();
    if (ptr == reinterpret_cast<PyObject*>(&PyLong_Type))
        return typeid(int);
    if (ptr == reinterpret_cast<PyObject*>(&PyFloat_Type))
        return typeid(double);
    if (ptr == reinterpret_cast<PyObject*>(&PyUnicode_Type))
        return typeid(std::string);
    if (ptr == reinterpret_cast<PyObject*>(&PyBaseObject_Type))
        return typeid(PyCustomObject);
    if (ptr == reinterpret_cast<PyObject*>(&PyBool_Type)) {
        // bool is excluded as a dedicated C++ primitive; Python bools still
        // work as mesh-level values via the 'object' (PyCustomObject) fallback.
        throw std::invalid_argument(
            "Unsupported custom component type 'bool'; use 'int' or 'object' "
            "instead.");
    }

    if (compType.is(py::type::of<Point2d>()))
        return typeid(Point2d);
    if (compType.is(py::type::of<Point3d>()))
        return typeid(Point3d);
    if (compType.is(py::type::of<Point4d>()))
        return typeid(Point4d);
    if (compType.is(py::type::of<Color>()))
        return typeid(Color);

    throw std::invalid_argument(
        "Unsupported custom component type '" +
        std::string(py::str(compType)) +
        "'. Supported types: int, float, str, Point2, Point3, Point4, Color, "
        "object, or MeshInfo.DataType.");
}

/**
 * @brief Infers the C++ storage type from a Python value when adding a
 * mesh-level custom component without an explicit comp_type.
 * Unsupported types (including bool, dict, list, etc.) fall back to
 * PyCustomObject.
 */
inline std::type_index inferMeshCustomComponentTypeIndex(
    pybind11::handle value)
{
    namespace py = pybind11;

    if (!py::isinstance<py::bool_>(value) && py::isinstance<py::int_>(value))
        return typeid(int);
    if (py::isinstance<py::float_>(value))
        return typeid(double);
    if (py::isinstance<py::str>(value))
        return typeid(std::string);
    if (py::isinstance<Point2d>(value))
        return typeid(Point2d);
    if (py::isinstance<Point3d>(value))
        return typeid(Point3d);
    if (py::isinstance<Point4d>(value))
        return typeid(Point4d);
    if (py::isinstance<Color>(value))
        return typeid(Color);

    return typeid(PyCustomObject);
}

/**
 * @brief Returns the Python type object representing a stored C++ type_index
 * (all C++ integral types map to int; float/double map to float).
 */
inline pybind11::object customComponentPyType(std::type_index ti)
{
    namespace py = pybind11;

    return dispatchCustomComponentType(ti, []<typename T>() -> py::object {
        if constexpr (std::integral<T>) {
            return py::reinterpret_borrow<py::object>(
                reinterpret_cast<PyObject*>(&PyLong_Type));
        }
        else if constexpr (std::floating_point<T>) {
            return py::reinterpret_borrow<py::object>(
                reinterpret_cast<PyObject*>(&PyFloat_Type));
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return py::reinterpret_borrow<py::object>(
                reinterpret_cast<PyObject*>(&PyUnicode_Type));
        }
        else if constexpr (
            isCustomComponentPointV<T> || std::is_same_v<T, Color>) {
            return py::type::of<T>();
        }
        else {
            return py::reinterpret_borrow<py::object>(
                reinterpret_cast<PyObject*>(&PyBaseObject_Type));
        }
    });
}

/**
 * @brief Checks if actualType matches expectedType. MeshInfo.DataType requires
 * an exact type match; Python int/float match any integral or floating C++ type.
 */
inline bool isCustomComponentTypeMatch(
    std::type_index  actualType,
    pybind11::handle expectedType)
{
    namespace py = pybind11;

    std::type_index expectedTi = resolveCustomComponentTypeIndex(expectedType);

    if (py::isinstance<MeshInfo::DataType>(expectedType)) {
        return actualType == expectedTi;
    }

    PyObject* ptr = expectedType.ptr();
    if (ptr == reinterpret_cast<PyObject*>(&PyLong_Type)) {
        return actualType == typeid(char) ||
               actualType == typeid(unsigned char) ||
               actualType == typeid(short) ||
               actualType == typeid(unsigned short) ||
               actualType == typeid(int) || actualType == typeid(uint);
    }
    if (ptr == reinterpret_cast<PyObject*>(&PyFloat_Type)) {
        return actualType == typeid(float) || actualType == typeid(double);
    }

    return actualType == expectedTi;
}

/**
 * @brief Converts a Point<double, DIM>, buffer (e.g. numpy array), or numeric
 * sequence into Point<double, DIM>. Explicitly invokes buffer/sequence paths
 * because py::handle::cast<T>() skips pybind11 implicit conversions.
 */
template<uint DIM>
Point<double, DIM> castPyObjectToPoint(pybind11::handle obj)
{
    namespace py = pybind11;
    using P      = Point<double, DIM>;

    if (py::isinstance<P>(obj)) {
        return obj.cast<P>();
    }
    if (py::isinstance<py::buffer>(obj)) {
        try {
            return P(pyBufferToEigen<1, DIM>(obj.cast<py::buffer>()).transpose());
        }
        catch (const std::exception& e) {
            throw BadCustomComponentTypeException(e.what());
        }
    }
    if (py::isinstance<py::sequence>(obj) && !py::isinstance<py::str>(obj) &&
        !py::isinstance<py::bytes>(obj)) {
        py::sequence seq = obj.cast<py::sequence>();
        if (seq.size() != DIM) {
            throw BadCustomComponentTypeException(
                "Expected sequence of length " + std::to_string(DIM) +
                " for Point" + std::to_string(DIM) + ".");
        }
        P p;
        for (uint i = 0; i < DIM; ++i) {
            py::handle elem = seq[i];
            if (py::isinstance<py::bool_>(elem)) {
                throw BadCustomComponentTypeException(
                    "Boolean value is not valid as Point coordinate.");
            }
            try {
                p(i) = elem.cast<double>();
            }
            catch (const py::cast_error& e) {
                throw BadCustomComponentTypeException(e.what());
            }
        }
        return p;
    }

    throw BadCustomComponentTypeException(
        "Expected Point" + std::to_string(DIM) +
        " (or compatible buffer/sequence).");
}

/**
 * @brief Converts a Python object into C++ type T, rejecting bool for numeric
 * targets and raising BadCustomComponentTypeException on mismatch.
 */
template<typename T>
T castFromPyObject(pybind11::handle obj)
{
    namespace py = pybind11;

    if constexpr (std::is_same_v<T, PyCustomObject>) {
        return PyCustomObject(py::reinterpret_borrow<py::object>(obj));
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        if (!py::isinstance<py::str>(obj)) {
            throw BadCustomComponentTypeException(
                "Expected 'str' value for string custom component.");
        }
        return obj.cast<std::string>();
    }
    else if constexpr (isCustomComponentPointV<T>) {
        return castPyObjectToPoint<T::DIM>(obj);
    }
    else if constexpr (std::is_same_v<T, Color>) {
        if (py::isinstance<Color>(obj)) {
            return obj.cast<Color>();
        }
        if (py::isinstance<Color::ColorABGR>(obj)) {
            return Color(obj.cast<Color::ColorABGR>());
        }
        throw BadCustomComponentTypeException(
            "Expected 'Color' value for Color custom component.");
    }
    else if constexpr (std::integral<T>) {
        if (py::isinstance<py::bool_>(obj) ||
            (!PyLong_Check(obj.ptr()) && !py::hasattr(obj, "__index__"))) {
            throw BadCustomComponentTypeException(
                "Expected integer value for integral custom component.");
        }
        try {
            return obj.cast<T>();
        }
        catch (const py::cast_error& e) {
            throw BadCustomComponentTypeException(e.what());
        }
    }
    else if constexpr (std::floating_point<T>) {
        if (py::isinstance<py::bool_>(obj)) {
            throw BadCustomComponentTypeException(
                "Boolean value is not valid for floating-point custom "
                "component.");
        }
        try {
            return obj.cast<T>();
        }
        catch (const py::cast_error& e) {
            throw BadCustomComponentTypeException(e.what());
        }
    }
    else {
        static_assert(sizeof(T) == 0, "Unsupported custom component type.");
    }
}

/**
 * @brief Exposes a stored C++ value to Python: Point2/3/4 and Color are returned
 * via reference_internal so in-place mutations affect the mesh/element;
 * PyCustomObject unwraps its Python object; scalars/strings are copied.
 */
template<typename T>
pybind11::object toPyCustomComponentValue(T& val, pybind11::handle parent)
{
    namespace py = pybind11;

    if constexpr (std::is_same_v<T, PyCustomObject>) {
        return val.obj;
    }
    else if constexpr (
        isCustomComponentPointV<T> || std::is_same_v<T, Color>) {
        return py::cast(&val, py::return_value_policy::reference_internal, parent);
    }
    else {
        return py::cast(val);
    }
}

// --- Mesh & Element custom component helpers ---

/**
 * @brief Adds a mesh-level custom component, inferring the C++ type from value
 * when compType is None, or default-constructing compType when value is None.
 */
template<MeshConcept MeshType>
void addMeshCustomComponent(
    MeshType&          mesh,
    const std::string& name,
    pybind11::handle   value,
    pybind11::handle   compType)
{
    std::type_index ti = compType.is_none() ?
                             inferMeshCustomComponentTypeIndex(value) :
                             resolveCustomComponentTypeIndex(compType);

    dispatchCustomComponentType(ti, [&]<typename T>() {
        if (!compType.is_none() && value.is_none()) {
            mesh.template addCustomComponent<T>(name);
        }
        else {
            mesh.template addCustomComponent<T>(
                name, castFromPyObject<T>(value));
        }
    });
}

// Guards element proxy validity before accessing parent container storage.
template<typename Holder>
void checkCustomComponentHolder(const Holder& holder)
{
    if constexpr (ElementConcept<Holder>) {
        if (!holder.isValid()) {
            throw std::runtime_error(
                "Cannot access custom component on an invalid or null element "
                "proxy.");
        }
    }
}

template<typename Holder>
pybind11::object getCustomComponent(
    pybind11::object   holderObj,
    const std::string& name)
{
    auto& holder = holderObj.cast<Holder&>();
    checkCustomComponentHolder(holder);
    std::type_index ti = holder.customComponentType(name);

    return dispatchCustomComponentType(ti, [&]<typename T>() {
        return toPyCustomComponentValue(
            holder.template customComponent<T>(name), holderObj);
    });
}

template<typename Holder>
void setCustomComponent(
    Holder&            holder,
    const std::string& name,
    pybind11::handle   value)
{
    checkCustomComponentHolder(holder);
    std::type_index ti = holder.customComponentType(name);

    dispatchCustomComponentType(ti, [&]<typename T>() {
        holder.template customComponent<T>(name) = castFromPyObject<T>(value);
    });
}

template<typename Holder>
std::vector<std::string> customComponentNamesOfType(
    const Holder&    holder,
    pybind11::handle compType)
{
    checkCustomComponentHolder(holder);
    std::vector<std::string> res;
    for (const auto& name : holder.customComponentNames()) {
        if (isCustomComponentTypeMatch(
                holder.customComponentType(name), compType)) {
            res.push_back(name);
        }
    }
    return res;
}

/**
 * @brief Registers the custom component methods shared by Mesh and Element
 * (Vertex, Face, Edge) bindings.
 */
template<typename Holder, typename... Options>
void initCommonCustomComponents(pybind11::class_<Holder, Options...>& c)
{
    namespace py = pybind11;
    using namespace py::literals;

    c.def(
        "has_custom_component",
        [](const Holder& h, const std::string& name) {
            checkCustomComponentHolder(h);
            return h.hasCustomComponent(name);
        },
        "name"_a);

    c.def(
        "is_custom_component_of_type",
        [](const Holder& h, const std::string& name, py::object compType) {
            checkCustomComponentHolder(h);
            return isCustomComponentTypeMatch(
                h.customComponentType(name), compType);
        },
        "name"_a,
        "comp_type"_a);

    c.def(
        "custom_component_type",
        [](const Holder& h, const std::string& name) {
            checkCustomComponentHolder(h);
            return customComponentPyType(h.customComponentType(name));
        },
        "name"_a);

    c.def("custom_component_names", [](const Holder& h) {
        checkCustomComponentHolder(h);
        return h.customComponentNames();
    });

    c.def(
        "custom_component_names_of_type",
        [](const Holder& h, py::object compType) {
            return customComponentNamesOfType(h, compType);
        },
        "comp_type"_a);

    c.def(
        "custom_component",
        [](py::object self, const std::string& name) {
            return getCustomComponent<Holder>(self, name);
        },
        "name"_a);

    c.def(
        "set_custom_component",
        [](Holder& h, const std::string& name, py::object value) {
            setCustomComponent(h, name, value);
        },
        "name"_a,
        "value"_a);
}

// --- Container-level custom component helpers ---

template<ElemId ELEM_ID, MeshConcept MeshType>
void addContainerCustomComponent(
    MeshType&          mesh,
    const std::string& name,
    pybind11::handle   compType)
{
    std::type_index ti = resolveCustomComponentTypeIndex(compType);

    dispatchCustomComponentType(ti, [&]<typename T>() {
        mesh.template addElementCustomComponent<ELEM_ID, T>(name);
    });
}

template<ElemId ELEM_ID, MeshConcept MeshType>
std::vector<std::string> containerCustomComponentNamesOfType(
    const MeshType&  mesh,
    pybind11::handle compType)
{
    std::vector<std::string> res;
    for (const auto& name : mesh.template elementCustomComponentNames<ELEM_ID>()) {
        if (isCustomComponentTypeMatch(
                mesh.template elementCustomComponentType<ELEM_ID>(name),
                compType)) {
            res.push_back(name);
        }
    }
    return res;
}

/**
 * @brief Returns all valid (non-deleted) element values for a custom component
 * column as a NumPy array (scalars, Point2/3/4, Color RGBA) or Python list
 * (str, object). Copies data because ElementContainer stores columns as
 * std::vector<std::any>.
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
pybind11::object getContainerCustomComponentArray(
    const MeshType&    mesh,
    const std::string& name)
{
    namespace py = pybind11;

    std::type_index ti =
        mesh.template elementCustomComponentType<ELEM_ID>(name);
    std::size_t count = mesh.template count<ELEM_ID>();

    return dispatchCustomComponentType(ti, [&]<typename T>() -> py::object {
        if constexpr (isCustomComponentScalarV<T>) {
            using NpT = NumpyScalarTypeT<T>;
            py::array_t<NpT> arr(count);
            auto             r = arr.template mutable_unchecked<1>();
            std::size_t      i = 0;
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                r(i++) = static_cast<NpT>(el.template customComponent<T>(name));
            }
            return arr;
        }
        else if constexpr (isCustomComponentPointV<T>) {
            constexpr std::size_t DIM = T::DIM;
            py::array_t<double>   arr({count, DIM});
            auto                  r = arr.template mutable_unchecked<2>();
            std::size_t           i = 0;
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                const auto& p = el.template customComponent<T>(name);
                for (std::size_t d = 0; d < DIM; ++d) {
                    r(i, d) = p(d);
                }
                ++i;
            }
            return arr;
        }
        else if constexpr (std::is_same_v<T, Color>) {
            py::array_t<std::uint8_t> arr({count, std::size_t(4)});
            auto                      r = arr.template mutable_unchecked<2>();
            std::size_t               i = 0;
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                const auto& c = el.template customComponent<Color>(name);
                r(i, 0)       = c.red();
                r(i, 1)       = c.green();
                r(i, 2)       = c.blue();
                r(i, 3)       = c.alpha();
                ++i;
            }
            return arr;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            py::list    res(count);
            std::size_t i = 0;
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                res[i++] = py::str(el.template customComponent<std::string>(name));
            }
            return res;
        }
        else {
            py::list    res(count);
            std::size_t i = 0;
            for (const auto& el : mesh.template elements<ELEM_ID>()) {
                res[i++] =
                    el.template customComponent<PyCustomObject>(name).obj;
            }
            return res;
        }
    });
}

template<ElemId ELEM_ID, typename T, MeshConcept MeshType>
void assignSequenceColumn(
    MeshType&          mesh,
    const std::string& name,
    pybind11::handle   values,
    std::size_t        count,
    const std::string& expectedDesc)
{
    namespace py = pybind11;

    if (!py::isinstance<py::sequence>(values) ||
        py::isinstance<py::str>(values) || py::isinstance<py::bytes>(values)) {
        throw BadCustomComponentTypeException(
            "Expected " + expectedDesc + " for custom component '" + name +
            "'.");
    }
    py::sequence seq = values.cast<py::sequence>();
    if (seq.size() != count) {
        throw WrongSizeException(
            "Expected sequence of length " + std::to_string(count) +
            " for custom component '" + name + "'.");
    }
    std::vector<T> parsed;
    parsed.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        parsed.push_back(castFromPyObject<T>(seq[i]));
    }
    std::size_t i = 0;
    for (auto el : mesh.template elements<ELEM_ID>()) {
        el.template customComponent<T>(name) = std::move(parsed[i++]);
    }
}

// Validates shape, length and dtype.kind (avoiding silent float->int
// truncation) before returning a C-contiguous typed NumPy view.
template<typename NpT>
pybind11::array_t<NpT, pybind11::array::c_style | pybind11::array::forcecast>
ensureCustomComponentNumpyArray(
    pybind11::handle   values,
    std::size_t        count,
    std::size_t        secondDim,
    bool               requireIntegral,
    const std::string& name,
    const std::string& typeLabel)
{
    namespace py = pybind11;

    auto arrObj = py::reinterpret_borrow<py::array>(values);
    if (secondDim == 0) {
        if (arrObj.ndim() != 1) {
            throw std::invalid_argument(
                "Expected a 1D array for " + typeLabel + " custom component '" +
                name + "'.");
        }
    }
    else {
        if (arrObj.ndim() != 2 ||
            static_cast<std::size_t>(arrObj.shape(1)) != secondDim) {
            throw std::invalid_argument(
                "Expected a 2D array of shape (N, " +
                std::to_string(secondDim) + ") for " + typeLabel +
                " custom component '" + name + "'.");
        }
    }
    if (static_cast<std::size_t>(arrObj.shape(0)) != count) {
        throw WrongSizeException(
            "Expected array of length " + std::to_string(count) +
            " for custom component '" + name + "'.");
    }

    char kind = arrObj.dtype().kind();
    if (requireIntegral ? (kind != 'i' && kind != 'u') :
                          (kind != 'f' && kind != 'i' && kind != 'u')) {
        throw BadCustomComponentTypeException(
            std::string(
                requireIntegral ? "Expected an integer array for " :
                                  "Expected a numeric array for ") +
            typeLabel + " custom component '" + name + "'.");
    }

    auto typed =
        py::array_t<NpT, py::array::c_style | py::array::forcecast>::ensure(
            arrObj);
    if (!typed) {
        throw BadCustomComponentTypeException(
            "Failed to convert array for " + typeLabel + " custom component '" +
            name + "'.");
    }
    return typed;
}

/**
 * @brief Assigns all valid (non-deleted) element values of a custom component
 * column from a NumPy array or Python sequence of length count.
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
void setContainerCustomComponentArray(
    MeshType&          mesh,
    const std::string& name,
    pybind11::handle   values)
{
    namespace py = pybind11;

    std::type_index ti =
        mesh.template elementCustomComponentType<ELEM_ID>(name);
    std::size_t count = mesh.template count<ELEM_ID>();

    dispatchCustomComponentType(ti, [&]<typename T>() {
        if constexpr (isCustomComponentScalarV<T>) {
            using NpT = NumpyScalarTypeT<T>;
            if (py::isinstance<py::array>(values)) {
                auto typed = ensureCustomComponentNumpyArray<NpT>(
                    values, count, 0, std::integral<T>, name, "scalar");
                auto        r = typed.template unchecked<1>();
                std::size_t i = 0;
                for (auto el : mesh.template elements<ELEM_ID>()) {
                    el.template customComponent<T>(name) =
                        static_cast<T>(r(i++));
                }
                return;
            }
            assignSequenceColumn<ELEM_ID, T>(
                mesh, name, values, count, "a 1D array or sequence");
        }
        else if constexpr (isCustomComponentPointV<T>) {
            constexpr std::size_t DIM = T::DIM;
            if (py::isinstance<py::array>(values)) {
                auto typed = ensureCustomComponentNumpyArray<double>(
                    values,
                    count,
                    DIM,
                    false,
                    name,
                    "Point" + std::to_string(DIM));
                auto        r = typed.template unchecked<2>();
                std::size_t i = 0;
                for (auto el : mesh.template elements<ELEM_ID>()) {
                    auto& p = el.template customComponent<T>(name);
                    for (std::size_t d = 0; d < DIM; ++d) {
                        p(d) = r(i, d);
                    }
                    ++i;
                }
                return;
            }
            assignSequenceColumn<ELEM_ID, T>(
                mesh, name, values, count, "a 2D array or sequence");
        }
        else if constexpr (std::is_same_v<T, Color>) {
            if (py::isinstance<py::array>(values)) {
                auto typed = ensureCustomComponentNumpyArray<std::uint8_t>(
                    values, count, 4, true, name, "Color");
                auto        r = typed.template unchecked<2>();
                std::size_t i = 0;
                for (auto el : mesh.template elements<ELEM_ID>()) {
                    el.template customComponent<Color>(name) =
                        Color(r(i, 0), r(i, 1), r(i, 2), r(i, 3));
                    ++i;
                }
                return;
            }
            assignSequenceColumn<ELEM_ID, Color>(
                mesh, name, values, count, "a (N, 4) array or sequence of Color");
        }
        else {
            assignSequenceColumn<ELEM_ID, T>(
                mesh, name, values, count, "a Python sequence");
        }
    });
}

} // namespace detail

/**
 * @brief Binds mesh-level custom component management and access on MeshType.
 */
template<MeshConcept MeshType, typename... Options>
void initMeshCustomComponents(pybind11::class_<MeshType, Options...>& c)
    requires comp::HasCustomComponents<MeshType>
{
    namespace py = pybind11;
    using namespace py::literals;

    detail::initCommonCustomComponents(c);

    c.def(
        "add_custom_component",
        [](MeshType&          m,
           const std::string& name,
           py::object         value,
           py::object         compType) {
            detail::addMeshCustomComponent(m, name, value, compType);
        },
        "name"_a,
        "value"_a     = py::none(),
        "comp_type"_a = py::none());

    c.def(
        "delete_custom_component",
        [](MeshType& m, const std::string& name) {
            m.deleteCustomComponent(name);
        },
        "name"_a);

    c.def("clear_custom_components", [](MeshType& m) {
        m.clearCustomComponents();
    });
}

/**
 * @brief Binds element-level custom component access on Vertex, Face, and Edge.
 */
template<ElementConcept ElementType, typename... Options>
void initElemCustomComponents(pybind11::class_<ElementType, Options...>& c)
    requires comp::HasCustomComponents<ElementType>
{
    detail::initCommonCustomComponents(c);
}

/**
 * @brief Binds container-level custom component management and bulk array
 * getter/setter methods on MeshType for element ELEM_ID.
 */
template<ElemId ELEM_ID, MeshConcept MeshType, typename... Options>
void initContainerCustomComponents(
    pybind11::class_<MeshType, Options...>& c,
    const std::string&                      name)
    requires (MeshType::template hasElementComponent<
              ELEM_ID,
              CompId::CUSTOM_COMPONENTS>())
{
    namespace py = pybind11;
    using namespace py::literals;

    c.def(
        ("has_" + name + "_custom_component").c_str(),
        [](const MeshType& m, const std::string& compName) {
            return m.template hasElementCustomComponent<ELEM_ID>(compName);
        },
        "name"_a);

    c.def(
        ("is_" + name + "_custom_component_of_type").c_str(),
        [](const MeshType&    m,
           const std::string& compName,
           py::object         compType) {
            return detail::isCustomComponentTypeMatch(
                m.template elementCustomComponentType<ELEM_ID>(compName),
                compType);
        },
        "name"_a,
        "comp_type"_a);

    c.def(
        (name + "_custom_component_type").c_str(),
        [](const MeshType& m, const std::string& compName) {
            return detail::customComponentPyType(
                m.template elementCustomComponentType<ELEM_ID>(compName));
        },
        "name"_a);

    c.def((name + "_custom_component_names").c_str(), [](const MeshType& m) {
        return m.template elementCustomComponentNames<ELEM_ID>();
    });

    c.def(
        (name + "_custom_component_names_of_type").c_str(),
        [](const MeshType& m, py::object compType) {
            return detail::containerCustomComponentNamesOfType<ELEM_ID>(
                m, compType);
        },
        "comp_type"_a);

    c.def(
        ("add_" + name + "_custom_component").c_str(),
        [](MeshType& m, const std::string& compName, py::object compType) {
            detail::addContainerCustomComponent<ELEM_ID>(m, compName, compType);
        },
        "name"_a,
        "comp_type"_a);

    c.def(
        ("delete_" + name + "_custom_component").c_str(),
        [](MeshType& m, const std::string& compName) {
            m.template deleteElementCustomComponent<ELEM_ID>(compName);
        },
        "name"_a);

    c.def(
        (name + "_custom_component_array").c_str(),
        [](const MeshType& m, const std::string& compName) {
            return detail::getContainerCustomComponentArray<ELEM_ID>(
                m, compName);
        },
        "name"_a);

    c.def(
        ("set_" + name + "_custom_component_array").c_str(),
        [](MeshType& m, const std::string& compName, py::object values) {
            detail::setContainerCustomComponentArray<ELEM_ID>(
                m, compName, values);
        },
        "name"_a,
        "values"_a);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
