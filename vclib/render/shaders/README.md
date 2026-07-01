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

## Generating Shader Combinations (Combo Shaders)

To maximize performance, it's best practice to avoid dynamic branching (`if`/`else` statements) in shader code. However, managing many small, specialized shaders for every possible combination of features (e.g., shading models, coloring types, cross-section toggles) can lead to massive code duplication and maintenance nightmares.

To solve this, you can write a single base shader and dynamically generate all the necessary specialized variants using a combo `.config` file.

Combo config files can be placed inside the `shaders/vclib` directory and will be automatically parsed by CMake to generate all permutations of the vertex and fragment shaders. The resulting combinations are appended automatically to the registry.

### Combo Config Syntax

A combo config file must define basic information for prefixes and input files, followed by the dimensions (features) to combine.

**Basic Setup:**
- `ENUM_PREFIX <Prefix>`: The prefix used for the generated enum names (e.g., `DRAWABLE_MESH_SURFACE`).
- `DEFINE_PREFIX <Prefix>`: The prefix used for the generated macro definitions (e.g., `SURFACE`).

**Vertex Shader:**
- `VS_FILE <filename>`: If the vertex shader is static (no variations), provide its file name.
- *Alternatively, for dynamically generated vertex shaders:*
  - `VS_IN <filename>`: The base file to `#include` in the generated vertex shaders.
  - `VS_PREFIX <prefix>`: The prefix for the generated `.sc` files.

**Fragment Shader (Always Dynamic):**
- `FS_IN <filename>`: The base file to `#include` in the generated fragment shaders.
- `FS_PREFIX <prefix>`: The prefix for the generated `.sc` files.

**Dimensions:**
Dimensions define the variants. For each combination, CMake generates the required `#define <DEFINE_PREFIX>_<DIMENSION>_<VALUE> 1` macros and creates a unique shader file.

You must declare dimensions using one of the following keywords depending on which shader stage they apply to:
- `DIM_VS <DimensionName>`: The macro is defined only in the Vertex Shader.
- `DIM_FS <DimensionName>`: The macro is defined only in the Fragment Shader.
- `DIM_BOTH <DimensionName>`: The macro is defined in both Vertex and Fragment Shaders.

*Example:*
```
ENUM_PREFIX DRAWABLE_MESH_SURFACE
DEFINE_PREFIX SURFACE

VS_IN surface_vs_in.sh
VS_PREFIX vs_surface

FS_IN surface_in.sh
FS_PREFIX fs_surface

DIM_BOTH SECTION
    ENABLED
    DISABLED

DIM_FS SHADING
    FLAT
    SMOOTH

DIM_FS COLOR
    FACE
    VERTEX
```
In this example, CMake will generate 2 variants of the vertex shader (Section enabled/disabled) and 8 variants of the fragment shader (2x2x2). It will automatically register 8 complete programs in the combo registry that correctly pair the generated vertex and fragment shaders.
