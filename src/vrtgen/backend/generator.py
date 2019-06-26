class Generator:
    def set_option(self, name, value):
        raise ValueError("invalid option '"+name+"'")

    def start_file(self, filename):
        """
        Notification for the start of processing of an input YAML file.

        Override if your generator needs to perform any actions at the start
        of an input file, such as opening an output file.
        """

    def end_file(self):
        """
        Notification for the end of processing of an input YAML file.

        Override if your generator needs to perform any actions at the end of
        an input file, such as closing an output file.
        """

    def generate(self, packet):
        raise NotImplementedError('generate')
