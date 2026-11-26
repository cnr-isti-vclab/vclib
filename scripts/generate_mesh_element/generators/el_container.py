from . import common
from .components import COMMON_CONTAINER_COMPONENTS


def _extra(element):
    macros = ''
    for c in element.components:
        if c.optional and c.name not in COMMON_CONTAINER_COMPONENTS:
            macros += (
                f'    VCL_CONTAINER_COMPONENT_METHODS({element.upper_camel}, '
                f'{c.upper_camel}, {c.comp_id})\n')

    if element.fixed_vertex_count:
        requires = f' requires (sizeof...(Args) == {element.vertex_count})'
    else:
        requires = ' requires (sizeof...(Args) > 1)'

    return {
        '%CONTAINER_COMPONENT_METHODS%': macros,
        '%ADD_VERTICES_REQUIRES%': requires,
    }


def _add_container_concept(element):
    path = 'containers/base/element_container.h'
    name = f'{element.upper_camel}ContainerConcept'
    text = common.read_file(element, path)
    if name in text:
        return

    new = (
        '/**\n'
        f' * @brief Concept that checks whether a type T is an ElementContainer '
        f'of {element.plural_upper_camel}.\n'
        ' */\n'
        'template<typename T>\n'
        f'concept {name} =\n'
        '    ElementContainerConcept<T> && '
        f'(std::remove_cvref_t<T>::ELEMENT_ID == ElemId::{element.name_upper});'
        '\n\n')
    text = common.insert_before(text, '} // namespace vcl::mesh', new, last=True)
    common.update_file(element, path, text)


def generate_container(element):
    include = f'containers/{element.name}_container.h'
    content = common.render_template(
        'element_container.h', element, _extra(element))
    common.write_file(element, include, content)

    _add_container_concept(element)
    common.insert_include(element, 'containers.h', include)
    return include
