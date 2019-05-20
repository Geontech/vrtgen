#!/usr/bin/env python3

from setuptools import setup, find_packages

setup(
    name='vrtgen',
    version='0.1',
    packages=find_packages('src'),
    package_dir={'':'src'},
    install_requires=[
        'PyYaml ~= 3.11',
        'Jinja2 ~= 2.8',
    ],
)
