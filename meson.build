project('colr', 'c',
  default_options : [ 'b_sanitize=address' ])

# Add Math Library
cc = meson.get_compiler('c')

deps = [ dependency('gtk+-3.0'), dependency('x11'), cc.find_library('m', required: false) ]

gnome = import('gnome')
resources = gnome.compile_resources(
  'colr-resources',
  'resources/index.xml',
  source_dir: 'resources',
  c_name: 'colr'
)

src = [
  'src/app.c',
  'src/util.c',
  'src/ui/ui.c',
  'src/color/color.c',
  'src/ui/init.c',
  'src/ui/editor-hsv.c',
  'src/ui/editor-rgb.c',
  'src/ui/editor.c',
  'src/ui/about.c',
  resources
]

executable('colr', sources : src, dependencies : deps)

