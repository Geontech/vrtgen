VITA 49.2 Packet Generator
==========================
Generates native-language classes/structs for V49.2 packet definitons in a YAML configuration file.

This is a work-in-progress and almost assuredly will break repeatedly.

# Dependencies
`vrtgen` requires Python 3.6 or newer with the following packages:
* PyYAML (3.11 or newer)
* Jinja2 (2.8 or newer)

For developers, the following packages are recommended:
* pytest
* pylint

# Developer Setup
To perform a "developer install":
```
pip3 install -e .
```
This will create a link to your project in the Python `site-packages` directory.
It is strongly recommended to use a `venv` environment or Docker image for development.
