PYTHON = python3
top_srcdir = ..

GENERATED_SOURCES := include/vrtgen/enums.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif0.hpp
GENERATED_SOURCES += include/vrtgen/packing/cif1.hpp

.PHONY: generate

generate: $(GENERATED_SOURCES)

$(GENERATED_SOURCES) : gencpp.py
	PYTHONPATH=$(top_srcdir) $(PYTHON) gencpp.py

clean:
	rm -f $(GENERATED_SOURCES)
