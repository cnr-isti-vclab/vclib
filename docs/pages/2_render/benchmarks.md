# Benchmarking Framework {#benchmarks}

[TOC]

The **render** module of VCLib includes a comprehensive framework for executing performance benchmarks on 3D mesh rendering. This framework allows you to automate performance tests, measure metrics such as FPS and execution time, and save results in various formats.

## Overview

The benchmarking framework consists of three main components:

1. **Automation Actions** - Automated actions that are executed during the benchmark (rotations, scaling, etc.)
2. **Metrics** - Metrics to measure performance (FPS, time, etc.)
3. **Printers** - Components to save/print benchmark results (CSV, JSON, stdout, etc.)

## Main Components

### BenchmarkDrawer

The `vcl::BenchmarkDrawer` class is the heart of the framework. It manages the execution of automations, the measurement of metrics, and the printing of results.

```cpp
#include <vclib/render/drawers/benchmark_drawer.h>

using BenchmarkViewer = vcl::RenderApp<
    vcl::glfw::WindowManager,
    vcl::Canvas,
    vcl::BenchmarkViewerDrawer>;

BenchmarkViewer viewer("Benchmark", width, height);
```

### BenchmarkViewerDrawer

`vcl::BenchmarkViewerDrawer` combines the functionality of `ViewerDrawer` (to display meshes) with that of `BenchmarkDrawer`, allowing you to run benchmarks on scenes with loaded meshes.

## Automation Actions

**Automation Actions** define the operations to be executed during the benchmark. Several predefined actions are available:

### Rotations

#### PerFrameRotationAutomationAction
Rotates the scene by a certain angle per frame:

```cpp
#include <vclib/render/automation/actions.h>

// Complete rotation in 1000 frames around the Z axis
auto rotation = vcl::PerFrameRotationAutomationAction<BmarkDrawerT>::
    fromFramesPerRotation(1000, {0.f, 0.f, 1.f});
```

#### RotationAutomationAction
Rotates the scene by a certain angle per second (based on real time):

```cpp
// Complete rotation in 5 seconds around the Y axis
auto rotation = vcl::RotationAutomationAction<BmarkDrawerT>::
    fromSecondsPerRotation(5.0f, {0.f, 1.f, 0.f});
```

### Scaling

#### PerFrameChangeScaleMultiplicativeAutomationAction
Modifies the scene scale in a multiplicative way:

```cpp
// Multiplicative scale: finalScale = initialScale * (1 + deltaScale * duration)
auto scaling = vcl::PerFrameChangeScaleMultiplicativeAutomationAction<BmarkDrawerT>(0.01f);
```

### Composite Actions

#### SequentialAutomationActions
Executes a sequence of automations in order:

```cpp
vcl::SequentialAutomationActions<BmarkDrawerT> sequence({
    std::make_shared<vcl::PerFrameRotationAutomationAction<BmarkDrawerT>>(...),
    std::make_shared<vcl::PerFrameChangeScaleMultiplicativeAutomationAction<BmarkDrawerT>>(...)
});
```

#### SimultaneousAutomationActions
Executes multiple automations simultaneously:

```cpp
vcl::SimultaneousAutomationActions<BmarkDrawerT> simultaneous({
    std::make_shared<vcl::PerFrameRotationAutomationAction<BmarkDrawerT>>(...),
    std::make_shared<vcl::PerFrameChangeScaleMultiplicativeAutomationAction<BmarkDrawerT>>(...)
});
```

### Wrapper Actions

Wrappers allow you to add conditions to automations:

#### FrameLimitedAutomationAction
Limits the duration of an automation to a specific number of frames:

```cpp
auto limitedRotation = vcl::FrameLimitedAutomationAction<BmarkDrawerT>(
    rotation,  // automation to limit
    500        // maximum number of frames
);
```

#### TimeLimitedAutomationAction
Limits the duration of an automation to a specific time:

```cpp
auto limitedRotation = vcl::TimeLimitedAutomationAction<BmarkDrawerT>(
    rotation,  // automation to limit
    10.0f      // maximum duration in seconds
);
```

#### FrameDelayAutomationAction
Delays the start of an automation by a specific number of frames:

```cpp
auto delayedRotation = vcl::FrameDelayAutomationAction<BmarkDrawerT>(
    rotation,  // automation to delay
    100        // delay in frames
);
```

#### TimeDelayAutomationAction
Delays the start of an automation by a specific time:

