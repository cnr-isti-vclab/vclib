# VCLib Scripts

Here there are some convenience scripts that allow to keep in order the library.

- `happy_new_year.py`: this script overwrites the current year in the file `templates/headings.txt`, which is the file used to fill the heading lines of every header/source of the library.
- `replace_headings.py`: this script overwrites the heading lines of every header/source file of the library, and puts the content of the file `templates/headings.txt`.
- `replace_header_guards.py`: this script replaces the `ifndef/#define/#endif` guards of the headers of the library, by using the scheme of the path of each file, allowing to avoid collisions and keep the library consistent.
