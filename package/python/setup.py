import os
from distutils.core import setup

setup(
	name = 'pydusa',
	version = '0.4.4',
	author = 'wisk',
	author_email = '...',
	description = ('pydusa is python binding for medusa'),
	license = 'GPL',
	keywords = [ 'reverse engineering', 'disassembler', 'emulator', 'symbolic execution' ],
	url = 'https://github.com/wisk/medusa',
	packages = [ 'pydusa', ],
	include_package_data = True,
)
