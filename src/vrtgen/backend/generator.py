class Generator:
    def set_option(self, name, value):
        raise ValueError("invalid option '"+name+"'")

    def generate(self, packet):
        raise NotImplementedError('generate')
