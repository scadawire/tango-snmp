#!/usr/bin/env python
# -*- coding: utf-8 -*-
import os
import sys

import setuptools

setup_dir = os.path.dirname(os.path.abspath(__file__))

# prevent unnecessary installation of pytest-runner
needs_pytest = {"pytest", "test", "ptr"}.intersection(sys.argv)
pytest_runner = ["pytest-runner"] if needs_pytest else []

with open('README.md') as readme_file:
    readme = readme_file.read()

setuptools.setup(
    name='tangosnmp',
    version='0.0.1',
    description="",
    long_description=readme + '\n\n',
    url='https://github.com/ska-telescope/tangosnmp',
    #packages=find_packages(),
    #packages=setuptools.find_namespace_packages(where="src", include=["ska.*"]),
    package_dir={"": "src"},
    include_package_data=True,
    license="CSIRO Open Source Software Licence Agreement",
    zip_safe=False,
    keywords='tangosnmp',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
    ],
    #test_suite='tests',
    install_requires=['pytango == 9.3.1',
        'snmpsim'],  
    setup_requires=[] + pytest_runner,
    tests_require=[
        'pytest',
        'pytest-json-report',
        'pytest-forked' ],
)
