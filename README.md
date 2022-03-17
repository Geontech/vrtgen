# VITA 49.2 Packet Generator

Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

This is a work-in-progress and almost assuredly will break repeatedly. A stable 1.0 release will be
available in the next few months. Feedback is welcome.

## Dependencies

`vrtgen` requires Python >= 3.7 as well as `pip` for Python >= 3.7.

To build the C++ header library, CMake 3.17 or later must be installed.

Generated C++ code requires C++20 support.
The only dependency is the C++ header library.

## Installation

Installation via `pip` is recommended, as it will automatically install the required dependencies.
To install via `pip`:

```sh
sudo pip3 install --upgrade pip
pip3 install --user .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

### Backend Installation

To build the C++ library tests and install the header files:

```sh
cmake3 -B build
cd build
make
sudo make install
```

On some Linux systems, CMake 3 is the default; run `cmake` instead of `cmake3`.

The headers are installed to `/usr/local/include/vrtgen` by default.

### Developer Setup

For developers, `pip` supports an editable install mode that links the source code to the Python
`site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
To perform a "developer install" using a virtual environment:

```sh
python3 -m venv venv
. venv/bin/activate
pip install --upgrade pip
pip install -e .
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
