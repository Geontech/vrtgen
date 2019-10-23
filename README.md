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
sudo pip3 install .
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

To generate a hexadecimal dump of the default configuration for each packet defined in the YAML file:

```sh
vrtpktgen bindump <path-to-file.yml>
```

Help is available with the `--help` (or `-h`) option.
To get general help or to check which backends are available:

```sh
vrtpktgen --help
```

To get help on a specific backend, run `vrtpktgen <backend> --help`.
For example, to get help on the `bindump` backend:

```sh
vrtpktgen bindump --help
```

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
