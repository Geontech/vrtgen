# VITA 49.2 Packet Generator

Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

## Dependencies

`vrtgen` requires Python >= 3.7 as well as `pip` for Python >= 3.7.

To build the C++ header library, CMake 3.15 or later must be installed.

Generated C++ code requires g++ >= 10 for C++20 support, including concepts.
The only dependency is the C++ header library.
See the testing section.

If using the NATS command socket type option with `vrtpktgen`, the following library is also
required:

- [nats.c](https://github.com/nats-io/nats.c)

## Installation

Installation via `pip` is recommended, as it will automatically install the required dependencies.
To install via `pip`:

```sh
sudo pip3 install --upgrade pip
pip3 install --user .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

### Backend Installation

The `vrtgen` backend library is header only and can be included directly into your build tree
by adding the contents of the [include](include) folder to your project's build structure.

To install the C++ library header files on your system:

```sh
cmake3 -B build
sudo cmake3 --build build --target install
```

On some Linux systems, CMake 3 is the default; run `cmake` instead of `cmake3`.

The headers are installed to `/usr/local/include/vrtgen` by default.

### CMake Integration

**External**

To use the library from a CMake project, you can use `find_package()` to discover and use the
namespaced target from the package configuration:

```cmake
# CMakeLists.txt
find_package(vrtgen REQUIRED)
...
add_executable(foo foo.cpp)
...
target_link_libraries(foo PRIVATE vrtgen::vrtgen)
```

**FetchContent**

Using CMake's FetchContent allows this library to be downloaded and used as a dependency at
configuration time, therefore avoiding the need to install it on your system:

```cmake
# CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
    vrtgen
    GIT_REPOSITORY https://github.com/geontech/vrtgen
    GIT_TAG v0.7.6
)
FetchContent_MakeAvailable(vrtgen)
...
add_executable(foo foo.cpp)
...
target_link_libraries(foo PRIVATE vrtgen::vrtgen)
```

### Developer Setup

For developers, `pip` supports an editable install mode that links the source code to the Python
`site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
To perform a "developer install" using a virtual environment:

```sh
python3 -m venv venv
. venv/bin/activate
python -m pip install --upgrade pip
python -m pip install -e .
```

## Usage

This package installs a `vrtpktgen` executable in your path.
The `vrtpktgen` program supports backend plug-ins to target different output types.
By default, the `cpp` backend is installed.

Select an available backend with the first positional argument.

```
vrtpktgen cpp <filename>.yaml
```

Help is available with the `--help` (or `-h`) option.
To get general help or to check which backends are available:

```sh
vrtpktgen --help
```

## Testing

To build and execute the tests:

```sh
cmake3 -B build -DVRTGEN_BUILD_TESTS=ON
cmake3 --build build --target check
```

To run the Python parser tests:

```sh
pytest
```
