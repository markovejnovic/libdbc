project('libdbc', 'c', default_options: ['c_std=c90'])

# Main Library
includes = include_directories('include')
sources = ['src/libdbc.c', 'src/parser.c']
version = '0.1.0'
soversion = '0'

library('libdbc', sources, version: version, soversion: soversion,
        include_directories: includes)

# Tests
check_dep = dependency('check', required: false, disabler: true)
test_deps = [check_dep]

test_cc_args = ['-DLIBDBC_TEST']
test_includes = [includes]
test_exes = [
    {
        'name': 'creation_test',
        'sources': ['test/test_creation.c'],
        'includes': [],
        'run': true
    },
    {
        'name': 'parse_test',
        'sources': ['test/test_parsing.c'],
        'includes': ['src'],
        'run': true
    }
]

foreach test_exe : test_exes
    real_sources = [test_exe['sources'], sources]
    real_includes = [test_exe['includes'], test_includes]
    exe = executable(test_exe['name'], real_sources,
                     include_directories: real_includes,
                     dependencies: test_deps,
                     cpp_args: test_cc_args)
    if test_exe['run']
        test(test_exe['name'], exe)
    endif
endforeach
