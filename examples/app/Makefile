CXX=g++
CXX_VERSION=20

all: controller controllee

controller: test_controller.cpp example_information.cpp
	$(CXX) -std=c++$(CXX_VERSION) -I../../include -o test_controller test_controller.cpp example_information.cpp -lpthread

controllee: test_controllee.cpp example_information.cpp
	$(CXX) -std=c++$(CXX_VERSION) -I../../include -o test_controllee test_controllee.cpp example_information.cpp -lpthread

clean:
	rm -f test_controller test_controllee ExampleInfoControl*pp example_information.*pp
