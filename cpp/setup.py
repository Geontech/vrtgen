#!/usr/bin/env python3

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

from setuptools import setup, find_packages

setup(
    name='vrtgen-cpp',
    version='0.1.2',
    license='LGPL',
    packages=['vrtgen.backend.cpp'],
    package_dir={'':'python'},
    package_data={
        '': ['templates/*'],
    },
    python_requires='>= 3.6',
    install_requires=[
        'vrtgen ~= 0.1',
        'Jinja2 ~= 2.8',
    ],
    entry_points={
        'vrtgen.backend.packet': [
            'cpp=vrtgen.backend.cpp.generator:CppGenerator',
        ],
    },
)
