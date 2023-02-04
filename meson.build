project('libdbc', 'c',
        default_options: ['c_std=c99',
                          'warning_level=3',
                          'werror=false'])  # TODO: Enable werror

# Dependencies for all
cc = meson.get_compiler('c')
deps = [cc.find_library('m', required: true)]

lib_includes = ['lib/sds', 'lib/hashtable']
lib_sources = ['lib/sds/sds.c',
               'lib/hashtable/hashtable.c',
               'lib/hashtable/hashtable_itr.c',
               'lib/hashtable/hashtable_utility.c']

# Main Library
includes = include_directories('include', lib_includes)
sources = ['src/libdbc.c',
           'src/libdbc_node.c',
           'src/libdbc_value_table.c',
           'src/parser.c',
           lib_sources]
version = '0.1.0'
soversion = '0'

library('libdbc', sources, version: version, soversion: soversion,
        include_directories: includes,
        dependencies: deps)

# Tests
check_dep = dependency('check', required: false, disabler: true)
test_deps = [check_dep, deps]

test_cc_args = ['-DLIBDBC_TEST']
test_includes = [includes]
test_exes = [
    {
        'name': 'node_test',
        'sources': ['test/test_node.c'],
        'includes': [],
        'run': true
    },
    {
        'name': 'value_table_test',
        'sources': ['test/test_value_table.c'],
        'includes': [],
        'run': true
    },
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
