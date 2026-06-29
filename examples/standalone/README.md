# VCLib Standalone Examples & Testing

This directory contains the infrastructure for testing VCLib as an external dependency, as well as a ready-to-use template project for new users.

## 1. Automated Integration Tests

The root `CMakeLists.txt` in this directory is a unified project designed to test how VCLib is consumed by external projects. Instead of duplicating the example source code, it dynamically compiles the `main.cpp` files from the original `../core/` examples.

### Configuration Modes

You can configure this project to test two different integration methods:

#### `find_package` (Default)
Tests the consumption of VCLib through an installed version on the system.
```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/your/vclib/install
cmake --build .
ctest --output-on-failure
```

#### `FetchContent`
Tests the consumption of VCLib by downloading and embedding the repository at configure time.
```bash
mkdir build && cd build
cmake .. -DVCLIB_USE_FETCHCONTENT=ON
cmake --build .
ctest --output-on-failure
```

## 2. Template Project (`template-project`)

If you are looking for a quick, ready-to-use boilerplate to start a new project with VCLib, check out the `template-project` directory. 
It contains a minimal self-contained `CMakeLists.txt` and a `main.cpp` that demonstrate how to fetch VCLib via `FetchContent` and create a basic mesh. You can copy this folder anywhere on your computer and start coding immediately!

## CI Testing

These tests are automatically executed on all supported platforms via GitHub Actions:
- **Ubuntu** (x86_64 and ARM)
- **macOS** (Intel and Apple Silicon)
- **Windows** (x86_64)

See [StandaloneExamples.yml](../../.github/workflows/StandaloneExamples.yml) for the workflow configuration.

## License

These examples are provided under the same license as VCLib (MPL 2.0).
