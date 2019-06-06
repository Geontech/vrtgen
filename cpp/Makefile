PYTHON = python3
top_srcdir = ..
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

.PHONY: generate check

check: test
	./test

test: tests/test.cpp $(GENERATED_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o test tests/test.cpp

generate: $(GENERATED_SOURCES)

$(GENERATED_SOURCES) : gencpp.py templates/enums.hpp templates/struct.hpp
	$(PYTHON) gencpp.py

clean:
	rm -f $(GENERATED_SOURCES) test
