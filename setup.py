from distutils.core import setup, Extension

ext = Extension('utils', sources=['library.c'])
setup(name='Utils', version='1.0', ext_modules=[ext])
