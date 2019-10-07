# Copyright (C) 2019 Geon Technologies, LLC
#
# This file is part of vrtgen.
#
# vrtgen is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# vrtgen is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

"""
Command-line interface for vrtpktgen.
"""
import logging
import argparse

import pkg_resources

from vrtgen import parser
from vrtgen.version import __version__
from vrtgen.backend.generator import Generator

ENTRY_POINT_ID = 'vrtgen.backend.packet'

class CheckGenerator(Generator):
    """
    Validates packet definitions.
    """
    def generate(self, packet):
        pass

def load_generator(name):
    if name == 'check':
        return CheckGenerator()

    for entry_point in pkg_resources.iter_entry_points(ENTRY_POINT_ID):
        if entry_point.name == name:
            generator = entry_point.load()
            return generator()

    raise KeyError(name)

def main():
    """
    Main entry point for vrtpktgen.
    """
    logging.basicConfig()

    common_parser = argparse.ArgumentParser(add_help=False)
    common_parser.add_argument(
        '-v',
        '--verbose',
        action='store_true',
        default=False,
        help='display debug messages'
    )

    arg_parser = argparse.ArgumentParser(
        description='Generate VITA 49.2 packet classes.',
        parents=[common_parser],
        epilog="Run '%(prog)s GENERATOR --help' for more information on a generator.",
    )
    arg_parser.add_argument('--version', action='version', version='%(prog)s '+__version__)
    sub_parser = arg_parser.add_subparsers(dest='generator', metavar='GENERATOR')
    arg_parser.add_argument(
        'filename',
        nargs='+',
        help='VRT YAML definition file',
        metavar='FILENAME'
    )

    def _add_backend_parser(name, version, generator):
        backend_parser = sub_parser.add_parser(
            name,
            help=generator.__doc__,
            description=generator.__doc__,
            parents=[common_parser],
            usage='%(prog)s [OPTIONS] FILENAME [FILENAME ...]'
        )
        backend_parser.add_argument(
            'filename',
            nargs='*',
            help='VRT YAML definition file',
            metavar='FILENAME'
        )
        backend_parser.add_argument(
            '-o',
            '--option',
            action='append',
            default=[],
            help='options for code generator backend'
        )
        backend_parser.add_argument('--version', action='version', version='%(prog)s '+version)

    _add_backend_parser('check', __version__, CheckGenerator)

    for entry_point in pkg_resources.iter_entry_points(ENTRY_POINT_ID):
        version = entry_point.dist.version
        generator = entry_point.load()
        _add_backend_parser(entry_point.name, version, generator)

    args = arg_parser.parse_args()

    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    try:
        generator = load_generator(args.generator)
    except KeyError:
        raise SystemExit("invalid backend '"+args.backend+"'")

    for option in args.option:
        if '=' in option:
            name, value = option.split('=', 1)
        else:
            name = option
            value = True
        try:
            generator.set_option(name, value)
        except Exception as exc:
            raise SystemExit(str(exc))

    for filename in args.filename:
        logging.debug('Parsing %s', filename)
        generator.start_file(filename)
        for packet in parser.parse_file(filename):
            try:
                generator.generate(packet)
            except RuntimeError as exc:
                logging.error('Generator error: %s', exc)
        generator.end_file()

# Support running via "python -m vrtgen.main", used for C++ build
if __name__ == '__main__':
    main()
