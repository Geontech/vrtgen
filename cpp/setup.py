#!/usr/bin/env python3

from setuptools import setup, find_packages

setup(
    name='vrtgen-cpp',
    version='0.1',
    packages=['vrtgen.backend.cpp'],
    package_dir={'':'python'},
    package_data={
        '': ['templates/*'],
    },
    python_requires='>= 3.6',
    install_requires=[
        'vrtgen == 0.1',
        'Jinja2 ~= 2.8',
    ],
    entry_points={
        'vrtgen.backend.packet': [
            'cpp=vrtgen.backend.cpp.generator:CppGenerator',
        ],
    },
)
