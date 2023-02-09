expand_content_md_files = [
  'overview.md',
]

docs_dir = gmime_datadir / 'doc'

toml_conf = configuration_data()
toml_conf.set('version', meson.project_version())

gidocgen = find_program('gi-docgen', required: get_option('gtk_doc'), native: true)

doc_dependencies = [ 'GObject-2.0', 'Gio-2.0' ]

# if get_option('gtk_doc')
  gmime_toml = configure_file(
    input: 'gmime.toml.in',
    output: 'gmime.toml',
    configuration: toml_conf,
    install: true,
    install_dir: docs_dir / 'reference_gi',
  )
  custom_target('gmime-doc',
    input: [ gmime_toml, gmime_gir[0] ],
    output: 'gmime',
    command: [
      gidocgen,
      'generate',
      gidocgen_common_args,
      '--add-include-path=@0@'.format(meson.current_build_dir() / '../../gtk'),
      '--config=@INPUT0@',
      '--output-dir=@OUTPUT@',
      '--content-dir=@0@'.format(meson.current_build_dir()),
      '--content-dir=@0@'.format(meson.current_source_dir()),
      '@INPUT1@',
      ],
    # depends: [ gdk_gir[0], gsk_gir[0] ] + gtk_images,
    depend_files: [ expand_content_md_files ],
    build_by_default: true,
    install: true,
    install_dir: docs_dir,
    )
# endif

# content_files = [
#   'building.sgml',
#   'changes-2.0.sgml',
#   'changes-2.2.sgml',
#   'changes-2.4.sgml',
#   'changes-2.6.sgml',
#   'changes-3.0.sgml',
#   'compiling.sgml',
#   'data-wrappers.sgml',
#   'filters.sgml',
#   'question_index.sgml',
#   'resources.sgml',
#   'streams.sgml',
#   'tree_index.sgml',
# ]

# gnome = import('gnome')
#
# gnome.gtkdoc('gmime',
#   main_sgml: 'gmime-docs.sgml',
#   module_version: '@0@.0'.format(GMIME_MAJOR_VERSION),
#   src_dir: [meson.project_source_root() / 'gmime'],
#   content_files: [content_files],
#   mkdb_args: ['--sgml-mode', '--output-format=xml', '--ignore-files=trio'],
#   scanobjs_args: ['--type-init-func="g_mime_init ()"'],
#   # gobject_typesfile: 'umockdev.types',
#   scan_args: ['--deprecated-guards="GMIME_DISABLE_DEPRECATED"'],
#   ignore_headers: private,
#   dependencies: [glib, gobject, declare_dependency(link_with : [libgmime]), declare_dependency(link_with : [libutil])],
#   check: true,
#   install: true,
# )
