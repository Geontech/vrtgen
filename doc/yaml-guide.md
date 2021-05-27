# Creating YAML Files

All packet and information class specifications for vrtgen are developed using YAML. It is designed
to be a human-readable implementation of a specific VRT-supported class configuration. The intent
of this guide is to provide a reference when creating YAML file specifications. The following
sections describe the supported YAML mappings and tags for packets and information classes.

## 1. Packets

### 1.1 Packet Name

Every packet specification begins at the top level by specifying the desired name of the packet
class to be generated.

```
ExamplePacket:
```

### 1.2 Packet Type

The next required mapping item to be added to the YAML is the desired packet type. vrtgen currently
supports the following packet types:

| YAML Mapping | Meaning |
| ------------ | ------- |
| `data:` | VRT Signal Data Packet |
| `context:` | VRT Context Packet |
| `control:` | VRT Command Packet |

#### Signal Data Example

```
ExamplePacket:
  data:
```

#### Context Example

```
ExamplePacket:
  context:
```

#### Command Example

```
ExamplePacket:
  control:
```

### 1.3 Stream Identifier

To specify whether or not a Stream Identifier is used for this packet, the following mapping can be
added:

| YAML Mapping | Meaning |
| ------------ | ------- |
| `stream-id : required` | Stream Identifier is always present in packet |

> *Note: Stream Identifier is a required field for all packet types except Data packets and will be
ignored for those packet types.*

#### Signal Data with Stream Identifier Example

```
ExamplePacket:
  data:
    stream-id: required
```

### 1.4 Class Identifier

To specify whether or not a Class Identifier is used for this packet, the following mapping can be
added:

| YAML Mapping | Meaning |
| ------------ | ------- |
| `class-id : required` | Class Identifier is always present in packet |

#### Signal Data with Class Identifier Example

```
ExamplePacket:
  data:
    stream-id: required
    class-id: required
```

### 1.5 Timestamp Information

The VRT timestamp codes are broken down into separate integer and fractional parts. These
individual parts get mapped into a larger section with the key `timestamp:`. 

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `timestamp:` | `integer` | Specify integer-seconds timestamp type (see below) |
|              | `fractional` | Specify fractional-seconds timestamp type (see below) |

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `integer:`   | `none` | No Integer-seconds Timestamp field included |
|              | `utc` | UTC | 
|              | `gps` | GPS time |
|              | `other` | Other |

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `fractional:` | `none` | No Integer-seconds Timestamp field included |
|               | `samples` | Sample Count |
|               | `picoseconds` | Real-Time (Picoseconds) Timestamp |
|               | `free-running` | Free Running Count Timestamp |

#### UTC Real-Time Timestamp Example

```
ExamplePacket:
  data:
    stream-id: required
    timestamp:
      integer: utc
      fractional: picoseconds
```

### 1.6 Packet-Specific Prologue Fields

Additional prologue fields are available for Command packet types. These include the
mandatory Control-Acknowledge Mode and Message Identifier fields, as well as the optional
Controllee and Controller Identifier fields. Because the Message Identifier is always required
there is no YAML mapping required to include this field.

#### 1.6.1 Control-Acknowledge Mode Field

The Control-Acknowledge (CAM) field is made up of several sections of related bits that specify
certain conditions for the outgoing control packet. To that end, there are specific sections
that can be defined in YAML to configure these bits in the CAM.

#### 1.6.1.1 Partial, Warnings, Errors

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `partial:`   | `true`  | Partial packet implementation permitted |
|              | `false` | Partial packet implementation not permitted |
| `warnings:`  | `true`  | Permit execution of fields that generate warnings |
|              | `false` | Do not permit execution of fields that generate warnings |
| `errors:`    | `true`  | Permit execution of fields that generate errors |
|              | `false` | Do not permit execution of fields that generate errors |

```
ExamplePacket:
  control:
    permit:
      partial: true
      warnings: true
      errors: false
```

#### 1.6.1.2 Action Mode

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `action:`    | `none` | No-Action mode |
|              | `dry-run` | Dry-Run mode |
|              | `execute` | Execute mode |

