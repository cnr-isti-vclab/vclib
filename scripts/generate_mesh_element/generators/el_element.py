from . import common
from .components import COMPONENTS


def _static_assert_class(element, comp):
    return (
        '    static_assert(\n'
        f'        comp::Has{comp.upper_camel}<Components>,\n'
        f'        "{element.article.capitalize()} {element.upper_camel} must '
        f'have the {comp.upper_camel} component "\n'
        f'        "(vcl::comp::{comp.upper_camel}).");\n')


def _static_assert_components(element, comp):
    return (
        '    static_assert(\n'
        f'        comp::Has{comp.upper_camel}<vcl::TypeWrapper<Comps...>>,\n'
        f'        "{element.upper_camel} components must include '
        f'{comp.upper_camel} "\n'
        f'        "(vcl::comp::{comp.upper_camel}).");\n')


def _extra(element):
    cls = ''
    cmp_ = ''
    concept_clauses = ''
    required_clauses = []
    names = []
    includes = ''
    count = element.vertex_count
    for c in element.required:
        cls += _static_assert_class(element, c)
        cmp_ += _static_assert_components(element, c)
        concept_clauses += (
            f' &&\n    comp::Has{c.upper_camel}<typename '
            'std::remove_cvref_t<T>::Components>')
        required_clauses.append(f'comp::Has{c.upper_camel}<T>')
        includes += f'#include <vclib/mesh/components/{c.name}.h>\n'
        if c.name == 'vertex_references' and element.fixed_vertex_count:
            names.append(f'VertexReferences with VERTEX_COUNT == {count}')
            msg = (f'{element.article.capitalize()} {element.upper_camel} '
                   f'must have exactly {count} vertices.')
            cls += ('    static_assert(\n'
                    f'        VERTEX_COUNT == {count},\n'
                    f'        "{msg}");\n')
            cmp_ += ('    static_assert(\n'
                     '        comp::ComponentTypeT<\n'
                     '            CompId::VERTEX_REFERENCES,\n'
                     '            vcl::TypeWrapper<Comps...>>::VERTEX_COUNT == '
                     f'{count},\n'
                     f'        "{msg}");\n')
            concept_clauses += (
                ' &&\n    (std::remove_cvref_t<T>::VERTEX_COUNT == '
                f'{count})')
            required_clauses.append(
                '(comp::ComponentTypeT<CompId::VERTEX_REFERENCES, T>::'
                f'VERTEX_COUNT == {count})')
        else:
            names.append(c.upper_camel)

    if len(names) > 1:
        text = ', '.join(names[:-1]) + ' and ' + names[-1]
    else:
        text = names[0]
    return {
        '%CLASS_STATIC_ASSERTS%': cls.rstrip('\n'),
        '%COMPONENTS_STATIC_ASSERTS%': cmp_.rstrip('\n'),
        '%ELEMENT_CONCEPT_CLAUSES%': concept_clauses,
        '%REQUIRED_CONCEPT_CLAUSES%': ' &&\n    '.join(required_clauses),
        '%REQUIRED_TEXT%': text,
        '%REQUIRED_INCLUDES%': includes,
    }


def generate_element(element):
    path = f'elements/{element.name}.h'
    content = common.render_template('element.h', element, _extra(element))
    common.write_file(element, path, content)
    common.insert_include(element, 'elements.h', f'elements/{element.name}.h')
    return path


def _split_snippet(text):
    concepts, aliases = text.split('//@aliases\n')
    return concepts.replace('//@concepts\n', ''), aliases


def generate_element_components(element):
    path = f'elements/{element.name}_components.h'

    includes = ''.join(
        f'#include "../components/{c.name}.h"\n' for c in element.components)

    concepts = ''
    aliases = []
    for c in element.components:
        text = (common.TEMPLATES_DIR / 'components' / f'{c.name}.txt').read_text()
        conc, alias = _split_snippet(text)
        concepts += conc
        aliases.append(alias.strip('\n') + '\n')

    extra = {
        '%INCLUDE_COMPONENTS%': includes,
        '%COMPONENTS_CONCEPTS%': concepts,
        '%COMPONENTS_ALIASES%': '\n'.join(aliases),
        '%VERTEX_COUNT%': str(element.vertex_count),
    }
    content = common.render_template('element_components.h', element, extra)
    return common.write_file(element, path, content)
