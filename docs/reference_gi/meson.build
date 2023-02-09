expand_content_md_files = [
  'overview.md',
  'tutorial.md',
  'streams.md',
  'filters.md',
  'data-wrappers.md',
  'changelog.md',
]

gidocgen_common_args = [
  '--quiet',
  '--no-namespace-dir',
]

docs_dir = gmime_datadir / 'doc'

toml_conf = configuration_data()
toml_conf.set('version', meson.project_version())

gidocgen = find_program('gi-docgen', required: get_option('gtk_doc'), native: true)

doc_dependencies = [ 'GObject-2.0', 'Gio-2.0' ]

if get_option('gtk_doc')
  gmime_toml = configure_file(
    input: 'gmime.toml.in',
    output: 'gmime.toml',
    configuration: toml_conf,
    install: true,
    install_dir: docs_dir / gmime_package,
  )
  custom_target('gmime-doc',
    input: [ gmime_toml, gmime_gir[0] ],
    output: gmime_package,
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
    depend_files: [ expand_content_md_files ],
    build_by_default: true,
    install: true,
    install_dir: docs_dir,
    )
endif
