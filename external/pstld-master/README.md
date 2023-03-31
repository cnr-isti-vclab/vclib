![Build Status](https://github.com/mikekazakov/pstld/actions/workflows/build.yml/badge.svg)

# pstld
Experimental implementation of ParallelSTL on top of Grand Central Dispatch aka libdispatch

The purpose of this library is to provide a drop-in implementation of C++ parallel algorithms for the Apple platforms.
Xcode comes with no parallel algorithms in libc++, so this library aims to fill the gap.
pstld uses the native scheduler (libdispatch) and does not depend on any 3rd-party libraries.

## Usage

The simplest way to use the library is to consume it as header-only and to request exposing the parallel algorithms in the namespace ```std```:
```C++
#include <cstdlib>
#include <iostream>
#define PSTLD_HEADER_ONLY   // no prebuilt library, only the header
#define PSTLD_HACK_INTO_STD // export into namespace std
#include "pstld.h"
int main()
{
    std::vector<unsigned> v(100'000'000);
    std::generate(v.begin(), v.end(), [] { return std::rand(); });
    std::cout << std::reduce(std::execution::par, v.begin(), v.end()) << std::endl;
}
```
The default Xcode toolchain can now build it:
```Shell
% clang++ -std=c++17 main.cpp -o test && ./test
64818392
```
The same functions can be used directly from the ```pstld``` namespace without specifying the execution policy:
```C++
#include <cstdlib>
#include <iostream>
#define PSTLD_HEADER_ONLY   // no prebuilt library, only the header
#include "pstld.h"
int main()
{
    std::vector<unsigned> v(100'000'000);
    std::generate(v.begin(), v.end(), [] { return std::rand(); });
    std::cout << pstld::reduce(v.begin(), v.end()) << std::endl;
}
```
pstld can be precompiled via CMake or manually by adding ```pstld.cpp``` into the build sources:
```C++
#include <cstdlib>
#include <iostream>
#include "pstld.h"
int main()
{
    std::vector<unsigned> v(100'000'000);
    std::generate(v.begin(), v.end(), [] { return std::rand(); });
    std::cout << pstld::reduce(v.begin(), v.end()) << std::endl;
}
```
```Shell
% clang++ -std=c++17 main.cpp pstld.cpp -o test && ./test
64818392
```

## Completeness

The library is not complete, this table shows which algorithms are currently available:

§ | Function | Exposed | Parallel
:--- |:--- |:---:|:---:
25.6.1 | std::all_of | ✅ | ✅
25.6.2 | std::any_of | ✅ | ✅
25.6.3 | std::none_of | ✅ | ✅
25.6.4 | std::for_each | ✅ | ✅
| | std::for_each_n | ✅ | ✅
25.6.5 | std::find | ✅ | ✅
| | std::find_if | ✅ | ✅
| | std::find_if_not | ✅ | ✅
25.6.6 | std::find_end | ✅ | ✅
25.6.7 | std::find_first_of | ✅ | ✅
25.6.8 | std::adjacent_find | ✅ | ✅
25.6.9 | std::count | ✅ | ✅
| | std::count_if | ✅ | ✅
25.6.10 | std::mismatch | ✅ | ✅
25.6.11 | std::equal | ✅ | ✅
25.6.13 | std::search | ✅ | ✅
| | std::search_n | ✅ | ✅
25.7.1 | std::copy | ✅ | ✅
| | std::copy_n | ✅ | ✅
| | std::copy_if | ✅ | ❌
25.7.2 | std::move | ✅ | ✅
25.7.3 | std::swap_ranges | ✅ | ✅
25.7.4 | std::transform | ✅ | ✅
25.7.5 | std::replace | ✅ | ✅
| | std::replace_if | ✅ | ✅
| | std::replace_copy | ✅ | ❌
| | std::replace_copy_if | ✅ | ❌
25.7.6 | std::fill | ✅ | ✅
| | std::fill_n | ✅ | ✅
25.7.7 | std::generate | ✅ | ✅
| | std::generate_n | ✅ | ✅
25.7.8 | std::remove | ❌ | ❌
| | std::remove_if | ❌ | ❌
| | std::remove_copy | ❌ | ❌
| | std::remove_copy_if | ❌ | ❌
25.7.9 | std::unique | ❌ | ❌
| | std::unique_copy | ❌ | ❌
25.7.10 | std::reverse | ✅ | ✅
| | std::reverse_copy | ❌ | ❌
25.7.11 | std::rotate | ❌ | ❌
25.7.14 | std::shift_left | ❌ | ❌
| | std::shift_right | ❌ | ❌
25.8.2.1 | std::sort | ✅ | ✅
25.8.2.2 | std::stable_sort | ✅ | ✅
25.8.2.3 | std::partial_sort | ❌ | ❌
25.8.2.4 | std::partial_sort_copy | ❌ | ❌
25.8.2.5 | std::is_sorted | ✅ | ✅
| | std::is_sorted_until | ✅ | ✅
25.8.3 | std::nth_element | ❌ | ❌
25.8.5 | std::is_partitioned | ✅ | ✅
| | std::partition | ❌ | ❌
| | std::stable_partition | ❌ | ❌
| | std::partition_copy | ❌ | ❌
25.8.6 | std::merge | ✅ | ✅
| | std::inplace_merge | ❌ | ❌
25.8.7.2 | std::includes | ❌ | ❌
25.8.7.3 | std::set_union | ❌ | ❌
25.8.7.4 | std::set_intersection | ❌ | ❌
25.8.7.5 | std::set_difference | ❌ | ❌
25.8.7.6 | std::set_symmetric_difference | ❌ | ❌
25.8.8.6 | std::is_heap | ❌ | ❌
| | std::is_heap_until | ❌ | ❌
25.8.9 | std::min_element | ✅ | ✅
| | std::max_element | ✅ | ✅
| | std::minmax_element | ✅ | ✅
25.8.11 | std::lexicographical_compare | ✅ | ✅
25.10.4 | std::reduce | ✅ | ✅
25.10.6 | std::transform_reduce | ✅ | ✅
25.10.8 | std::exclusive_scan | ✅ | ✅
25.10.9 | std::inclusive_scan | ✅ | ✅
25.10.10 | std::transform_exclusive_scan | ✅ | ✅
25.10.11 | std::transform_inclusive_scan | ✅ | ✅
25.10.12 | std::adjacent_difference | ✅ | ✅
25.11.3 | std::uninitialized_default_construct | ✅ | ✅
| | std::uninitialized_default_construct_n | ✅ | ✅
25.11.4 | std::uninitialized_value_construct | ✅ | ✅
| | std::uninitialized_value_construct_n | ✅ | ✅
25.11.5 | std::uninitialized_copy | ✅ | ✅
| | std::uninitialized_copy_n | ✅ | ✅
25.11.6 | std::uninitialized_move | ✅ | ✅
| | std::uninitialized_move_n | ✅ | ✅
25.11.7 | std::uninitialized_fill | ✅ | ✅
| | std::uninitialized_fill_n | ✅ | ✅
25.11.9 | std::destroy | ✅ | ✅
| | std::destroy_n | ✅ | ✅

## Parallel speedup

```benchmark.cpp``` contains a set of primitive synthetic performance tests.
Each row shows how the parallel implementation compares to its serial counterpart depending on the number of elements in a working set.
The serial variant executes the default algorithms from libc++ and the parallel one runs the pstld implementation.
Per-element operations are mostly trivial in these benchmarks, so the speed-up numbers represent a somewhat worst-case scenario.

<details>
  <summary>2020 MacBook Pro 13" (Apple M1 CPU, 4P+4E cores), macOS 13, Xcode 14</summary>

```
                                 1K   10K  100K    1M   10M  100M 
all_of                         0.36  0.41  1.41  2.67  2.16  3.46 
any_of                         0.52  0.41  0.97  1.83  2.24  3.50 
none_of                        0.40  0.43  0.86  1.84  2.10  3.11 
for_each                       0.14  0.75  0.55  0.82  0.99  1.74 
find                           0.21  0.71  0.80  1.75  2.11  3.16 
find_end                       0.12  0.14  0.23  0.59  0.67  1.02 
find_first_of                  0.46  0.97  1.71  4.33  4.90  4.95 
adjacent_find                  0.46  0.50  0.64  1.59  2.03  2.23 
count                          0.16  0.64  0.93  0.78  0.93  2.24 
mismatch                       0.29  0.67  0.67  1.12  1.13  1.17 
equal                          0.48  0.78  0.75  1.21  1.24  1.29 
search                         0.54  0.69  1.39  3.25  3.79  3.98 
copy                           0.28  0.65  0.50  0.82  0.97  1.80 
move                           0.53  0.43  1.12  1.10  1.72  2.18 
swap_ranges                    0.32  0.66  0.57  0.64  0.97  1.51 
transform                      0.29  0.85  0.40  0.72  0.83  1.68 
replace                        0.16  0.31  0.32  1.02  0.98  1.73 
fill                           0.09  0.78  0.70  0.76  0.72  1.29 
reverse                        0.29  0.40  0.58  1.31  1.02  0.98 
sort_Rnd                       0.14  1.64  2.95  3.72  4.32  4.31 
sort_Eq                        0.07  0.42  0.59  0.94  0.98  0.94 
sort_Asc                       0.08  0.10  0.33  0.39  0.43  0.39 
sort_Des                       0.07  0.49  0.62  0.94  0.98  0.96 
stable_sort                    0.71  1.77  3.08  3.77  4.00  4.09 
is_sorted                      0.28  0.31  0.96  1.87  2.10  3.19 
is_partitioned                 0.24  0.68  0.95  1.84  2.10  3.05 
merge                          1.00  0.52  0.76  1.24  1.96  2.26 
minmax_element                 1.17  1.04  2.67  5.68  7.22  6.84 
lexicographical_compare        0.65  1.02  1.30  2.07  2.14  2.22 
reduce                         1.13  2.01  1.56  4.28  5.57  5.25 
transform_reduce               0.99  1.33  1.28  4.39  5.56  5.40 
exclusive_scan                 0.60  0.47  0.91  2.34  2.24  2.47 
inclusive_scan                 0.49  0.63  0.83  2.24  2.24  2.45 
transform_exclusive_scan       0.45  0.48  1.06  2.30  2.29  2.33 
transform_inclusive_scan       0.37  0.49  1.70  2.26  2.32  2.36 
adjacent_difference            0.19  0.79  0.49  0.70  0.79  0.78 
uninitialized_value_construct  0.26  0.24  0.40  0.97  1.86  1.89 
uninitialized_copy             0.30  0.59  1.86  1.37  2.05  2.54 
uninitialized_move             0.56  0.48  1.13  1.25  1.06  2.22 
uninitialized_fill             0.76  0.93  3.21  4.94  5.21  5.11 
destroy                        0.33  0.81  1.68  1.90  1.93  4.13 
```
</details>

<details>
  <summary>2018 Mac Mini (Intel 8500B CPU, 6 cores), macOS 13, Xcode 14</summary>

```
                                 1K   10K  100K    1M   10M  100M 
all_of                         0.26  0.64  3.28  3.00  3.24  4.10 
any_of                         0.50  0.67  0.93  2.66  3.07  3.60 
none_of                        0.36  0.63  0.61  2.06  2.30  2.85 
for_each                       0.17  0.47  0.47  0.97  0.99  1.22 
find                           0.25  0.48  0.89  2.34  2.43  3.10 
find_end                       0.09  0.24  0.42  1.01  1.16  1.36 
find_first_of                  0.58  1.26  2.80  3.99  4.82  5.09 
adjacent_find                  0.41  0.61  1.16  2.70  2.39  2.58 
count                          0.12  0.40  0.47  1.41  1.64  2.32 
mismatch                       0.36  0.27  0.66  1.55  1.60  1.72 
equal                          0.22  0.90  3.28  2.29  2.29  2.33 
search                         0.36  1.18  1.70  3.48  3.88  3.93 
copy                           0.12  0.33  0.67  0.98  1.13  1.68 
move                           0.20  0.43  1.29  0.89  1.11  4.15 
swap_ranges                    0.27  0.23  0.52  0.80  0.89  1.09 
transform                      0.22  0.41  0.62  0.88  1.03  1.49 
replace                        0.40  0.71  0.81  1.56  1.44  1.66 
fill                           0.14  0.49  0.87  1.18  0.97  1.13 
reverse                        0.31  0.49  0.90  1.88  1.00  0.96 
sort_Rnd                       0.28  1.92  3.16  3.44  3.76  3.66 
sort_Eq                        0.04  0.33  1.36  2.18  2.27  2.07 
sort_Asc                       0.02  0.07  0.30  0.41  0.33  0.29 
sort_Des                       0.04  0.33  1.47  2.12  1.53  2.30 
stable_sort                    0.48  1.66  3.25  3.71  3.75  3.63 
is_sorted                      0.40  0.51  0.68  2.15  2.43  2.80 
is_partitioned                 0.13  0.53  0.43  1.89  2.18  2.58 
merge                          1.00  0.45  1.15  1.12  1.23  1.28 
minmax_element                 0.37  0.59  0.83  2.54  3.02  3.45 
lexicographical_compare        0.65  0.55  1.09  2.07  2.08  2.24 
reduce                         0.51  0.81  1.08  3.12  3.93  4.21 
transform_reduce               0.50  0.87  1.47  3.61  4.06  4.47 
exclusive_scan                 0.16  0.51  0.74  0.97  1.05  1.31 
inclusive_scan                 0.17  0.28  0.82  1.01  1.06  1.33 
transform_exclusive_scan       0.43  0.94  2.17  2.76  2.71  2.80 
transform_inclusive_scan       0.35  1.04  2.29  2.87  2.79  2.85 
adjacent_difference            0.04  0.12  1.10  0.92  1.04  1.07 
uninitialized_value_construct  0.03  0.29  0.54  0.81  1.28  1.35 
uninitialized_copy             0.27  0.53  1.85  1.13  1.43  1.93 
uninitialized_move             0.29  0.68  2.01  0.89  0.91  1.08 
uninitialized_fill             0.33  0.59  3.08  3.16  2.95  3.10 
destroy                        0.42  1.25  2.49  2.72  2.76  2.86
```
</details>

<details>
  <summary>2012 MacBook Pro 13" (Intel 3520M CPU, 2 cores), macOS 10.15, Xcode 12</summary>

```
                                 1K   10K  100K    1M   10M  100M 
all_of                         0.32  0.47  1.28  1.23  1.83  2.04 
any_of                         0.43  1.01  1.70  1.96  2.46  2.47 
none_of                        0.09  0.71  1.69  1.99  2.58  3.29 
for_each                       0.18  0.41  1.46  1.34  1.31  1.44 
find                           0.31  0.34  1.37  1.58  2.81  3.14 
find_end                       0.17  0.29  0.39  0.55  0.57  0.67 
find_first_of                  0.60  1.01  0.87  2.48  3.53  3.53 
adjacent_find                  0.06  1.45  0.93  1.53  2.68  3.47 
count                          0.02  0.27  1.16  1.27  1.53  1.80 
mismatch                       0.42  0.78  1.65  1.83  1.67  2.21 
equal                          0.34  0.63  2.38  1.81  2.30  2.46 
search                         0.83  0.68  1.67  2.55  3.60  3.63 
copy                           0.42  0.91  1.05  1.26  1.21  1.42 
move                           0.66  0.84  1.31  1.21  1.27  1.46 
swap_ranges                    0.38  0.70  0.79  1.23  1.25  1.40 
transform                      0.03  0.27  1.11  1.28  1.35  1.71 
replace                        0.59  0.82  0.59  1.42  1.85  1.76 
fill                           0.41  0.77  0.95  1.05  1.28  1.40 
reverse                        0.43  0.62  1.81  1.42  1.32  1.22 
sort_Rnd                       0.04  0.06  0.23  0.97  1.57  1.63 
sort_Eq                        0.11  0.02  0.07  0.45  1.46  1.91 
sort_Asc                       0.06  0.24  0.21  0.20  0.16  0.13 
sort_Des                       0.09  0.49  1.44  1.77  1.55  1.90 
stable_sort                    0.65  1.88  1.64  1.90  1.72  1.71 
is_sorted                      0.32  0.53  1.01  1.25  1.93  1.95 
is_partitioned                 0.25  0.47  1.14  1.21  1.64  1.93 
merge                          0.99  0.56  1.24  1.08  1.07  1.18 
minmax_element                 0.28  0.56  0.98  1.54  1.77  1.95 
lexicographical_compare        0.79  0.80  1.16  1.51  1.62  1.64 
reduce                         0.59  0.70  1.47  1.49  2.14  2.47 
transform_reduce               0.61  0.49  1.85  1.56  2.13  2.43 
exclusive_scan                 0.28  0.37  1.28  0.84  0.96  1.03 
inclusive_scan                 0.07  0.38  0.79  0.76  0.91  1.02 
transform_exclusive_scan       0.56  0.85  0.91  0.99  1.14  1.13 
transform_inclusive_scan       0.50  0.92  1.03  1.12  1.12  1.16 
adjacent_difference            0.03  0.14  1.67  0.97  1.04  1.05 
uninitialized_value_construct  0.03  0.46  0.66  0.82  0.80  1.03 
uninitialized_copy             0.60  0.74  1.70  1.60  1.68  1.89 
uninitialized_move             0.19  1.22  1.30  1.07  1.19  1.29 
uninitialized_fill             0.52  1.47  2.11  2.12  2.48  2.48 
destroy                        0.23  0.25  0.31  0.48  0.55  0.73 
```
</details>

<details>
  <summary>iPhone 11 Pro (Apple A13 CPU, 2P+4E cores), iOS 16, Xcode 14</summary>

```
                                 1K   10K  100K    1M   10M   30M 
all_of                         0.32  0.64  1.06  1.99  1.85  1.95 
any_of                         0.46  0.26  0.95  2.02  1.90  2.06 
none_of                        0.40  0.43  0.72  1.69  1.77  1.80 
for_each                       0.40  0.51  0.87  1.73  1.18  1.38 
find                           0.03  0.15  0.79  1.86  1.71  1.78 
find_end                       0.12  0.10  0.20  0.37  0.49  0.61 
find_first_of                  0.65  0.73  1.43  2.62  2.81  2.85 
adjacent_find                  0.39  0.40  0.43  1.21  1.54  1.56 
count                          0.34  0.32  0.84  1.72  1.71  1.82 
mismatch                       0.36  0.39  0.61  1.07  1.05  1.02 
equal                          0.44  0.43  1.08  1.48  1.16  1.13 
search                         0.39  0.49  1.15  1.95  2.22  2.22 
copy                           0.02  0.08  0.57  0.66  1.21  1.39 
move                           0.22  0.58  0.91  0.98  1.22  1.13 
swap_ranges                    0.40  0.61  0.80  1.71  1.05  1.21 
transform                      0.34  0.33  0.84  1.01  1.11  1.28 
replace                        0.39  0.51  0.48  1.28  1.05  1.26 
fill                           0.13  0.20  0.29  0.60  0.46  0.54 
reverse                        0.26  0.27  0.69  1.43  1.04  1.02 
sort_Rnd                       0.12  1.40  2.51  2.92  2.86  2.70 
sort_Eq                        0.04  0.24  0.57  0.91  1.18  1.08 
sort_Asc                       0.05  0.09  0.21  0.24  0.19  0.18 
sort_Des                       0.04  0.24  0.58  0.93  1.25  1.09 
stable_sort                    0.48  1.57  2.67  2.86  2.78  2.62 
is_sorted                      0.31  0.42  0.91  2.08  1.84  1.81 
is_partitioned                 0.06  0.45  1.02  1.98  1.76  1.77 
merge                          1.00  0.53  1.29  1.75  1.53  1.57 
minmax_element                 1.20  1.00  2.27  4.62  5.54  5.44 
lexicographical_compare        0.47  0.64  1.11  2.16  1.69  1.66 
reduce                         0.82  0.68  1.79  3.48  3.80  3.87 
transform_reduce               0.67  0.92  1.69  3.47  3.80  3.81 
exclusive_scan                 0.50  0.54  0.93  1.87  1.44  1.50 
inclusive_scan                 0.53  0.42  0.97  1.85  1.44  1.49 
transform_exclusive_scan       0.18  0.72  1.35  1.45  1.20  1.15 
transform_inclusive_scan       0.33  0.90  1.62  1.67  1.23  1.13 
adjacent_difference            0.29  0.37  0.83  0.93  1.00  0.94 
uninitialized_value_construct  0.03  0.12  0.43  0.99  1.15  1.35 
uninitialized_copy             0.34  1.10  1.51  1.04  1.30  1.69 
uninitialized_move             0.23  0.87  1.17  1.00  1.09  1.48 
uninitialized_fill             0.80  1.76  2.33  3.05  2.92  2.83 
destroy                        0.51  0.89  1.85  1.99  0.29  0.26 
```
</details>


## Running the benchmarks

Run the following commands to execute the benchmarks locally:

```Shell
git clone https://github.com/mikekazakov/pstld.git && \
  cd pstld && \
  cd test-llvm-pstl && ./bootstrap.sh && cd - && \
  cd test-msvc-stl && ./bootstrap.sh && cd - && \
  cmake -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release . && \
  make benchmark &&
  ./benchmark/benchmark
```