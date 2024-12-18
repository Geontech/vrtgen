# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.7.14] - 2024-11-06
### Added
  - YAML tags and namespace to support DIFI 1.2 deviations to reference level and buffer size context fields
  - Support for packet types in multiple information classes
  - 'any' type for tsm fields
    - Adds additional getter/setter functions for tsm
    - Removes explicit matching on tsm
### Changed
- Make Class ID field `pad_bits` runtime dynamic
  - Do not check value in match function
  - Add public getter/setter functions
- Removed setter for control packet CAM action mode field
### Fixed
- Vrtpktgen bug when no acknowledgement packets are requested
## [0.7.13] - 2024-09-19
### Fixed
- Threshold data model field variable names
- Spectrum data model to include number of bits
- Spectrum packing class function types and
## [0.7.12] - 2024-09-06
### Fixed
- Controller class template not including message_id and socket members for control packets
- PayloadFormat bits updated to 64
## [0.7.11] - 2024-07-31
### Fixed
- Set `not_v49d0` bit to true when `spectrum` bit is true for data packets
## [0.7.10] - 2024-07-03
### Fixed
- `SNRNoise::noise` functions using incorrect subfield
### Removed
- CentOS 7 CI
## [0.7.9] - 2024-07-02
### Added
- Getter/setter functions for `action_mode` and `timing_control` in command packets
## [0.7.8] - 2024-05-09
### Fixed
- Error setter functions in ack packets not setting cam.ack_er appropriately
## [0.7.7] - 2024-02-15
### Fixed
- CIF2 UUID fields (`controllee_uuid` and `controller_uuid`) being generated as `uint32_t`
## [0.7.6] - 2023-10-11
### Added
- 'any' type for timestamp fields
  - Adds additional getter/setter functions for tsi/tsf
  - Removes explicit matching on tsi/tsf
### Changed
- Difi example yamls and generated project
- Generated CMake files use FetchContent
### Fixed
- `reset_cif_2()` declaration not being generated
## [0.7.5] - 2023-05-19
### Added
- Setters/getters for CAM bits SchX and AckP
## [0.7.4] - 2023-04-14
### Added
- DIFI yaml files in examples
- rocky9 to CI pipeline
### Fixed
- Ack packet match function.
  - Moved from base ack class to AckVX and AckS subclasses
- Pack CAM into packet's data buffer during construction when ack_* value is required
## [0.7.3] - 2023-03-31
### Added
- CMake target alias
- pkg-config file install
- CMake integration instructions in README
- Random inbox string generator
### Fixed
- Check for empty message in NATS client
### Changed
- Removed uuid.h dependency from NATS client
## [0.7.2] - 2023-03-21
### Fixed
- udp send_packet updated to use latest packet structure and tuple argument
- Bug in packet match function not incrementing `curr_pos` correctly
- Controllee base class calling the correct send function for udp socket type
## [0.7.1] - 2023-03-13
### Fixed
- Controller nats include path in template
- `req_s` packet check in controllee base template
- AckS typo in controller tempalte
## [0.7.0] - 2023-03-13
### Added
- New `--cmd-socket` option for `vrtpktgen`: 'nats'
  - Enables the use of NATS request-reply for command socket interactions between controller and controllee
- NATS header files to interact with the [nats.c](https://github.com/nats-io/nats.c) library
  - Adds a dependency on this library as well as the external libraries it depends on
- Template code to support new `vrtgen::nats::client` type as the transport
- Example application using NATS
### Fixed
- Destructor bug in packet classes
- Packaging pipeline for centos 7
## [0.6.3] - 2023-03-08
### Changed
- Version => 0.6.3
- Updated Changelog
## [0.6.2] - 2023-03-02
### Fixed
- Template bug for reset_cif functions
- Template bug for query ack enable getter function
## [0.6.1] - 2023-03-01
### Added
- CMake configuration file install
### Fixed
- Pad data payloads to be on word boundaries
- Example project controllee
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
