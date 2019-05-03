from .struct import Struct, StructEntry, Enable, Field, Reserved

class ContextIndicatorFields:
    def __init__(self, name):
        self.name = name
        self._fields = []

    def __setattr__(self, attr, value):
        # Automatically add struct entries to the internally-tracked list of
        # fields. This enables statements like:
        #
        #     CIF0 = ContextIndicatorFields('CIF0')
        #     CIF0.change_indicator = Enable('...')
        #
        # This pattern supports defining fields as attributes for more static
        # checking of names.
        if isinstance(value, StructEntry) and not hasattr(self, attr):
            self._fields.append((attr, value))
        return super().__setattr__(attr, value)

    def get_fields(self):
        """
        Returns the CIF fields that can be present in a Context/Command packet
        payload.
        """
        # Only return contents that are defined as fields. Some CIF0 bits are
        # simple indicators, and there are a handful of reserved bits; don't
        # report these.
        return [value for attr, value in self._fields if isinstance(value, Field)]

    def create_enables(self):
        """
        Returns a struct class containing the CIF bit flags to enable the
        defined fields.
        """
        namespace = {}
        for attr, field in self._fields:
            # Turn all non-reserved CIF bits into enable flags
            if isinstance(field, Reserved):
                entry = Reserved(1)
            else:
                entry = Enable(field.name)
            namespace[attr] = entry
        return Struct.__class__(self.name + 'Enables', (Struct,), namespace)
