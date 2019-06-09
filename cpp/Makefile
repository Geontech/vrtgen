PYTHON = python3
srcdir = .

CPPFLAGS = -I $(srcdir)/include
CXXFLAGS = $(CPPFLAGS) -Wall

HEADERS := include/vrtgen/types.hpp
HEADERS += include/vrtgen/utils/macros.hpp

GENERATED_SOURCES := include/vrtgen/enums.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif0.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif1.hpp
GENERATED_SOURCES += include/vrtgen/packing/header.hpp
GENERATED_SOURCES += include/vrtgen/packing/trailer.hpp

TEST_PROGRAM = test_libvrtgen
TEST_SOURCES := tests/test.o
TEST_SOURCES += tests/main.o

.PHONY: generate check

check: $(TEST_PROGRAM)
	./$(TEST_PROGRAM) $(TEST_OPTIONS)

$(TEST_PROGRAM): $(TEST_SOURCES)
	$(CXX) $(LDFLAGS) -o $@ $(TEST_SOURCES)

tests/test.cpp: $(GENERATED_SOURCES) $(HEADERS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

generate: $(GENERATED_SOURCES)

$(GENERATED_SOURCES) : gencpp.py templates/enums.hpp templates/struct.hpp
	$(PYTHON) gencpp.py

clean:
	rm -f $(GENERATED_SOURCES)
	rm -f $(TEST_PROGRAM)
	rm -f tests/*.o
