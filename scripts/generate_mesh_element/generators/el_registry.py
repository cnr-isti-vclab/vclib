"""Registration of the new element in the files that list all the elements."""

import re

from . import common


def update_element_enum(element):
    """Adds the element to ElemId and to ELEMENT_ENUM_STRINGS."""
    path = 'elements/base/base.h'
    text = common.read_file(element, path)
    if re.search(rf'^\s+{element.name_upper}(\s*=.*)?\s*,\s*$', text, re.M):
        return

    text = text.replace(
        '    ELEMENT_COUNT,',
        f'    {element.name_upper},\n    ELEMENT_COUNT,', 1)

    m = re.search(
        r'ELEMENT_ENUM_STRINGS\[static_cast<uint>\(ElemId::ELEMENT_COUNT\)\] = \{\n.*?\n\};',
        text, re.S)
    block = m.group(0)
    if f'"{element.upper_camel}"' not in block:
        new_block = block[:-len('};')] + f'    "{element.upper_camel}",\n' + '};'
        text = text.replace(block, new_block, 1)
    common.update_file(element, path, text)


def update_traits(element):
    """Adds <Element>Type and Const<Element>Type to mesh/traits.h."""
    path = 'traits.h'
    text = common.read_file(element, path)
    if f'struct {element.upper_camel}TypeHelper' in text:
        return

    helpers = common.render_template('traits_helpers.txt', element)
    aliases = common.render_template('traits_aliases.txt', element)
    helpers = helpers.rstrip('\n') + '\n\n'
    aliases = aliases.rstrip('\n') + '\n\n'

    # the new element is added before the Wedge ones
    m = re.search(r'^// =+\n// WedgeTypeHelper', text, re.M)
    text = text[:m.start()] + helpers + text[m.start():]
    anchor = '/**\n * @brief Extracts the Wedge proxy type'
    text = common.insert_before(text, anchor, aliases)
    common.update_file(element, path, text)
