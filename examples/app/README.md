# Example Application

This example is meant to be a simple demonstration of various packet classes
being aggregated together to form a transactional interface for VITA 49.2. The
following instructions assume that `vrtgen` and `vrtgen/cpp` have been
installed per their respective READMEs.

## Generating VITA 49.2 Code

If you attempted to build the example straight away, you may have noticed that
there were files missing and that `make` failed. The included YAML file
contains an Information Class specification that needs to be passed to the
`vrtpktgen` command line tool in order to generate the code required to build
the application.

To do this, execute the provided script:

```
./generate.sh
```

Once this script has been run five new files will have been generated that
include the VITA 49.2 code that was specified by the packets included in the
YAML. These packet specifications can be found one directory back at 
`examples/packets/`.

## Implement Stub Functions

At this point, running `make` will succeed, but the code isn't quite ready to
be executed. If the code were to be executed, there would be exceptions thrown
by the Controllee application because certain functions have not yet been
implemented. Examining the `RDCInformationControllee.hpp` file we can see why:

```
virtual double getBandwidth()
{
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT HARDWARE-SPECIFIC FUNCTIONALITY HERE
    throw std::runtime_error("getBandwidth not implemented");
}
```

All of the functions in this file are stubbed out to throw a
`std::runtime_error` by default. The functions in this file are left as stubs
to be implemented with the hardware-specific functionality that is required
to interface with the radio.

To add in some functionality to the Controllee interface, run the following:

```
cp .RDCInformationControllee.hpp RDCInformationControllee.hpp
```

## Build

```
make
```

## Run

In a terminal window first launch the Controllee interface to act as the mock
radiohead waiting for VRT commands.

```
./test_controllee
```

In another terminal window the Controller program can now be run to start
sending commands to the Controllee and receiving Acknowledge, Data, and Context
packets.

```
./test_controller
```