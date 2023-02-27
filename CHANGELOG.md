# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [unreleased]
### Fixed
- Pad data payloads to be on word boundaries
## [0.6.0] - 2023-02-20
### Added
- User-defined stream id and trailer classes via YAML
- Discrete IO user-defined fields
- Doxygen support
- Increased test coverage driven by VITA 49.2-2017 specification rules
### Changed
- **The schema for YAML files has changed**.
  Individual trailer fields have been removed.
  - Only user-defined bits are settable in YAML.
- **The packet class structure has changed**.
  Generated packet classes no longer use the `helper` construct.
  - The packet helper `pack` function was replaced with internal data management and the `data` function.
  - The packet helper `unpack` function was replaced with a new constructor that unpacks from a `std::span` of bytes.
  - Packet members that are a class type (e.g. `vrtgen::packing::Gain`) can be updated with a reference getter function.
    The internal data however will not be updated until the `data` function is called which will sync the internal
    management of these classes.
## [0.5.2] - 2022-06-07
#### Fixed
- Template for creating a UDP command socket.
## [0.5.1] - 2022-05-31
#### Fixed
- Setting number of bits for `PayloadFormat` model class
- `version_build_code` field type
- Setting number of bits `ControlIdentifier` model class
## [0.5.0] - 2022-03-17
### Added
- `cpp` backend as default installation
### Changed
- **The schema for YAML files has changed**.
  Mappings are now marked with specific YAML tags for ingest by the new loader.
- Migrated the backend parser model classes to Python `dataclass`.
- Changed parser loader to utilize built-in `SafeLoader` from PyYAML
- Removed generation of backend library. Now available as source in `include/vrtgen`
- Updated templates.
## [0.4.0] - 2021-06-03
### Added
- Support for including other YAML files containing packet definitions
- Support for Information Class YAML configurations
- Generation of Controllee and Controller classes from Information Class specification
- Socket classes to libvrtgen
- Example packets and demo application
### Changed
- **The schema for YAML files has changed**.
  All mappings in YAML configurations are now lowercase separated with dashes. See doc/yaml-guide.md for details.
  **Existing YAML files must be updated to work with version 0.4.**
## [0.3.1] - 2020-04-20
### Changed
- Fixed copy and move semantics for C++ `vrtgen::optional<T>` class.
- Minor whitespace fixes in generated C++ code.

## [0.3.0] - 2020-04-17
### Changed
- **The schema for YAML files has changed**.
  Instead of requiring a top-level "type" key, each packet definition must contain a single-key mapping with the packet type and a mapping of its fields.
  The "payload" key in context and command packets has also been removed; all packet fields should be under the type mapping.
  **Existing YAML files must be updated to work with version 0.3.**
### Added
- User-defined Discrete I/O fields, including indicators.
- Python struct classes support unpacking from bytes.
- Data packets may specificy spectrum mode, context and data may specify "Not a V49.0 packet" flag.
### Removed
- The packet type "command" was removed.

## [0.2.1] - 2019-11-13
### Fixed
- Using struct fields in packets works again in C++.

## [0.2.0] - 2019-10-23
### Added
- Support for defining Control/Acknowledge packets.
- C++ code generation of Control/Acknowledge packets.
### Changed
- Command line syntax of `vrtpktgen` changed to require generator name as a positional argument.
- Fixed a bug in replacing existing values in packed binary fields.
- Addressed warnings in generated C++ code.
### Deprecated
- The packet type "command" is deprecated.

## [0.1.2] - 2019-09-23
### Changed
- Support CentOS 7 `setuptools` RPM for installation.
- C++ build does not depend on installed location of `vrtpktgen`.

## [0.1.1] - 2019-09-16
### Changed
- Updated source headers to reference LGPL.

## [0.1.0] - 2019-09-13
### Added
- Python data model for describing VITA 49.2 packets.
- YAML parser for packet defintions.
- Basic support for common CIF0/CIF1 fields.
- Initial C++ code generation backend.
