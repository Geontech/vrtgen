# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