```cpp
auto delayedRotation = vcl::TimeDelayAutomationAction<BmarkDrawerT>(
    rotation,  // automation to delay
    2.0f       // delay in seconds
);
```

### Automation Action Factory

To simplify the creation of automations with wrappers, `AutomationActionFactory` is available:

```cpp
#include <vclib/render/automation/actions/automation_action_factory.h>

vcl::AutomationActionFactory<BmarkDrawerT> factory;

// Create a rotation limited to 250 frames
auto rotation = factory.createFrameLimited(
    vcl::PerFrameRotationAutomationAction<BmarkDrawerT>::fromFramesPerRotation(250, {0.f, 0.f, 1.f}),
    250
);
```

## Metrics

**Metrics** define what to measure during the benchmark:

### FpsBenchmarkMetric
Measures the average framerate (FPS) of the automation:

```cpp
#include <vclib/render/automation/metrics.h>

viewer.setMetric(vcl::FpsBenchmarkMetric());
```

### TimeBenchmarkMetric
Measures the total execution time:

```cpp
viewer.setMetric(vcl::TimeBenchmarkMetric());
```

### MultipleFpsBenchmarkMetric
Measures the framerate at multiple intervals during the automation:

```cpp
// Divides the automation into 10 intervals and measures the average FPS for each
viewer.setMetric(vcl::MultipleFpsBenchmarkMetric(10));
```

### MultipleTimeBenchmarkMetric
Measures the time at multiple intervals:

```cpp
// Divides the automation into 5 intervals and measures the total time for each
viewer.setMetric(vcl::MultipleTimeBenchmarkMetric(5));
```

### NullBenchmarkMetric
Measures nothing (useful for automations without measurement):

```cpp
viewer.setMetric(vcl::NullBenchmarkMetric());
```

## Printers

**Printers** determine how the results are saved/printed:

### StdoutBenchmarkPrinter
Prints the results to standard output:

```cpp
#include <vclib/render/automation/printers.h>

viewer.setPrinter(vcl::StdoutBenchmarkPrinter());
```

### CsvBenchmarkPrinter
Saves the results to a CSV file (separator: `;`):

```cpp
vcl::CsvBenchmarkPrinter printer("results.csv");
printer.useHeader(true);          // Include header
printer.useDescription(true);      // Include automation description
printer.useUnitOfMeasure(true);   // Include unit of measure
viewer.setPrinter(printer);
```

The CSV file will have this format:
```
Description;Measurement 0;Measurement 1;...
Rotate 0.025 radians per frame around { 0, 0, 1 };60.5fps;61.2fps;...
```

### JsonBenchmarkPrinter
Saves the results in JSON format:

```cpp
vcl::JsonBenchmarkPrinter printer("results.json");
printer.useDescription(true);
printer.useUnitOfMeasure(true);
viewer.setPrinter(printer);
```

The JSON file will have this format:
```json
[
    {
        "Description": "Rotate 0.025 radians per frame around { 0, 0, 1 }",
        "Measurements": [
            "60.5fps",
            "61.2fps",
            ...
        ]
    },
    ...
]
```

### NullBenchmarkPrinter
Prints nothing (useful for testing or debugging):

```cpp
viewer.setPrinter(vcl::NullBenchmarkPrinter());
```

### ObjectBenchmarkPrinter
Saves the results to a thread-safe vector, useful for programmatically accessing results:

```cpp
vcl::ObjectBenchmarkPrinter printer;
auto resultPtr = printer.getResultPtr();
viewer.setPrinter(printer);

// After execution, access the results
auto results = resultPtr->getVectorBlocking();
for (const auto& [description, metric] : results) {
    // Process the results
}
```

## Complete Example

Here is a complete example that runs a rotation benchmark on a mesh:

