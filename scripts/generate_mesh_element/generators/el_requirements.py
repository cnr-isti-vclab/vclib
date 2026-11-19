import re

from . import common
from .components import MESH_CONCEPTS_ORDER, MESH_CONCEPTS_TITLES, COMPONENTS


def _concept_name(element, comp, optional):
    prefix = f'Has{element.upper_camel}'
    return f'{prefix}{"Optional" if optional else ""}{comp.upper_camel}'


def _mesh_concepts_block(element, comp):
    eid = f'ElemId::{element.name_upper}'
    cid = f'CompId::{comp.comp_id}'
    name = _concept_name(element, comp, False)
    oname = _concept_name(element, comp, True)
    if comp.kind == 'adjacency':
        generic = f'HasElement{comp.upper_camel}<M, {eid}>'
        optional = f'HasElementOptional{comp.upper_camel}<M, {eid}>'
    elif comp.kind == 'custom':
        generic = f'HasElement{comp.upper_camel}<M, {eid}>'
        optional = None
    else:
        generic = f'HasElementComponent<M, {eid}, {cid}>'
        optional = f'HasElementOptionalComponent<M, {eid}, {cid}>'

    text = f'template<typename M>\nconcept {name} =\n    {generic};\n'
    if optional is not None:
        text += f'\ntemplate<typename M>\nconcept {oname} =\n    {optional};\n'
    return text


def _mesh_concepts(element):
    out = ''
    adjacencies_title_done = False
    for name in MESH_CONCEPTS_ORDER:
        if not element.has(name):
            continue
        comp = COMPONENTS[name]
        if comp.kind == 'adjacency':
            title = None if adjacencies_title_done else 'Adjacencies'
            adjacencies_title_done = True
        else:
            title = MESH_CONCEPTS_TITLES[name]
        if title:
            out += f'/* {title} */\n'
        out += _mesh_concepts_block(element, comp) + '\n'
    return out


def _update_mesh_concepts_base(element):
    if element.name_upper == 'VERTEX':
        return
    path = 'requirements/mesh_concepts/base.h'
    text = common.read_file(element, path)
    has_name = f'Has{element.plural_upper_camel}'
    mesh_concept = f'{element.upper_camel}MeshConcept'
    if f'concept {has_name} ' in text:
        return

    has_concept = (
        '/**\n'
        f' * @brief Concept that checks whether a Mesh has {element.article} '
        f'{element.upper_camel}Container.\n'
        ' * @ingroup mesh_concepts\n'
        ' */\n'
        'template<typename MeshType>\n'
        f'concept {has_name} = HasContainerOf<MeshType, '
        f'ElemId::{element.name_upper}>;\n\n')
    mesh_concept_text = (
        '/**\n'
        f' * @brief Concept that checks whether a type is a Mesh with '
        f'{element.text_plural}.\n'
        ' * @ingroup mesh_concepts\n'
        ' */\n'
        'template<typename M>\n'
        f'concept {mesh_concept} = MeshConcept<M> && {has_name}<M>;\n\n')

    # the Has<Elements> concepts are before the <Element>MeshConcept ones
    m = list(re.finditer(
        r'concept \w+ = HasContainerOf<MeshType, ElemId::\w+>;\n\n', text))[-1]
    text = text[:m.end()] + has_concept + text[m.end():]
    m = list(re.finditer(
        r'concept \w+MeshConcept = MeshConcept<M> && Has\w+<M>;\n\n', text))[-1]
    text = text[:m.end()] + mesh_concept_text + text[m.end():]
    common.update_file(element, path, text)


def _contracts_functions(element, comp):
    eid = f'ElemId::{element.name_upper}'
    cid = f'CompId::{comp.comp_id}'
    name = comp.upper_camel
    prefix = element.upper_camel
    out = ''
    if comp.kind != 'custom':
        out += (
            'template<MeshConcept MeshType>\n'
            f'bool enableIf{prefix}{name}Optional(MeshType& m)\n'
            '{\n'
            f'    return enableIfElementComponentOptional<{eid}, {cid}>(m);\n'
            '}\n\n')
    out += (
        'template<MeshConcept MeshType>\n'
        f'requires (Has{prefix}{name}<MeshType>)\n'
        f'void require{prefix}{name}(const MeshType& m)\n'
        '{\n'
        f'    requireElementComponent<{eid}, {cid}>(m);\n'
        '}\n\n')
    return out


def _update_contracts(element):
    path = 'requirements/contracts.h'
    text = common.read_file(element, path)

    # compactness
    fn = f'require{element.upper_camel}Compactness'
    if fn not in text:
        new = (
            'template<MeshConcept MeshType>\n'
            f'requires (Has{element.plural_upper_camel}<MeshType>)\n'
            f'void {fn}(const MeshType& m)\n'
            '{\n'
            f'    requireElementCompactness<ElemId::{element.name_upper}>(m);\n'
            '}\n\n')
        anchor = '/* ' + '=' * 73 + '\n * 2. Mesh Topology Checks'
        text = common.insert_before(text, anchor, new)

    # per component functions
    for comp in element.components:
        if comp.kind is None:
            continue
        fn = f'require{element.upper_camel}{comp.upper_camel}'
        if re.search(rf'\b{fn}\(', text):
            continue
        marker = f'/* --- {comp.contracts_section} --- */'
        start = text.index(marker) + len(marker)
        m = re.search(r'^/\* (---|===)|^} // namespace vcl', text[start:], re.M)
        end = start + m.start()
        text = text[:end] + _contracts_functions(element, comp) + text[end:]
    common.update_file(element, path, text)


def generate_requirements(element):
    include = f'mesh_concepts/{element.name}.h'
    content = common.render_template(
        'mesh_concepts.h', element, {'%MESH_CONCEPTS%': _mesh_concepts(element)})
    common.write_file(element, f'requirements/{include}', content)

    _update_mesh_concepts_base(element)
    common.insert_include(
        element, 'requirements/mesh_concepts.h', include,
        before='mesh_concepts/mesh.h')
    _update_contracts(element)
    return f'requirements/{include}'
