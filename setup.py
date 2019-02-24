#!venv/bin/python
from setuptools import setup, find_packages
from setuptools.extension import Extension
import sys

# Setuptools bug workaround issue #10945
import codecs
try:
    codecs.lookup('mbcs')
except LookupError:
    ascii = codecs.lookup('ascii')
    func = lambda name, enc=ascii: {True: enc}.get(name == 'mbcs')
    codecs.register(func)

# Cannot build ptp driver on windows at the moment.
if sys.platform.startswith("win"):
    ext_modules = []

else:
    ext_modules = [
        Extension('pygmalion.ptp',
            ['ptp/ptp.c', 'ptp/usb.c', 'ptp/camera.c'],
            include_dirs=['ptp/', '/usr/include/libusb-1.0/', 'include/'],
            libraries=['usb-1.0'],
        )
    ]

setup(
    name='pygmalion',
    packages=find_packages(exclude=['etc', 'contrib']),
    description='Control a bunch of cameras at once',
    long_description="""Use Photogrammetry to scan objects or people.""",
    license="MIT",
    author='Mark Murnane',
    author_email='mark@hackafe.net',
    url='https://github.com/bitbyt3r/pygmalion',
    keywords=[
        '3d', 'creative', 'photogrammetry',
    ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: No Input/Output (Daemon)',
        'Operating System :: POSIX :: Linux',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3 :: Only',
    ],
    entry_points={
        'console_scripts': [
            'pygmalion-server=pygmalion.__main__:server',
            'pygmalion=pygmalion.__main__:cli',
        ]
    },
    ext_modules=ext_modules,
)