```
ExamplePacket:
  control:
    action: execute
```

#### 1.6.1.3 Acknowledge

To specify the type of response expected from a control packet, the following mappings are
available to set the relevant bits in the CAM:

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `acknowledge:` | `negative` | Specify NACK |
|                | `execution` | Specify execution acknowledgement |
|                | `query-state` | Specify query-state acknowledgement |
|                | `warnings` | Request warnings in response |
|                | `errors` | Request errors in response |

Each of the acknowledge options have the following options available to them:

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `negative:` | `true` | NACK bit will be set to 1 |
|             | `false` | NACK bit will be set to 0 |
| `execution:` | `true` | Req-X bit will be set to 1 |
|              | `false` | Req-X bit will be set to 0 |
| `query-state:` | `true` | Req-S bit will be set to 1 |
|                | `false` | Req-S bit will be set to 0 |
| `warnings:` | `true` | Req-W bit will be set to 1 |
|             | `false` | Req-W bit will be set to 0 |
| `errors:` | `true` | Req-E bit will be set to 1 |
|           | `false` | Req-E bit will be set to 0 |

```
ExamplePacket:
  control:
    acknowledge:
      negative: false
      execution: true
      warnings: true
      errors: true
```

#### 1.6.2 Controllee/Controller Identifier

The controllee and controller identifier fields require that when in use, the packet shall have
either the 32-bit ID or 128-bit UUID version, but not both to specify which format the packet will
use. The following mappings can be added to define these formats:

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `controllee:` | `word` | 32-bit Controllee ID format |
|               | `uuid` | 128-bit Controllee UUID format |
| `controller:` | `word` | 32-bit Controller ID format |
|               | `uuid` | 128-bit Controller UUID format |

##### Controllee/Controller Example

```
ExamplePacket:
  control:
    controllee: word
    controller: word
```

### 1.7 Payload

#### 1.7.1 Context/Control Indicator Field (CIF) 0

