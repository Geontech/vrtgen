# Copyright (C) 2022 Geon Technologies, LLC
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

ENTRY_POINT_ID = 'vrtgen.backend.packet'

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
    common_parser.add_argument(
        '-p',
        '--project',
        action='store_true',
        default=False,
        help='create project with directory structure'
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
        backend_parser.add_argument('--version', action='version', version='%(prog)s '+version)
        for attr, option in generator.get_options():
            backend_parser.add_argument(
                option.opt,
                dest=attr,
                type=option.type,
                help=option.help,
                default=option.default,
                choices=option.choices
            )

    # Load all registered generators and include their command line options
    generators = {}
    for entry_point in pkg_resources.iter_entry_points(ENTRY_POINT_ID):
        version = entry_point.dist.version
        generator = entry_point.load()
        _add_backend_parser(entry_point.name, version, generator)
        generators[entry_point.name] = generator

    args = arg_parser.parse_args()

    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    try:
        generator = generators[args.generator]()
    except KeyError:
        raise SystemExit("invalid backend '"+args.backend+"'")

    if args.project:
        if hasattr(generator, 'project'):
            setattr(generator, 'project', args.project)
        else:
            raise SystemExit("generator {} does not have a project option".format(generator.__class__.__name__))

    for attr, _ in generator.get_options():
        value = getattr(args, attr, None)
        if value is not None:
            setattr(generator, attr, value)

    for filename in args.filename:
        logging.debug('Parsing %s', filename)
        generator.start_file(filename)
        for name,value in parser.parse_file(filename, generator.get_loader()):
            if name == 'files':
                generator.yamls = value
            else:
                try:
                    generator.generate(name, value)
                except RuntimeError as exc:
                    logging.error('Generator error: %s', exc)
        generator.end_file()

# Support running via "python -m vrtgen.main", used for C++ build
if __name__ == '__main__':
    main()
