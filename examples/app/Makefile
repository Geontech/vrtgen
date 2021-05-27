CXX=g++
CXX_VERSION=11

all: controller controllee

controller: test_controller.cpp rdc-information.cpp
	$(CXX) -std=c++$(CXX_VERSION) -o test_controller test_controller.cpp rdc-information.cpp -lpthread

controllee: test_controllee.cpp rdc-information.cpp
	$(CXX) -std=c++$(CXX_VERSION) -o test_controllee test_controllee.cpp rdc-information.cpp -lpthread

clean:
	rm -f test_controller test_controllee RDCInformationControl*pp rdc-information.*pp
