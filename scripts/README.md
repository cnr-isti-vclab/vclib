# VCLib Scripts

Here there are some convenience scripts that allow to keep in order the library.

- `happy_new_year.py`: this script overwrites the current year in the file `templates/headings.txt`, which is the file used to fill the heading lines of every header/source of the library.
- `replace_headings.py`: this script overwrites the heading lines of every header/source file of the library, and puts the content of the file `templates/headings.txt`.
- `replace_header_guards.py`: this script replaces the `ifndef/#define/#endif` guards of the headers of the library, by using the scheme of the path of each file, allowing to avoid collisions and keep the library consistent.
- `update_version.py`: this script updates the version of VCLib in all relevant files (VERSION file and version.h header). Usage: `python scripts/update_version.py <new_version>` (e.g., `python scripts/update_version.py 1.2.3`).
- `benchmarks/compare.py`: compares two benchmark JSON reports, or runs two benchmark executables and compares them.
- `benchmarks/compare_branches.py`: builds the core mesh benchmark on the current working tree and on another branch (in a git worktree under `out/worktrees`), runs both on the same input, saves the JSON reports in `benchmarks/results/<date_time>_<branch>.json` and compares them. The other branch is the baseline. Usage: `python scripts/benchmarks/compare_branches.py <branch> [--mesh <file> | --grid <rows> <cols>] [-n <iterations>] [-j <jobs>] [--cpu <n>] [--no-build] [--remove-worktree]`. Without `--mesh`, a synthetic grid is used. For building only the benchmark of the current branch, use the `vclib-core-benchmarks` CMake preset (`cmake --preset vclib-core-benchmarks && cmake --build --preset vclib-core-benchmarks`).
