# Example Application

This example is meant to be a simple demonstration of various packet classes
being aggregated together to form a transactional interface for VITA 49.2. The
following instructions assume that `vrtpktgen` utility and the `vrtgen` header
library has been installed.

## Generating VITA 49.2 Code

To get started, we need to generate some V49.2 source code. The included YAML
file contains an Information Class specification that needs to be passed to the
`vrtpktgen` command line tool in order to generate the code required to build
the application.

To do this, execute the provided script:

```
$ ./generate.sh
```

Once this script has been run, a new folder `example_project` will have been
generated that includes the VITA 49.2 code that was specified by the packets
included in the YAML. These packet specifications can be found one directory
back at `examples/packets/`.

This project structure comes with a CMake build structure that can be executed
with the following:

```sh
$ cd example_project
$ cmake -B build
$ cmake --build build
```

## Implement Stub Functions

At this point, we have successfully built our project, but the application code
is not yet in place. Run the following commands to copy the test files into the
project directory.

```sh
$ cp ../example_controllee.cpp ./src/bin/.
$ cp ../example_controller.cpp ./src/bin/.
```

Edit the src/bin/CMakeLists.txt file to look like the following:

```
add_executable(example_project main.cpp)
target_link_libraries(example_project PUBLIC packetlib)
target_include_directories(example_project PUBLIC
    ${vrtgen_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/packets
)

add_executable(example_controllee example_controllee.cpp)
target_link_libraries(example_controllee PUBLIC packetlib pthread)
target_include_directories(example_controllee PUBLIC
    ${vrtgen_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/packets
)

add_executable(example_controller example_controller.cpp)
target_link_libraries(example_controller PUBLIC packetlib pthread)
target_include_directories(example_controller PUBLIC
    ${vrtgen_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/packets
)
```

Re-run the CMake build to incorporate these new executables:

```sh
$ cmake --build build
```

Taking a closer look at the Controllee.hpp file in `include/` folder we can see
that there is a function stub that needs to be implemented to handle the
incoming control packet. This function is stubbed out for a Controllee
developer to implement with the necessary functionality for thier frontend. The
execution of this function is automatically handled by the base class and is
not something that the Controllee developer needs to manage themselves.

```
auto execute_example_control(ExampleControl& packet) -> ExampleControlAckVX override
{
    auto ack = ExampleControlAckVX{};
    // AUTO-GENERATED FUNCTION STUB
    // IMPLEMENT PACKET HANDLING FUNCTIONALITY HERE
    return ack;
}
```

To add in some functionality to the Controllee interface, run the following
from the top level of `example_project`:

```
cp ../.Controllee.hpp include/Controllee.hpp
```

## Build

Rebuild the project to incorporate these new changes.

```
cmake --build build
```

## Run

In a terminal window first launch the Controllee interface to act as the mock
application waiting for VRT commands.

```
./build/src/bin/example_controllee
```

In another terminal window the Controller program can now be run to start
sending commands to the Controllee and receiving Acknowledge, Data, and Context
packets.

```
./build/src/bin/example_controller
```