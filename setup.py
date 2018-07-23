#!venv/bin/python
from setuptools import setup, find_packages
from setuptools.extension import Extension
import subprocess
import os
import sys
from pip.req import parse_requirements

# Setuptools bug workaround issue #10945
import codecs
try:
    codecs.lookup('mbcs')
except LookupError:
    ascii = codecs.lookup('ascii')
    func = lambda name, enc=ascii: {True: enc}.get(name == 'mbcs')
    codecs.register(func)

ext_modules = [
    Extension('pygmalion.ptp',
        ['ptp/ptp.c', 'ptp/usb.c', 'ptp/camera.c'],
        include_dirs=['ptp/', '/usr/include/libusb-1.0/'],
        libraries=['usb-1.0'],
    )
]

cmd = "git describe --tags --abbrev=0 HEAD"
result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
git_tag = result.stdout.readlines()[0].strip().decode('ASCII')
version_num = git_tag.lstrip('v')

cmd = "git rev-list {}..HEAD --count".format(git_tag)
result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
release_num = result.stdout.readlines()[0].strip().decode('ASCII')

print("Building {}-{}".format(version_num, release_num))

data_files = []

if os.environ.get('RPM_MODE') == "true":
    print("Adding migration files")
    for x in os.walk("migrations"):
        data_files.append((os.path.join('/usr/lib/pygmalion/', x[0]), [os.path.join(x[0], y) for y in x[2]]))
else:
    print("Not adding migration files")

if os.environ.get('RPM_MODE') == "true":
    print("Adding systemd service")
    data_files.append(('/usr/lib/systemd/system/', ('contrib/pygmalion-server.service',)))
else:
    print("Not adding systemd service")

setup(
    name='pygmalion',
    packages=find_packages(exclude=['etc', 'contrib']),
    version=version_num,
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
    data_files=data_files,
    entry_points={
        'console_scripts': [
            'pygmalion-server=pygmalion.__main__:server',
            'pygmalion=pygmalion.__main__:cli',
        ]
    },
    ext_modules=ext_modules,
)