```cpp
#include <vclib/render/render_app.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/benchmark_viewer_drawer.h>
#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/glfw/window_manager.h>
#include <vclib/io.h>
#include <vclib/algorithms/mesh/update/normal.h>

int main()
{
    // Setup viewer
    using BenchmarkViewer = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::BenchmarkViewerDrawer>;
    
    BenchmarkViewer viewer("Benchmark", 1920, 1080);
    
    // Load mesh
    vcl::TriMesh mesh = vcl::loadMesh<vcl::TriMesh>("model.ply");
    vcl::updatePerVertexAndFaceNormals(mesh);
    
    vcl::DrawableMesh<vcl::TriMesh> drawable(mesh);
    
    auto vec = std::make_shared<vcl::DrawableObjectVector>();
    vec->pushBack(std::move(drawable));
    viewer.setDrawableObjectVector(vec);
    
    // Configure benchmark
    using BmarkDrawerT = vcl::BenchmarkDrawer<BenchmarkViewer>;
    vcl::AutomationActionFactory<BmarkDrawerT> factory;
    
    // Repeat the benchmark 3 times
    viewer.setRepeatTimes(3);
    
    // Use FPS metric
    viewer.setMetric(vcl::FpsBenchmarkMetric());
    
    // Save results to CSV
    vcl::CsvBenchmarkPrinter printer("benchmark_results.csv");
    viewer.setPrinter(printer);
    
    // Add automation: complete rotation in 250 frames around Z
    viewer.addAutomation(factory.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<BmarkDrawerT>::fromFramesPerRotation(250, {0.f, 0.f, 1.f}),
        250
    ));
    
    // Add automation: complete rotation in 250 frames around Y
    viewer.addAutomation(factory.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<BmarkDrawerT>::fromFramesPerRotation(250, {0.f, 1.f, 0.f}),
        250
    ));
    
    // Add automation: complete rotation in 250 frames around X
    viewer.addAutomation(factory.createFrameLimited(
        vcl::PerFrameRotationAutomationAction<BmarkDrawerT>::fromFramesPerRotation(250, {1.f, 0.f, 0.f}),
        250
    ));
    
    // Terminate the program when all automations are finished
    viewer.terminateUponCompletion();
    
    // Start viewer
    viewer.show();
    
    return 0;
}
```

## Example: Benchmark with Different Shader Types

The `980-benchmark` example in the `examples/render/` directory shows how to run comparative benchmarks between different shader splitting types:

- **UBER**: A single uber-shader that handles all cases
- **SPLIT**: Separate shaders for different configurations
- **UBER_WITH_STATIC_IF**: Uber-shader with static branches

The example supports command-line parameters to configure:
- Shading type (smooth/flat)
- Shader splitting type
- Window resolution
- Number of frames and repetitions
- Results output (stdout, CSV/JSON file)

### Using the 980-benchmark Example

```bash
# Run benchmark with flat shading and SPLIT splitting
./vclib-render-example-980-benchmark --flat --split -o results.json model.ply

# Run benchmark with custom resolution and 500 frames per rotation
./vclib-render-example-980-benchmark --res 1920 1920 -f 500 model.ply

# Run benchmark with 5 repetitions
./vclib-render-example-980-benchmark -r 5 model.ply
```

For a complete list of available options, use:
```bash
./vclib-render-example-980-benchmark --help
```

## Python Script for Batch Testing

The example also includes a Python script (`test_battery.py`) to run automated batch tests with various parameter combinations:

```python
# Run a complete test battery
python test_battery.py

# Repeat the tests 3 times
python test_battery.py 3
```

The script automatically tests different combinations of:
- Models (small, medium, large)
- Shading types (smooth, flat)
- Splitting types (uber, split, uber-static-if)
- Coloring types (per-vertex, per-face, texture)
- Resolutions (960x960, 1920x1920)

## Technical Notes

### Thread Safety
`ObjectBenchmarkPrinter` is thread-safe and can be used to access results from separate threads.

### Timer and Precision
The framework uses the `vcl::Timer` class to measure times with high precision. It's important to note that:
- The first frames may have different performance (warm-up)
- The framework introduces an initial delay of ~0.6 seconds to stabilize measurements

### Repetitions
- Repetitions allow you to obtain more reliable measurements
- Use `VCL_BENCHMARK_DRAWER_REPEAT_FOREVER` for infinite repetitions (not recommended with printers other than `NullBenchmarkPrinter`)

### Customization
You can create custom automations, metrics, and printers by extending the base classes:
- `AbstractAutomationAction<BmarkDrawer>` for custom automations
- `BenchmarkMetric` for custom metrics
- `BenchmarkPrinter` for custom printers

## API References

- @ref vcl::BenchmarkDrawer
- @ref vcl::BenchmarkViewerDrawer
- @ref vcl::AbstractAutomationAction
- @ref vcl::BenchmarkMetric
- @ref vcl::BenchmarkPrinter
- @ref vcl::AutomationActionFactory
