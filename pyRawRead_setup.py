#!/usr/bin/env python

from distutils.core import setup , Extension

pyRawRead_module =Extension( '_pyRawRead',
				sources =['pyRawRead_wrap.cxx',
					'pyRawRead.cxx',
					'rawParser.cpp'],
				)
setup( 	name	='pyRawRead',
	version	='0,1',
	author	='Simon Peters',
	description='''Module for convert MRT Data in Matrix''',
	ext_modules=[pyRawRead_module],
	py_modules=["pyRawRead"],
	)
