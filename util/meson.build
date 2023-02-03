util_src = files(
  'gtrie.c',
  'packed.c',
  'url-scanner.c',
)

# libutil = library('util', util_src, dependencies : deps, include_directories: inc_lib)
inc_conf = include_directories('..')
libutil = static_library('util', util_src, dependencies : deps, include_directories: inc_conf)
libutil_dep = declare_dependency(link_with : libutil,
  include_directories : inc_conf)
