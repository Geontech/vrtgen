# VITA 49.2 Packet Generator

Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

This is a work-in-progress and almost assuredly will break repeatedly. A stable 1.0 release will be
available in the next few months. Feeback is welcome.

## Dependencies

`vrtgen` requires Python 3.6 as well as `pip` for Python 3.6.

## Installation

Installation via `pip` is recommended, as it will automatically install the required dependencies.
To install via `pip`:

```sh
sudo pip3 install --upgrade pip
pip3 install --user .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

### Backend Installation

`vrtgen` uses a plug-in system to install different native-language backends.
Each backend must be installed before being available to the `vrtpktgen` executable.

A C++ backend is available in the `cpp` subdirectory of the source.
For more information, see [cpp/README.md](./cpp/README.md).

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
By default, the `check` and `bindump` backends are installed.

Select an available backend with the first positional argument.
To validate a YAML file, use the `check` backend:

```sh
vrtpktgen check <path-to-file.yml>
```

Invalid packet configurations will be flagged with warnings and errors.

Help is available with the `--help` (or `-h`) option.
To get general help or to check which backends are available:

```sh
vrtpktgen --help
```

## Testing

The included test suite verifies the YAML parser and Python data model.
To run the test suite, from within the base directory:

```sh
pytest
```
