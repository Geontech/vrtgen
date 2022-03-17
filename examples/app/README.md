# Example Application

This example is meant to be a simple demonstration of various packet classes
being aggregated together to form a transactional interface for VITA 49.2. The
following instructions assume that `vrtgen` has been previously built from
the top level of the repository.

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
be executed. If the code were to be executed, there would be an exception thrown
by the Controllee application because the control packet callback handler would
not have been implemented yet. Examining the `ExampleInformationClassControllee.hpp`
file we can see why:

```
virtual void handle_example_control(const ExampleControl& packet)
{
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT PACKET HANDLING FUNCTIONALITY HERE
    throw std::runtime_error("handle_example_control not implemented");
}
```

All packet handlers in this file get stubbed out to throw a
`std::runtime_error` by default. The functions in this file are left as stubs
to be implemented with the specific functionality that is required for the
user's application.

To add in some functionality to the Controllee interface, run the following:

```
cp .ExampleInfoControllee.hpp ExampleInfoControllee.hpp
```

## Build

```
make
```

## Run

In a terminal window first launch the Controllee interface to act as the mock
application waiting for VRT commands.

```
./test_controllee
```

In another terminal window the Controller program can now be run to start
sending commands to the Controllee and receiving Acknowledge, Data, and Context
packets.

```
./test_controller
```