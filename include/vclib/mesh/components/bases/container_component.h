#ifndef VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H

#include <vclib/space/vector.h>

#include "component.h"

namespace vcl::comp {

namespace internal {

// small alias to differentiate between components having only a container or
// components having a container and an additional data type
// if AD is void, no additional data and the component will have only a Vector
// if AD is not void, there is additional data, and the component will have
// a tuple containing a Vector and the additional data
template<
	uint CT,
	typename T,
	int N,
	typename AD,
	typename El,
	bool o,
	bool TT,
	typename... PT>
using ContCompBase = std::conditional_t<
	std::is_same_v<AD, void>,
	Component<CT, Vector<T, N>, El, o, PT...>,
	Component<CT, std::tuple<Vector<T, N>, AD>, El, o, PT...>>;

} // namespace vcl::comp::internal

/**
 * @brief The ContainerComponent class is the base class for all the components
 * of VCLib that store a container of objects of a given type T.
 *
 * A component that inherits from this class will have all the features of the
 * @ref Component class, plus automatic storage of the data in a Container of
 * objects that could have static or dynamic size, and the possibility to tie
 * the size of the Container to the number of Vertices of the Element (E.g.
 * useful for some Face components like AdjacentFaces or WedgeTexCoords).
 *
 * For further details , please refer to the page @ref implement_component page.
 *
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_TYPE: The type of the component. It is a value of the enum
 * ComponentType, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam T: The type of the data that the component needs to store in a
 * Container. E.g. a WedgeTexCoord component would have vcl::TexCoordd as
 * DataType.
 * @tparam N: The size of the container: if >= 0 the size is static, if < 0 the
 * size is dynamic.
 * @tparam AdditionalData: the type of additional data that could be stored
 * outside the Container. If you don't have additional data. set AdditionalData
 * = void. E.g. a WedgeTexCoord component stores a short as AdditionalData that
 * indicates the texture index used by all the Wedge TexCoords.
 * @tparam ElementType: This type is used to discriminate between horizontal and
 * vertical components. When a component is horizontal, this type must be void.
 * When a component is vertical, this type must be the type of the Element that
 * has the component, and it will be used by the vcl::Mesh to access to the data
 * stored vertically.
 * @tparam OPT: When a component is vertical, it could be optional, that means
 * that could be enabled/disabled at runtime. To make the component optional,
 * this template parameter must be true.
 * @tparam TTVN: "Tied To Vertex Number", this boolean tells whether the
 * component should leave liberty to control the size of the Container to the
 * user, or it should be tied to the vertex number and changed automatically
 * when the number of vertices changes. E.g. a WedgeTexCoord component is a
 * Container that should be tied to the vertex number, meaning that the number
 * of texture coordinates should be always be the same of the number of
 * vertices. Setting this value to true will tie the size of this container to
 * the number of vertices.
 * @tparam PointedTypes: Variadic Template types of all the Pointer types that
 * the component stores, and that need to be updated when some reallocation
 * happens.
 */
template<
	typename DerivedComponent,
	uint COMP_TYPE,
	typename T,
	int N,
	typename AdditionalData,
	typename ElementType,
	bool OPT,
	bool TTVN,
	typename... PointedTypes>
class ContainerComponent :
		public internal::ContCompBase<
			COMP_TYPE,
			T,
			N,
			AdditionalData,
			ElementType,
			OPT,
			TTVN,
			PointedTypes...>
{
	static constexpr bool HAS_ADDITIONAL_DATA =
		!std::is_same_v<AdditionalData, void>;

	using Base = internal::ContCompBase<
		COMP_TYPE,
		T,
		N,
		AdditionalData,
		ElementType,
		OPT,
		TTVN,
		PointedTypes...>;

public:
	/**
	 * @brief Boolean that tells if this component stores a container having its
	 * size tied to the number of the vertices of the Element.
	 *
	 * E.g. suppose to have a Polygonal Face f (dynamic number of vertices),
	 * having 5 vertices (and 5 edges). This means that if the Face has the
	 * AdjacentFaces component, then it should store 5 adjacent faces (the same
	 * number of the vertices). In this case, the AdjacentFaces component will
	 * have the boolean TIED_TO_VERTEX_NUMBER set to true.
	 */
	static const bool TIED_TO_VERTEX_NUMBER = TTVN;

	static const int SIZE = N;

protected:
	/* Iterator Types declaration */

	using Iterator      = typename Vector<T, N>::Iterator;
	using ConstIterator = typename Vector<T, N>::ConstIterator;

	/* Constructor */

	ContainerComponent();

	void init();

	Vector<T, N>& container();

	const Vector<T, N>& container() const;

	template<typename AdDt = AdditionalData>
	AdDt& additionalData() requires (HAS_ADDITIONAL_DATA);

	template<typename AdDt = AdditionalData>
	const AdDt& additionalData() const
		requires (HAS_ADDITIONAL_DATA);
};

} // namespace vcl::comp

#include "container_component.cpp"

#endif // VCL_MESH_COMPONENTS_BASES_CONTAINER_COMPONENT_H