The Context/Control Indicator Field sections can be added to Context and Command packet types to
describe the CIF enable words and the associated field values that can be set in the resulting
packets. The following table provides the options available to the `cif0:` section.

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `cif0:` | `change-indicator` | Context Field Change Indicator |
|         | `reference-point-id` | Reference Point Identifier |
|         | `bandwidth` | Bandwidth |
|         | `if-reference-frequency` | IF Reference Frequency |
|         | `rf-reference-frequency` | RF Reference Frequency |
|         | `rf-reference-frequency-offset` | RF Reference Frequency Offset |
|         | `if-band-offset` | IF Band Offset |
|         | `reference-level` | Reference Level |
|         | `gain` | Gain |
|         | `over-range-count` | Over-range Count |
|         | `sample-rate` | Sample Rate |
|         | `timestamp-adjustment` | Timestamp Adjustment |
|         | `timestamp-calibration-time` | Timestamp Calibration Time |
|         | `temperature` | Temperature |
|         | `device-id` | Device Identifier |
|         | `data-payload-format` | Data Packet Payload Format |
|         | `formatted-gps` | Formatted GPS |
|         | `formatted-ins` | Formatted INS |
|         | `ecef-ephemeris` | ECEF Ephemeris |
|         | `relative-ephemeris` | Relative Ephemeris |
|         | `ephemeris-reference-id` | Ephemeris Reference ID |

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `change-indicator:` | `required` | Context Field Change Indicator is always present |
|                     | `optional` | Context Field Change Indicator can be enabled or disabled at runtime |
| `reference-point-id:` | `required` | Reference Point Identifier is always present |
|                       | `optional` | Reference Point Identifier can be enabled or disabled at runtime |
| `bandwidth:` | `required` | Bandwidth is always present |
|              | `optional` | Bandwidth can be enabled or disabled at runtime |
| `if-reference-frequency:` | `required` | IF Reference Frequency is always present |
|                           | `optional` | IF Reference Frequency can be enabled or disabled at runtime |
| `rf-reference-frequency:` | `required` | RF Reference Frequency is always present |
|                           | `optional` | RF Reference Frequency can be enabled or disabled at runtime |
| `rf-reference-frequency-offset:` | `required` | RF Reference Frequency Offset is always present |
|                                  | `optional` | RF Reference Frequency Offset can be enabled or disabled at runtime |
| `if-band-offset:` | `required` | IF Band Offset is always present |
|                   | `optional` | IF Band Offset can be enabled or disabled at runtime |
| `reference-level:` | `required` | Reference Level is always present |
|                    | `optional` | Reference Level can be enabled or disabled at runtime |
| `gain:` | `required` | Gain is always present |
|         | `optional` | Gain can be enabled or disabled at runtime |
| `over-range-count:` | `required` | Over-range Count is always present |
|                     | `optional` | Over-range Count can be enabled or disabled at runtime |
| `sample-rate:` | `required` | Sample Rate is always present |
|                | `optional` | Sample Rate can be enabled or disabled at runtime |
| `timestamp-adjustment:` | `required` | Timestamp Adjustment is always present |
|                         | `optional` | Timestamp Adjustment can be enabled or disabled at runtime |
| `timestamp-calibration-time:` | `required` | Timestamp Calibration Time is always present |
|                               | `optional` | Timestamp Calibration Time can be enabled or disabled at runtime |
| `temperature:` | `required` | Temperature is always present |
|                | `optional` | Temperature can be enabled or disabled at runtime |
| `device-id:` | `required` | Device Identifier is always present |
|              | `optional` | Device Identifier can be enabled or disabled at runtime |
| `data-payload-format:` | `required` | Data Packet Payload Format is always present |
|                        | `optional` | Data Packet Payload Format can be enabled or disabled at runtime |
| `formatted-gps:` | `required` | Formatted GPS is always present |
|                  | `optional` | Formatted GPS can be enabled or disabled at runtime |
| `formatted-ins:` | `required` | Formatted INS is always present |
|                  | `optional` | Formatted INS can be enabled or disabled at runtime |
| `ecef-ephemeris:` | `required` | ECEF Ephemeris is always present |
|                   | `optional` | ECEF Ephemeris can be enabled or disabled at runtime |
| `relative-ephemeris:` | `required` | Relative Ephemeris is always present |
|                       | `optional` | Relative Ephemeris can be enabled or disabled at runtime |
| `ephemeris-reference-id:` | `required` | Ephemeris Reference ID is always present |
|                           | `optional` | Ephemeris Reference ID can be enabled or disabled at runtime |

```
ExamplePacket:
  control:
    cif0:
      bandwidth: required
      rf-reference-frequency: required
      gain: optional
      sample-rate: required
```

#### 1.7.2 Context/Control Indicator Field (CIF) 1

