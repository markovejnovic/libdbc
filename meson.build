project('libdbc', 'c')

includes = include_directories('include')
sources = ['src/libdbc.c', 'src/parser.c']
version = '0.1.0'
soversion = '0'

library('libdbc', sources, version: version, soversion: soversion,
        include_directories: includes)
