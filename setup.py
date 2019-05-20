#!/usr/bin/env python3

from setuptools import setup, find_packages

# For a single point of maintenance, the canonical package version is in the
# version module
version = {}
with open('src/vrtgen/version.py') as fp:
    exec(fp.read(), version)

setup(
    name='vrtgen',
    version=version['__version__'],
    packages=find_packages('src'),
    package_dir={'':'src'},
    python_requires='>= 3.6',
    install_requires=[
        'PyYaml ~= 3.11',
        'Jinja2 ~= 2.8',
    ],
    entry_points={
        'console_scripts': [
            'vrtpktgen=vrtgen.main:main',
        ],
    },
)
