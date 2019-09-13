# VITA 49.2 Packet Generator

Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

This is a work-in-progress and almost assuredly will break repeatedly.

## Dependencies

`vrtgen` requires Python 3.6 or newer with the following packages:

* PyYAML (3.11 or newer)
* Jinja2 (2.8 or newer)

For developers, the following packages are recommended:

* pytest
* pylint

## Installation

Installation via `pip` is recommended, as it will automatically install the required dependencies.
To install via `pip`:

```sh
pip3 install .
```

_Note_: Installing to the default system location may require superuser privileges.

## Usage

The package installs a `vrtpktgen` executable in your path.
By default, `vrtpktgen` only validates a YAML packet definition file.

```sh
vrtpktgen <path-to-file.yml>
```

The `vrtpktgen` program supports backend plug-ins to target different output types.
Select an available backend with the `-b <backend>` flag.
The `bindump` backend is included with the default install.
For example:

```sh
vrtpktgen -b bindump <path-to-file.yml>
```

will give a hexadecimal dump of the default configuration for each packet defined in the YAML file.

A C++ backend is available in the `cpp` subdirectory of the source.
For more information, see [cpp/README.md](./cpp/README.md).

## Developer Setup

For developers, `pip` supports an editable install mode that links the source code to the Python `site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
To perform a "developer install" using a virtual environment:

```sh
python3 -m venv venv
. venv/bin/activate
pip install -e .
```

This will install both the `vrtgen` Python modules and the `vrtpktgen` executable.

## Testing

The included test suite verifies the YAML parser and Python data model.
To run the test suite, from within the base directory:

```sh
pytest
```
