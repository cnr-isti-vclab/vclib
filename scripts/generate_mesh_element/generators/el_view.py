from . import common


def generate_view(element):
    include = f'elements/{element.name}.h'
    content = common.render_template('view.h', element)
    common.write_file(element, f'views/{include}', content)
    common.insert_include(element, 'views/elements.h', include, before='elements/wedge.h')
    return f'views/{include}'
