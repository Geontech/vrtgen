PYTHON = python3
top_srcdir = ..
srcdir = .

CPPFLAGS = -I $(srcdir)/include
CXXFLAGS = $(CPPFLAGS) -Wall

GENERATED_SOURCES := include/vrtgen/enums.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif0.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif1.hpp
GENERATED_SOURCES += include/vrtgen/packing/trailer.hpp

.PHONY: generate test

all: generate test

generate: $(GENERATED_SOURCES)

$(GENERATED_SOURCES) : gencpp.py templates/enums.hpp templates/trailer.hpp templates/cif.hpp
	PYTHONPATH=$(top_srcdir) $(PYTHON) gencpp.py

test: generate
	$(CXX) $(CXXFLAGS) -c lib/test.cpp

clean:
	rm -f $(GENERATED_SOURCES)
