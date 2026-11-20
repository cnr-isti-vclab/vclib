import bisect
import re
from pathlib import Path

from .components import COMPONENTS, get_component

SCRIPT_DIR = Path(__file__).resolve().parents[1]
REPO_ROOT = SCRIPT_DIR.parents[1]
TEMPLATES_DIR = SCRIPT_DIR / 'templates'
HEADINGS_FILE = REPO_ROOT / 'scripts' / 'templates' / 'headings.txt'
DEFAULT_MESH_DIR = REPO_ROOT / 'vclib' / 'core' / 'include' / 'vclib' / 'mesh'


def _camel(name):
    return ''.join(w.capitalize() for w in name.split('_'))


class Element:
    """Everything the generators need to know about the element to create."""

    def __init__(self, name, mesh_dir, namespace=None, plural=None,
                 required=None, components=None, vertex_count=None):
        self.name = name
        self.mesh_dir = Path(mesh_dir)
        self.name_upper = name.upper()
        self.upper_camel = _camel(name)
        self.camel = self.upper_camel[0].lower() + self.upper_camel[1:]
        self.namespace = namespace or name.replace('_', '')[:4]
        self.plural = plural or name + 's'
        self.plural_upper_camel = _camel(self.plural)
        self.plural_camel = (
            self.plural_upper_camel[0].lower() + self.plural_upper_camel[1:])
        self.text = name.replace('_', ' ')
        self.text_plural = self.plural.replace('_', ' ')
        self.article = 'an' if name[0] in 'aeiou' else 'a'

        required = ['bit_flags'] + list(required or [])
        components = list(components or [])
        if vertex_count is not None:
            required.append('vertex_references')
        self.vertex_count = -1 if vertex_count is None else vertex_count

        self.required = []
        for r in required:
            if r not in self.required:
                self.required.append(r)
        names = set(self.required) | set(components)
        self.components = [get_component(n) for n in sorted(names)]
        self.required = [get_component(n) for n in self.required]

    def has(self, comp_name):
        return any(c.name == comp_name for c in self.components)

    @property
    def has_vertex_references(self):
        return self.has('vertex_references')

    @property
    def fixed_vertex_count(self):
        return self.has_vertex_references and self.vertex_count > 0

    def tokens(self):
        return {
            '%EL%': self.name,
            '%EL_U%': self.name_upper,
            '%EL_UC%': self.upper_camel,
            '%EL_C%': self.camel,
            '%EL_NS%': self.namespace,
            '%EL_TXT%': self.text,
            '%EL_UC_PL%': self.plural_upper_camel,
            '%EL_C_PL%': self.plural_camel,
            '%EL_TXT_PL%': self.text_plural,
            '%EL_A%': self.article,
            '%EL_A_UC%': self.article.capitalize(),
        }

    def flags(self):
        return {
            'vertex_references': self.has_vertex_references,
            'fixed_vertex_count': self.fixed_vertex_count,
        }


def replace_tokens(text, element, extra=None):
    tokens = dict(extra or {})
    if '%HEADER%' in text:
        tokens['%HEADER%'] = HEADINGS_FILE.read_text().rstrip('\n')
    # the extra tokens can contain element tokens, therefore they go first
    tokens.update(element.tokens())
    for k, v in tokens.items():
        text = text.replace(k, v)
    return text


def preprocess(text, flags):
    """Handles the //@if flag, //@else, //@endif lines of the templates."""
    out = []
    stack = []  # list of (parent_active, condition)
    active = True
    for line in text.split('\n'):
        s = line.strip()
        if s.startswith('//@if '):
            flag = s[len('//@if '):].strip()
            negate = flag.startswith('!')
            value = flags[flag.lstrip('!')]
            stack.append((active, value != negate))
            active = active and (value != negate)
        elif s == '//@else':
            parent, cond = stack[-1]
            stack[-1] = (parent, not cond)
            active = parent and not cond
        elif s == '//@endif':
            parent, _ = stack.pop()
            active = parent
        elif active:
            out.append(line)
    assert not stack, 'unbalanced //@if in template'
    return '\n'.join(out)


def render_template(template_name, element, extra=None):
    text = (TEMPLATES_DIR / template_name).read_text()
    text = preprocess(text, element.flags())
    text = replace_tokens(text, element, extra)
    text = re.sub(r'\n{3,}', '\n\n', text)
    return text.rstrip('\n') + '\n'


def write_file(element, relative_path, content):
    path = element.mesh_dir / relative_path
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content)
    return relative_path


# --- helpers to update existing files ---------------------------------------

def read_file(element, relative_path):
    return (element.mesh_dir / relative_path).read_text()


def update_file(element, relative_path, content):
    (element.mesh_dir / relative_path).write_text(content)


def insert_include(element, relative_path, include, before=None):
    """
    Adds `#include "<include>"` in the given header. If the existing includes
    are alphabetically sorted the new one is inserted in order, otherwise it is
    inserted before the include `before` (or after the last include).
    Does nothing if the include is already present.
    """
    text = read_file(element, relative_path)
    line = f'#include "{include}"'
    if line in text.split('\n'):
        return
    lines = text.split('\n')
    idx = [i for i, l in enumerate(lines) if l.startswith('#include "')]
    inc = [lines[i] for i in idx]
    if inc == sorted(inc):
        pos = idx[0] + bisect.bisect_left(inc, line)
    elif before is not None and f'#include "{before}"' in inc:
        pos = idx[inc.index(f'#include "{before}"')]
    else:
        pos = idx[-1] + 1
    lines.insert(pos, line)
    update_file(element, relative_path, '\n'.join(lines))


def insert_before(text, anchor, new_text, last=False):
    """Inserts new_text before the first (or last) occurrence of anchor."""
    i = text.rindex(anchor) if last else text.index(anchor)
    return text[:i] + new_text + text[i:]
