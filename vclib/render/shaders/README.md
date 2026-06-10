# Embedded Shaders Registry

Add shader programs to the library by registering them in a `.config` file.

## Adding a Vertex-Fragment Program

1. Add an entry to `embedded_vf_programs.config`:
```
MY_PROGRAM
    vclib/shaders/my_program/vs_my_program.sc
    vclib/shaders/my_program/fs_my_program.sc
```

2. Access it via `ProgramManager`:
```cpp
bgfx::ProgramHandle prog = Context::instance().programManager().getProgram<VertFragProgram::MY_PROGRAM>();
```

## Adding a Compute Program

1. Add an entry to `embedded_c_programs.config`:
```
MY_COMPUTE
    vclib/shaders/my_compute/cs_my_compute.sc
```

2. Access it via `ProgramManager`:
```cpp
bgfx::ProgramHandle prog = Context::instance().programManager().getComputeProgram<ComputeProgram::MY_COMPUTE>();
```

## Config Format

Each entry consists of:
- `ENUM_NAME` - uppercase identifier (becomes an enum value)
- Indented shader file paths (one per line)

Entries are separated by blank lines. Lines starting with `#` are comments.
