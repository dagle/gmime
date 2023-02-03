project('gmime',
	['c'],
	license: 'GPLv2',
    # version: '@0@.@1@.@2@'.format(GMIME_MAJOR_VERSION, GMIME_MINOR_VERSION, GMIME_MICRO_VERSION),
    version: '3.2.13',
    meson_version: '>=1.0.0')

GMIME_MAJOR_VERSION=3
GMIME_MINOR_VERSION=2
GMIME_MICRO_VERSION=13
GMIME_BINARY_AGE= 100 * GMIME_MINOR_VERSION + GMIME_MICRO_VERSION
GMIME_INTERFACE_AGE=0

# make this into a option
add_global_arguments('-DHAVE_CONFIG_H=1', language : 'c')

  # dep_glib = dependency('glib-2.0', version: '>=2.68')
  # dep_zlib = dependency('zlib', version: '>=1.2.5.2')
  # dep_gpgme = dependency('gpgme', version: '>=1.6.0')
  # dep_libidn2 = dependency('libidn2', version: '>=2.0.0')
  #
# docbook-utils (make)
# gobject-introspection (make)
# gtk-doc (make)
# vala (make)
iconv = dependency('iconv', required : false)
libidn2 = dependency('libidn2', version: '>=2.0.0', required : false)
deps = [
  dependency('glib-2.0', version: '>=2.68'),
  dependency('zlib', version: '>=1.2.5.2'),
  dependency('gpgme', version: '>=1.6.0'),
  dependency('gobject-2.0'),
  dependency('gio-2.0'),
  dependency('gthread-2.0'),
  libidn2,
  iconv,
]

cc = meson.get_compiler('c')

conf_data = configuration_data()
conf_data.set('GMIME_VERSION', '@0@.@1@.@2@'.format(GMIME_MAJOR_VERSION, GMIME_MINOR_VERSION, GMIME_MICRO_VERSION))


if cc.has_header('getopt.h')
  if cc.has_function('getopt_long')
    conf_data.set('HAVE_GETOPT_H', 1)
  endif
endif

# if cc.has_header('dlfcn.h')
#   conf_data.set('HAVE_DLFCN_H', 1)
# endif
#
# if cc.has_header('inttypes.h')
#   conf_data.set('HAVE_INTTYPES_H', 1)
# endif
if libidn2.found()
  conf_data.set('LIBIDN', 1)
endif

if iconv.found()
  conf_data.set('HAVE_ICONV_DETECT_H', 1)
endif

foreach hd : [
    { 'm': 'HAVE_DLFCN_H',                   'v': 'dlfcn.h', },
    { 'm': 'HAVE_INTTYPES_H',                'v': 'inttypes.h', },
    { 'm': 'HAVE_NETDB_H',                   'v': 'netdb.h', },
    { 'm': 'HAVE_STDINT_H',                  'v': 'stdint.h', },
    # { 'm': 'HAVE_POLL_H',                    'v': 'poll.h', },
    # { 'm': 'HAVE_STDIO_H',                   'v': '', },
    # { 'm': 'HAVE_STDLIB_H',                   'v': '', },
    # { 'm': 'HAVE_STRINGS_H',                   'v': '', },
    # { 'm': 'HAVE_STRING_H',                   'v': '', },
    { 'm': 'HAVE_SYS_MMAN_H',                   'v': 'sys/mman.h', },
    { 'm': 'HAVE_SYS_PARAM_H',                   'v': 'sys/param.h', },
    # { 'm': 'HAVE_WINSOCK2_H',                   'v': '', },
    # { 'm': '',                   'v': '', },
    # { 'm': '',                   'v': '', },
    # { 'm': '',                   'v': '', },
    # { 'm': '',                   'v': '', },
  ]
  if cc.has_header(hd['v'])
    conf_data.set10(hd['m'], true)
  endif
endforeach

uts_prefix = '''
#define _GNU_SOURCE
#include <sys/utsname.h>
'''

if cc.has_member('struct utsname', 'domainname', prefix : uts_prefix)
  conf_data.set10('HAVE_UTSNAME_DOMAINNAME', true)
endif

if cc.has_header_symbol('fcntl.h', 'O_LARGEFILE')
  conf_data.set10('O_LARGEFILE', true)
endif

foreach fn : [
    # { 'm': 'HAVE_ALLOCA',                   'v': 'alloca', },
    # { 'm': 'HAVE_DCGETTEXT',                'v': 'dcgettext', },
    # { 'm': 'HAVE_DIFFTIME',                 'v': 'difftime', },
    # { 'm': 'HAVE_GETNAMEINFO',              'v': 'getnameinfo', },
    # { 'm': 'HAVE_FSYNC',                    'v': 'fsync', },
    { 'm': 'HAVE_GETADDRINFO',              'v': 'getaddrinfo', },
    { 'm': 'HAVE_GETDOMAINNAME',            'v': 'getdomainname', },
    { 'm': 'HAVE_GETHOSTNAME',              'v': 'gethostname', },
    { 'm': 'HAVE_GETPAGESIZE',              'v': 'getpagesize', },
    { 'm': 'HAVE_MMAP',                     'v': 'mmap', },
    { 'm': 'HAVE_MSYNC',                    'v': 'msync', },
    { 'm': 'HAVE_MUNMAP',                   'v': 'munmap', },
    { 'm': 'HAVE_POLL',                     'v': 'poll', },
    { 'm': 'HAVE_SELECT',                   'v': 'select', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': '',              'v': '', },
    # { 'm': 'HAVE_GETTEXT',                  'v': 'gettext', },
    # { 'm': 'HAVE_MMAP',                     'v': 'mmap', },
    # { 'm': 'HAVE_RINT',                     'v': 'rint', },
    # { 'm': 'HAVE_THR_SELF',                 'v': 'thr_self', },
    # { 'm': 'HAVE_VFORK',                    'v': 'vfork', },
  ]
  if cc.has_function(fn['v'])
    conf_data.set(fn['m'], 1)
  endif
endforeach
if cc.has_function('fsync')
  conf_data.set('HAVE_FSYNC', 1)
endif
# if cc.has_function()

# if cc.has_header('alloca.h')
#   conf_data.set('HAVE_ALLOCA_H', 1)
# endif

configure_file(input: 'config_meson.h.in',
  output: 'config.h',
  configuration : conf_data
)

# add_global_arguments('-Wno-deprecated-declarations', language: ['c', 'cpp'])

# add_project_arguments('-D_GNU_SOURCE', language: ['c', 'cpp'])
# add_project_arguments('-include', 'config.h', language: ['c', 'cpp'])

# inc_cli = include_directories('.')
#

subdir('util')
subdir('gmime')
# subdir('parse-time-string')
# subdir('test')

#
# executable('notmuch', notmuch_client_srcs,
# 	   include_directories: [inc_util, inc_compat, inc_lib],
# 	   dependencies: [dep_gmime, dep_zlib, dep_glib, dep_talloc],
# 	   link_with: [lib_compat, lib_util, lib_parse_time, lib_notmuch.get_static_lib()],
# 	   install: true)
#
# executable('notmuch-shared', notmuch_client_srcs,
# 	   include_directories: [inc_util, inc_compat, inc_lib],
# 	   dependencies: [dep_gmime, dep_zlib, dep_glib, dep_talloc],
# 	   link_with: [lib_compat, lib_util, lib_parse_time, lib_notmuch],
# 	   install: true)
#
# configure_file(input: 'sh.config.in',
# 	       output: 'sh.config',
# 	       configuration: config)