The Context/Control Indicator Field sections can be added to Context and Command packet types to
describe the CIF enable words and the associated field values that can be set in the resulting
packets. The following table provides the options available to the `cif1:` section.

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `cif1:` | `phase-offset` | Phase Offset |
|         | `polarization` | Polarization |
|         | `3d-pointing-vector` | 3-D Pointing Vector |
|         | `spatial-scan` | Spatial Scan Type |
|         | `beam-width` | Beam Width |
|         | `range` | Range |
|         | `ebno-ber` | Eb/No BER |
|         | `threshold` | Threshold |
|         | `compression-point` | Compression Point |
|         | `snr-noise-figure` | SNR/Noise Figure |
|         | `aux-frequency` | Aux Frequency |
|         | `aux-gain` | Aux Gain |
|         | `aux-bandwidth` | Aux Bandwidth |
|         | `discrete-io-32` | Discrete I/O 32-bit |
|         | `discrete-io-64` | Discrete I/O 64-bit |
|         | `health-status` | Health Status |
|         | `version-build-code` | Version and Build Code |
|         | `buffer-size` | Context Field Change Indicator |

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `phase-offset:` | `required` | Phase Offset is always present |
|                 | `optional` | Phase Offset can be enabled or disabled at runtime |
| `polarization:` | `required` | Polarization is always present |
|                 | `optional` | Polarization can be enabled or disabled at runtime |
| `3d-pointing-vector:` | `required` | 3-D Pointing Vector is always present |
|                       | `optional` | 3-D Pointing Vector can be enabled or disabled at runtime |
| `spatial-scan:` | `required` | Spatial Scan Type is always present |
|                 | `optional` | Spatial Scan Type can be enabled or disabled at runtime |
| `beam-width:` | `required` | Beam Width is always present |
|               | `optional` | Beam Width can be enabled or disabled at runtime |
| `range:` | `required` | Range (Distance) is always present |
|          | `optional` | Range (Distance) can be enabled or disabled at runtime |
| `ebno-ber:` | `required` | Eb/No BER is always present |
|             | `optional` | Eb/No BER  can be enabled or disabled at runtime |
| `threshold:` | `required` | Threshold is always present |
|              | `optional` | Threshold can be enabled or disabled at runtime |
| `compression-point:` | `required` | Compression Point is always present |
|                      | `optional` | Compression Point can be enabled or disabled at runtime |
| `snr-noise-figure:` | `required` | SNR/Noise Figure is always present |
|                     | `optional` | SNR/Noise Figure can be enabled or disabled at runtime |
| `aux-frequency:` | `required` | Aux Frequency is always present |
|                  | `optional` | Aux Frequency can be enabled or disabled at runtime |
| `aux-gain:` | `required` | Aux Gain is always present |
|             | `optional` | Aux Gain can be enabled or disabled at runtime |
| `aux-bandwidth:` | `required` | Aux Bandwidth is always present |
|                  | `optional` | Aux Bandwidth can be enabled or disabled at runtime |
| `discrete-io-32:` | `required` | Discrete I/O 32-bit is always present |
|                   | `optional` | Discrete I/O 32-bit can be enabled or disabled at runtime |
| `discrete-io-64:` | `required` | Discrete I/O 64-bit is always present |
|                   | `optional` | Discrete I/O 64-bit can be enabled or disabled at runtime |
| `health-status:` | `required` | Health Status is always present |
|                  | `optional` | Health Status can be enabled or disabled at runtime |
| `version-build-code:` | `required` | Version and Build Code is always present |
|                       | `optional` | Version and Build Code can be enabled or disabled at runtime |
| `buffer-size:` | `required` | Buffer Size is always present |
|                | `optional` | Buffer Size can be enabled or disabled at runtime |

### 1.8 Trailer

Signal Data packets have an optional trailer that be included by adding the `trailer:` mapping to
the YAML sprecification.

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `trailer:` | `calibrated-time` | Calibrated Time indicator |
|            | `valid-data` | Valid Data indicator |
|            | `reference-lock` | Reference Lock indicator |
|            | `agc-mgc` | AGC/MGC indicator |
|            | `detected-signal` | Detected Signal indicator |
|            | `spectral-inversion` | Spectral Inversion indicator |
|            | `over-range` | Over-range indicator |
|            | `sample-loss` | Sample Loss indicator |

| YAML Mapping | Options | Meaning |
| ------------ | ------- | ------- |
| `calibrated-time:` | `required` | Calibrated Time is always present |
|                    | `optional` | Calibrated Time can be enabled or disabled at runtime |
| `valid-data:` | `required` | Valid Data is always present |
|                | `optional` | Valid Data can be enabled or disabled at runtime |
| `reference-lock:` | `required` | Reference Lock is always present |
|                   | `optional` | Reference Lock can be enabled or disabled at runtime |
| `agc-mgc:` | `required` | AGC/MGC is always present |
|            | `optional` | AGC/MGC can be enabled or disabled at runtime |
| `detected-signal:` | `required` | Detected Signal is always present |
|                    | `optional` | Detected Signal can be enabled or disabled at runtime |
| `spectral-inversion:` | `required` | Spectral Inversion is always present |
|                       | `optional` | Spectral Inversion can be enabled or disabled at runtime |
| `over-range:` | `required` | Over-range is always present |
|               | `optional` | Over-range can be enabled or disabled at runtime |
| `sample-loss:` | `required` | Sample Loss is always present |
|                | `optional` | Sample Loss can be enabled or disabled at runtime |

#### Signal Data with Trailer Example

```
ExamplePacket:
  data:
    trailer:
      valid-data: optional
      agc-mgc: required
```